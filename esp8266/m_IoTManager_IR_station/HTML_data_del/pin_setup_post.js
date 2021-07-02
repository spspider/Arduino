//var xhr = new XMLHttpRequest();   // new HttpRequest instance 


function createXmlHttpObject() {
    if (window.XMLHttpRequest) {
        xmlHttp = new XMLHttpRequest();
    } else {
        xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');
    }
    return xmlHttp;
}
function toJSONString(form) {
    var obj = {};
    var elements = form.querySelectorAll("input, select, textarea");
    for (var i = 0; i < elements.length; ++i) {
        var element = elements[i];
        var name = element.name;
        var value = element.value;

        if (name) {
            obj[name] = value;
        }
    }

    return JSON.stringify(obj);
}
function makePOST() {

    var form = document.getElementById("form");
    var output = document.getElementById("output");
    var JsonStringForm = (toJSONString(form));
    var ParsedForm = JSON.parse(JsonStringForm);
    //alert(ParsedForm);
   // alert(JsonStringForm);
    //numberChosed = document.getElementById("sel1").value;
    numberChosed = getVal("sel1");
    saveAllStrings(numberChosed);

    //xmlHttp = createXmlHttpObject();
    var jsonStr2 = {
        "id": ["1", "2", "3", "4", "5", "6", "6", "7"],
        "pin": [10, 5, 0, 17, 2, 15, 12, 13],
        "page": ["Light", "Light", "Light", "Light", "Light", "Light", "Light", "Light"],
        "descr": ["Light-0", "Light-1", "Dimmer", "ADC", "Light light", "RED", "GREEN", "BLUE"],
        "widget": ["toggle", "toggle", "range", "small-badge", "toggle", "range", "range", "range"]
    };
    var jsonStrBack = {
        "pinmode": ["out", "out", "pwm", "adc", "in", "pwm", "pwm", "pwm"],
        "id": ["0", "1", "2", "3", "4", "5", "6", "7"],
        "pin": [4, 5, 0, 17, 2, 15, 12, 13],
        "page": ["Kitchen", "Kitchen", "Kitchen", "Kitchen", "Outdoor", "Kitchen", "Kitchen", "Kitchen"],
        "descr": ["Light-0", "Light-1", "Dimmer", "ADC", "Garden light", "RED", "GREEN", "BLUE"],
        "widget": ["toggle", "toggle", "range", "small-badge", "toggle", "range", "range", "range"],
        "defaultVal": [0, 0, 1023, 0, 1, 0, 0, 0]
    };
    //document.getElementById("test").innerHTML += "works" + numberChosed;

    var one_string_saved_pinmode = [];
    var string_widget_saved_index = [];
    var string_IrBtnId_index = ["0"];
    one_string_saved.slice(0, inputPinmode);
    string_widget_saved.slice(0, numberChosed);

    for (i = 0; i < numberChosed; i++) {
        one_string_saved_pinmode[i] = parseInt(inputPinmode.indexOf(one_string_saved[i]));
        string_widget_saved_index[i] = parseInt(inputWidjet.indexOf(string_widget_saved[i]));
        string_IrBtnId_index[i] = parseInt(optionsIR_array.indexOf(string_IR_saved[i]) - 1);
        string_pin_saved[i] = parseInt(string_pin_saved[i]);
        string_pin_saved[i] = (!string_pin_saved[i]) ? -1 : string_pin_saved[i];
    }

    jsonStr2 = {};
    jsonStr2.numberChosed = parseInt(numberChosed);
    jsonStr2.pinmode = one_string_saved_pinmode; //one_string_saved.slice(0,inputPinmode);;//selectedindex  inputPinmode.indexOf(one_string_saved[i]);
    //jsonStr2.id = string_id_saved.slice(0, numberChosed);
    jsonStr2.pin = string_pin_saved.slice(0, numberChosed);
    jsonStr2.descr = string_descr_saved.slice(0, numberChosed);
    jsonStr2.widget = string_widget_saved_index;//string_widget_saved.slice(0, numberChosed); //selectedindex
    jsonStr2.IrBtnId = string_IrBtnId_index;
    //jsonStr2.delimVal=string_delimeterVal_saved.slice(0, numberChosed);
    jsonStr2.defaultVal = string_defaultVal_saved.slice(0, numberChosed);
    jsonStr2.aDiv = parseFloat(analogDivider);
    jsonStr2.aSusbt = parseFloat(analogSubtracter);
    jsonStr2.router=router!=="no"?parseInt(router):255;

    Object.assign(jsonStr2, ParsedForm);
    var json_upload = "json_name=" + JSON.stringify(jsonStr2);
    setHTML("test2", JSON.stringify(jsonStr2));
    saveData("pin_setup.txt", jsonStr2);
    /*
     xmlHttp.open("POST", '/pin_setup', true);
     xmlHttp.setRequestHeader('Content-Type', 'application/json; charset=UTF-8');
     xmlHttp.send(json_upload);

     xmlHttp.onloadend = function () {
     // done
     //alert(json_upload);
     document.getElementById("test").innerHTML = json_upload;
     loadAllStrings(numberChosed);

     // document.getElementById("test").innerHTML += json_upload;
     //location.reload();
     };
     xmlHttp.onreadystatechange = function () {
     if (xmlHttp.readyState === 4 && xmlHttp.status == "200") {
     //callback(xmlHttp.responseText);
     alert(xmlHttp.responseText);
     }
     }
     */
    //alert(data);
}
function saveData(filename, data) {

    var xmlHttp = createXmlHttpObject();

    var file = new Blob([JSON.stringify(data, null, 2)], {type: "text/plain;charset=utf-8"});
    var a = new FormData();
    a.append("data", file, filename);
    xmlHttp.open("POST", "/edit");
    xmlHttp.send(a);

    xmlHttp.onreadystatechange = function () {

        if (xmlHttp.readyState == 4) {
            if (xmlHttp.status != 200) alert("ERROR[" + xmlHttp.status + "]: " + xmlHttp.responseText);
            else {
                alert(xmlHttp.responseText);

            }
        }


    }
    xmlHttp.onloadend = function () {

    }
}
function isNumber(n) {
    return !isNaN(parseFloat(n)) && isFinite(n);
}
//var form;