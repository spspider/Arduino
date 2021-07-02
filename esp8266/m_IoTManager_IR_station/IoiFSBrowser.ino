
/*
  FSWebServer - Example WebServer with SPIFFS backend for esp8266
  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the ESP8266WebServer library for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  upload the contents of the data folder with MkSPIFFS Tool ("ESP8266 Sketch Data Upload" in Tools menu in Arduino IDE)
  or you can upload the contents of a folder if you CD in that folder and run the following command:
  for file in `ls -A1`; do curl -F "file=@$PWD/$file" esp8266fs.local/edit; done

  access the sample web page at http://esp8266fs.local
  edit the page by going to http://esp8266fs.local/edit
*/
//#include <Main.h>;
//ESP8266WebServer server(80);
//holds the current upload

File fsUploadFile;

//format bytes
String formatBytes(size_t bytes) {
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  } else {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

String getContentType(String filename) {
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) {
  DBG_OUTPUT_PORT.println("handleFileRead: " + path);
  if ((path == "/bootstrap.min.css") || (path == "/chart.min.js")) {
    Serial.println("CASHE_CONTROL: " + path);
    server.sendHeader("Cache-Control", "public, max-age=604800, must-revalidate");
    server.sendHeader("Pragma", "public");
    server.sendHeader("Expires", "604800");
  }
  //if ((path.indexOf(".") == -1) && (!path.endsWith("/"))) path += ".htm";
  if (path.endsWith("/")) path += "home.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    ESP_busy = true;
    if (contentType != "text/plain") {
      //server.sendHeader("Cache-Control", "public, max-age=86400, must-revalidate");
      //server.sendHeader("Pragma", "public");
      //server.sendHeader("Expires", "86400");
    }

    size_t sent = server.streamFile(file, contentType);
    server.client().stop();
    file.close();
    return true;
  }
  return false;
}

void handleFileUpload() {
  if (server.uri() != "/edit") return;
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (filename == "pin_setup.txt") {
      (updatepinsetup(String((char *)upload.buf))) ? server.send(200, "text/plain", "ok") : server.send(200, "text/plain", "FAIL!");
    } else if (filename == "other_setup.txt") {
      (loadConfig(String((char *)upload.buf))) ? server.send(200, "text/plain", "ok") : server.send(200, "text/plain", "FAIL!");
    }
    else if (filename.indexOf("Condition") != -1) {
      //Serial.println("load Condition");
      (load_Current_condition(String((char *)upload.buf))) ? server.send(200, "text/plain", "ok") : server.send(200, "text/plain", "FAIL!");
    }
    DBG_OUTPUT_PORT.print("handleFileUpload Name: "); DBG_OUTPUT_PORT.println(filename);
    if (!filename.startsWith("/")) filename = "/" + filename;
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    //DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile)
      fsUploadFile.close();
    DBG_OUTPUT_PORT.print("handleFileUpload Size: "); DBG_OUTPUT_PORT.println(upload.totalSize);
    server.send(200, "text/plain", "ok");
  }
}

void handleFileDelete() {
  if (server.args() == 0) return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  DBG_OUTPUT_PORT.println("handleFileDelete: " + path);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (!SPIFFS.exists(path))
    return server.send(404, "text/plain", "FileNotFound");
  SPIFFS.remove(path);
  server.send(200, "text/plain", "");
  path = String();
}

