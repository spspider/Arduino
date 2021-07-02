function makePOST(){
var data={};

var jsonStr2 = {
  "id": ["1", "2", "3", "4", "5", "6", "6", "7"],
  "pin": [10, 5, 0, 17, 2, 15, 12, 13],
  "page": ["Light", "Light", "Light", "Light", "Light", "Light", "Light", "Light"],
  "descr": ["Light-0", "Light-1", "Dimmer", "ADC", "Light light", "RED", "GREEN", "BLUE"],
  "widget": ["toggle", "toggle", "range", "small-badge", "toggle", "range", "range", "range"]
};

var inputs = document.getElementById("string_descr[6]").value;
inputs += document.getElementById("string_widget[6]").value;
  

var json_upload = "json_name=" + JSON.stringify(jsonStr2);
var xhr = new XMLHttpRequest();   // new HttpRequest instance 
xhr.open("POST", '/pin_setup', true)
xhr.setRequestHeader('Content-Type', 'application/json; charset=UTF-8');
xhr.send(json_upload);

  xhr.onloadend = function () {
    // done
    //alert(json_upload);
    document.getElementById("test").innerHTML += json_upload;
  };
  

//alert(data);
}
//var form;
