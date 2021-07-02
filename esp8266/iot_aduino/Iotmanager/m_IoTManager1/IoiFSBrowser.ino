
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
  if (path.endsWith("/")) path += "home.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
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
    if (!filename.startsWith("/")) filename = "/" + filename;
    DBG_OUTPUT_PORT.print("handleFileUpload Name: "); DBG_OUTPUT_PORT.println(filename);
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
void handle_Set_Ssid() {
  String ssid_arg = server.arg("ssid");            // Получаем значение ssid из запроса сохраняем в глобальной переменной
  String password_arg = server.arg("password");    // Получаем значение password из запроса сохраняем в глобальной переменной
  //saveConfig();                        // Функция сохранения данных во Flash
  Serial.println("Recieved:" + ssid_arg + password_arg);
  server.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
};
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
  server.on("/list", HTTP_GET, handleFileList);
  server.on("/home", HTTP_GET, []() {
    handleFileRead("/home.htm");
  });
  server.on("/settings.txt", handle_ConfigJSON); // формирование configs.json страницы для передачи данных в web интерфейс

  //server.on("/other_setup",[](){ handleFileRead("/other_setup.htm");});


  server.on("/other_setup", HTTP_POST, []() {
    mqttServerName = server.arg("mqttServerName");
    mqttport = server.arg("mqttport").toInt();
    mqttuser = server.arg("mqttuser");
    mqttpass = server.arg("mqttpass");
    prefix = server.arg("prefix");
    deviceID = server.arg("deviceID");

    String smtp_arr_str = server.arg("smtp_arr"); strcpy(smtp_arr, smtp_arr_str.c_str());
    smtp_port = server.arg("smtp_port").toInt();
    to_email_addr = server.arg("to_email_addr");
    from_email_addr = server.arg("from_email_addr");
    emaillogin = server.arg("emaillogin");
    password_email = server.arg("password_email");


    String softAP_ssid_str = server.arg("softAP_ssid"); strcpy(softAP_ssid, softAP_ssid_str.c_str());
    String softAP_password_str = server.arg("softAP_password"); strcpy(softAP_password, softAP_password_str.c_str());
    String ssid_str = server.arg("ssid"); strcpy(ssid, ssid_str.c_str());
    String password_str = server.arg("password"); strcpy(password, password_str.c_str());


    Serial.println("POST_RECIEVED:");

    if ( saveConfig()) {
      Serial.println("CONFIG SAVED:OK");

    };
    handleFileRead("/other_setup.htm");
  });
  server.on("/other_setup", []() {
    handleFileRead("/other_setup.htm");
  });
  /////////////////////////

  server.on("/ssid", handle_Set_Ssid); // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
  //server.on("/ssid", handle_Set_Ssid); // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
  /////////////////////////
  server.on("/IR_setup", []() {
    handleFileRead("/IR_setup.htm");
  });
  server.on("/pin_setup.txt", handle_ConfigJSON_pinSetup); // формирование configs.json страницы для передачи данных в web интерфейс
  server.on("/pin_setup", HTTP_POST, []() {
    String jsonrecieve = server.arg("json_name");

    Serial.println(jsonrecieve);
    if (savePinSetup(jsonrecieve)) {
      Serial.println("new Pinsetup");
    }
    updatepinsetup(jsonrecieve);
    Serial.println();
    Serial.println("recieve nested");
    handleFileRead("/pin_setup.htm");
  });
  server.on("/pin_setup", []() {
    handleFileRead("/pin_setup.htm");
  });
  server.on("/condition", []() {
    handleFileRead("/condition.htm");
  });


  //server.on("/home", HTTP_POST, [](){ server.send(200, "text/plain", ""); }, handleFileRead("/home.htm"));
  //handleFileRead("/home.htm");
  server.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm")) server.send(404, "text/plain", "FileNotFound");
  });

  server.on("/edit", HTTP_PUT, handleFileCreate);
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  server.on("/edit", HTTP_POST, []() {
    server.send(200, "text/plain", "");
  }, handleFileUpload);

  //called when the url is not defined here
  //use it to load content from SPIFFS
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  //get heap status, analog input value and all GPIO statuses in one json call
  server.on("/all", HTTP_GET, []() {
    String json = "{";
    json += "\"heap\":" + String(ESP.getFreeHeap());
    json += ", \"analog\":" + String(analogRead(A0));
    json += ", \"gpio\":" + String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });
  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");

}

void loop_FS(void) {
  server.handleClient();
}