void handleFileCreate() {
  if (server.args() == 0)
    return server.send(500, "text/plain", "BAD ARGS");
  String path = server.arg(0);
  DBG_OUTPUT_PORT.println("handleFileCreate: " + path);
  if (path == "/")
    return server.send(500, "text/plain", "BAD PATH");
  if (SPIFFS.exists(path))
    return server.send(500, "text/plain", "FILE EXISTS");
  File file = SPIFFS.open(path, "w");
  if (file)
    file.close();
  else
    return server.send(500, "text/plain", "CREATE FAILED");
  server.send(200, "text/plain", "");
  path = String();
}
void handle_sendEmail() {
  //String email_txt = server.arg("Email"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  if (sendEmail(server.arg("Email"))) {
    server.send(200, "text/plain", "send OK"); // отправляем ответ о выполнении
  }
  else {
    server.send(200, "text/plain", "Fail"); // отправляем ответ о выполнении

  }
}
unsigned long ConvertCharToLong(char *pSrc) {
  int i = 1;
  static unsigned long result = (int)pSrc[0] - '0';
  while (i < strlen(pSrc)) {
    result = result * 10 + ((int)pSrc[i] - '0');
    ++i;
  }
  return result;
}
unsigned long ConvertToLong(String x) {
  static unsigned long y;
  for (int i = 0; i < x.length(); i++) {
    char c = x.charAt(i);
    if (c < '0' || c > '9') break;
    y *= 10;
    y += (c - '0');
  }
  return y;
}
void handle_setTime() {
  //unsigned long now_time = 1508149905UL;
  //String DateTime = server.arg("DateTime");
  Serial.println(server.arg("DateTime"));

  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(server.arg("DateTime"));

  if (!root.success()) {
    Serial.println("FAIL DATE!!!!" + server.arg("DateTime"));
    return;
  }
  unsigned int hour_r = root["h"];
  unsigned int min_r = root["m"];
  unsigned int day_r = root["d"];
  unsigned int month_r = root["o"];
  unsigned int year_r = root["y"];

  if (timeStatus() == timeNotSet) {
    Serial.println("Time no set");
  }
  if (timeStatus() == timeNeedsSync ) {
    Serial.println("timeNeedsSync");
  }
  //int *Time = getTime_min_hour(hour_min);
  int timestat = timeStatus();
  setTime(hour_r, min_r, 0, day_r, month_r, year_r);
  //Serial.println(String(Time[0]) + ":" + String(Time[1]));
  Serial.println(String(hour_r) + ":" + String(min_r));
  if ((timestat == timeNotSet) && (timeStatus() == timeSet)) {
    setup_alarm();
  }
  server.send(500, "text/plain", "TimeSetted");
}
void handleFileList() {
  if (!server.hasArg("dir")) {
    server.send(500, "text/plain", "BAD ARGS");
    return;
  }

  String path = server.arg("dir");
  DBG_OUTPUT_PORT.println("handleFileList: " + path);
  Dir dir = SPIFFS.openDir(path);
  path = String();

  String output = "[";
  while (dir.next()) {
    File entry = dir.openFile("r");
    if (output != "[") output += ',';
    bool isDir = false;
    output += "{\"type\":\"";
    output += (isDir) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += String(entry.name()).substring(1);
    output += "\"}";
    entry.close();
  }

  output += "]";
  server.send(200, "text/json", output);
}

void handle_saveIR() {
  String IRjson = server.arg("IR");
  saveCommonFiletoJson("IRButtons", IRjson);
  updateIR();
}

