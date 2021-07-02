//var xhr = new XMLHttpRequest();   // new HttpRequest instance 



function createXmlHttpObject() {
  if (window.XMLHttpRequest) {
    xmlHttp = new XMLHttpRequest();
  } else {
    xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');
  }
  return xmlHttp;
}

function makePOST() {
  numberChosed=document.getElementById("sel1").value;
  saveAllStrings(numberChosed);
  xmlHttp = createXmlHttpObject();
  var jsonStr2 = {
    "id": ["1", "2", "3", "4", "5", "6", "6", "7"],
    "pin": [10, 5, 0, 17, 2, 15, 12, 13],
    "page": ["Light", "Light", "Light", "Light", "Light", "Light", "Light", "Light"],
    "descr": ["Light-0", "Light-1", "Dimmer", "ADC", "Light light", "RED", "GREEN", "BLUE"],
    "widget": ["toggle", "toggle", "range", "small-badge", "toggle", "range", "range", "range"]
  };
var jsonStrBack={
  "pinmode":["out","out","pwm","adc","in","pwm","pwm","pwm"],
  "id":["0","1","2","3","4","5","6","7"],
  "pin":[4,5,0,17,2,15,12,13],
  "page":["Kitchen","Kitchen","Kitchen","Kitchen","Outdoor","Kitchen","Kitchen","Kitchen"],
  "descr":["Light-0","Light-1","Dimmer","ADC","Garden light","RED","GREEN","BLUE"],
  "widget":["toggle","toggle","range","small-badge","toggle","range","range","range"],
  "defaultVal":[0,0,1023,0,1,0,0,0]};
  var inputs = document.getElementById("string_descr6").value;
  inputs += document.getElementById("string_widget6").value;
jsonStr2={};
jsonStr2.numberChosed=numberChosed;
jsonStr2.pinmode=one_string_saved;
jsonStr2.id=string_id_saved;
jsonStr2.pin=string_pin_saved;
jsonStr2.page=string_page_saved;
jsonStr2.descr=string_descr_saved;
jsonStr2.widget=string_widget_saved;
jsonStr2.defaultVal=string_defaultVal_saved;





  var json_upload = "json_name=" + JSON.stringify(jsonStr2);

  xmlHttp.open("POST", '/pin_setup', true);
  xmlHttp.setRequestHeader('Content-Type', 'application/json; charset=UTF-8');
  xmlHttp.send(json_upload);

  xmlHttp.onloadend = function() {
    // done
    //alert(json_upload);
    document.getElementById("test").innerHTML = json_upload;
    // document.getElementById("test").innerHTML += json_upload;
  };


  //alert(data);
}
//var form;