void setup_FS(void) {

  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      DBG_OUTPUT_PORT.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    DBG_OUTPUT_PORT.printf("\n");
  }



  //SERVER INIT
  //list directory

}
void handleAJAX() {
  //String requestString = server.arg("json"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(server.arg("json"));
  //Serial.println("" + requestString);
  if (!root.success()) {
    Serial.println("FAIL AJAX!!!!" + server.arg("json"));
    return;
  }
  char Topic_is = root["t"];
  int newValue = root["v"];

  callback_scoket(Topic_is, newValue);
}
void Server_begin() {

  server_init();
  Captive_server_init();
  // server.begin();
}
void server_init() {
  server.on("/sendAJAX", handleAJAX);
  server.on("/sendEmail", handle_sendEmail);
  server.on("/list", HTTP_GET, handleFileList);
  server.on("/", HTTP_GET, []() {
    handleFileRead("/home.htm");
  });
  server.on("/setDate", HTTP_POST, handle_setTime);
  /*
    server.on("/other_setup", HTTP_POST, []() {

      if (server.arg("jsonArray")) {
        Serial.println(server.arg("jsonArray"));
        String jsonString = server.arg("jsonArray");
        if ( saveCommonFiletoJson("other_setup", jsonString)) {
          Serial.println("CONFIG SAVED:OK");
          server.send(200, "text/plain", "OK");
        } else {
          server.send(200, "text/plain", "Error");
        }
      }
      //handleFileRead("/other_setup.htm");
    });
  */
  server.on("/other_setup", []() {
    handleFileRead("/other_setup.htm");
  });
  /*
    server.on("/condition_setup", HTTP_POST, []() {
      String jsonrecieve = server.arg("json_name");
      SaveCondition(jsonrecieve);
      server.send(200, "text/plain", "OK");
      //Serial.println(jsonrecieve);

    }
             );
  */
  /////////////////////////
  //IR//////////////////////////
  server.on("/aRest", HTTP_GET, []() { /*192.168.1.108/aRest?Json={pin:1,val:100}*/
    //String json = server.arg("Json");
    Serial.println(server.arg("Json"));
    makeAres_sim(server.arg("Json"));
  });

  /*
    server.on("/CommonSave", []() { //получаем методом AJAX включаем
      saveCommonFiletoJson(server.arg("fileName"), server.arg("json"));
    });
  */
  //server.on("/CommonDelete", HTTP_DELETE, handleFileDelete);
  server.on("/IR_setup", []() {
    //IR_loop = true;
    //Serial.println("IR true");
    Page_IR_opened = false;
    handleFileRead("/IR_setup.htm");
  });

  server.on("/WaitIR", []() { //получаем методом AJAX включаем IR
    Page_IR_opened = true;
  });
  /*
    server.on("/SaveIR", []() {//!!!!!!!!!!!!!!!!!!можно переделать
      handle_saveIR();
      //setup_IR(true);
      //IR_loop = false;
      //IR_loop = false;
    });
  */
  /*
    server.on("/IRButtons.txt", []() {
    String jsonConfig = readCommonFiletoJson("IRButtons");
    server.send(200, "text/json", jsonConfig);
    });
  */
  ////////////////////////////////////
  //server.on("/pin_setup.txt", handle_ConfigJSON_pinSetup); // формирование configs.json страницы для передачи данных в web интерфейс
  /*
    server.on("/pin_setup", HTTP_POST, []() {
    String jsonrecieve = server.arg("json_name");
    Serial.println(jsonrecieve);
    if (saveCommonFiletoJson("pin_setup", jsonrecieve)) {
      Serial.println("new Pinsetup");
    }
    updatepinsetup(jsonrecieve);
    Serial.println();
    Serial.println("recieve nested");
    handleFileRead("/pin_setup.htm");
    });
  */
  server.on("/pin_setup", []() {
    handleFileRead("/pin_setup.htm");
  });
  server.on("/condition", []() {
    handleFileRead("/condition.htm");
  });
  server.on("/help", []() {
    handleFileRead("/help.htm");
  });
  server.on("/License", []() {
    if (server.arg("code")) {
      char code[5];
      server.arg("code").toCharArray(code, 5);
      saveEEPROM(0,code);
    } else {
      char* isLicensed = getEEPROM(0);
      server.send(404, "text/plain", isLicensed);
    }
  });
  //server.on("/home", HTTP_POST, [](){ server.send(200, "text/plain", ""); }, handleFileRead("/home.htm"));
  //handleFileRead("/home.htm");

  server.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm")) server.send(404, "text/plain", "FileNotFound");
  });

  server.on("/edit", HTTP_PUT, handleFileCreate);
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  server.on("/edit", HTTP_POST, []() {

  }, handleFileUpload);

  //called when the url is not defined here
  //use it to load content from SPIFFS
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  //get heap status, analog input value and all GPIO statuses in one json call
  /*
    server.on("/all", HTTP_GET, []() {
    String json = "{";
    json += "\"heap\":" + String(ESP.getFreeHeap());
    json += ", \"analog\":" + String(analogRead(A0));
    json += ", \"gpio\":" + String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
    json += "}";
    server.send(200, "text/json", json);
    json = String();
    });
  */
  //server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");

}
void Captive_server_init() {
  //server.on("/setup", handleRoot);
  //  server.on("/wifi", handleWifi);
  server.on("/wifi", []() {
    handleFileRead("/wifi_setup.htm");
  });
  server.on("/wifiList", handleWifilist);
  server.on("/wifisave", handleWifiSave);
  server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.

  server.serveStatic("/font", SPIFFS, "/font", "max-age=86400");
  server.serveStatic("/js",   SPIFFS, "/js"  , "max-age=86400");
  server.serveStatic("/css",  SPIFFS, "/css" , "max-age=86400");


  //server.onNotFound ( handleNotFound );
  //server.begin(); // Web server start
  Serial.println("HTTP server started");

}
void loop_FS(void) {

  server.handleClient();
}

