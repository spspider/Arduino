﻿document.addEventListener("DOMContentLoaded", loadIR);
//document.addEventListener("DOMContentLoaded", load2);
//document.addEventListener("DOMContentLoaded", jsonParse_fortest);
//
var inputOption = new Array();
var inputPinmode = new Array();
var inputPage = new Array();
var inputWidjet = new Array();
var inputPin = new Array();
var inputId = new Array();

var description;
//var inputDescr = new Array();
inputPin[-1] = "no";
inputPin[0] = 0;
//inputPin[2] = 1;//txd
inputPin[2] = 2;//подключен приемник IR
//inputPin[4] = 3;//rxd
inputPin[4] = 4;
inputPin[5] = 5;
//inputPin[6] = 6;//перезагрузка
//inputPin[7] = 7;//перезагрузка
//inputPin[8] = 8;//перезагрузка
//inputPin[9] = 9;//перезагрузка
inputPin[10] = 10; //d3
//inputPin[11] = 11;//перезагрузка
inputPin[12] = 12;
inputPin[13] = 13;
inputPin[14] = 14;
inputPin[15] = 15;// подключен передатчик IR
inputPin[16] = 16; //xpd
inputPin[17] = 17; //adc


inputPinmode[0] = "нет";
inputPinmode[1] = "вход";
inputPinmode[2] = "выход";
inputPinmode[3] = "ШИМ";
inputPinmode[4] = "ADC";
inputPinmode[5] = "медл. ШИМ";
inputPinmode[6] = "DHT 1.1 Temp";
inputPinmode[7] = "питание датчика MQ7";
inputPinmode[8] = "DHT 1.1 Влажность";
//inputPinmode[9] = "zero-cross вход";
//inputPinmode[10] = "PWM AC";

//inputPinmode[7] = "chart";

inputPage[0] = 'unknown';
inputPage[1] = 'Kitchen';
inputPage[2] = 'Outdoor';

inputWidjet[0] = 'unknown';
inputWidjet[1] = 'переключатель';
inputWidjet[2] = 'кнопка';
inputWidjet[3] = 'ползунок';
inputWidjet[4] = 'progress-bar';
inputWidjet[5] = 'график';
inputWidjet[6] = 'данные текст';

var one_string_saved = new Array();
var string_page_saved = new Array();
var string_descr_saved = new Array();
var string_widget_saved = new Array();
var string_pin_saved = new Array();
var string_id_saved = new Array();
var string_defaultVal_saved = new Array();
var string_delimeterVal_saved = [];
var string_IR_saved = [];
var numberChosed = 0;
var jsonStr2 = {
    "id": ["0", "1", "2", "3", "4", "5", "6", "7"],
    "pin": [10, 5, 0, 17, 2, 15, 12, 13],
    "page": ["Kitchen", "Kitchen", "Kitchen", "Kitchen", "Outdoor", "Kitchen", "Kitchen", "Kitchen"],
    "descr": ["Light-0", "Light-1", "Dimmer", "ADC", "Garden light", "RED", "GREEN", "BLUE"],
    "widget": ["toggle", "toggle", "range", "small-badge", "toggle", "range", "range", "range"]
};
var jsonStr = "{}";
var jsonStrIR = {};
var dataOther = {};
var optionsIR_array = [];
//var inputIRArray=[];
//var xmlHttp = createXmlHttpObject();
//load();
///////////////////////////////////////////////////


////////////////////////////////////////////////////
var openFile = function (event) {
    var input = event.target;
    var reader = new FileReader();
    reader.onload = receivedText;
    reader.readAsText(input.files[0]);
};

function receivedText(e) {
    lines = e.target.result;
    var newArr = JSON.parse(lines);
    //set(newArr);
}

function load2() {
    //loadIR();
    var data = {};
    try {
        readTextFile("pin_setup.txt", function (text) {
            try {
                data = JSON.parse(text);
                document.getElementById("test").innerHTML = JSON.stringify(data);
            } catch (e) {
                document.getElementById("test").innerHTML += e + text;
                makeInputs();
            }
            jsonStr = data;
            makeInputs(jsonStr);

            set(data);
            loadBodyFetchData(jsonStr);
            makeDivider();
            setRouterPin();

        });
    } catch (e) {
        makeInputs();
    }

}
////////////////////////////////
function loadBodyFetchData(jsonStr) {
    ///////////////////////////////////////////////
    var data2 = jsonStr;
    var data = document.getElementsByTagName('body')[0].innerHTML;
    var new_string;
    for (var key in data2) {
        new_string = data.replace(new RegExp('{{' + key + '}}', 'g'), data2[key]);
        data = new_string;
    }
    document.getElementsByTagName('body')[0].innerHTML = new_string;

////////////////////////////////////
}
function callback_Other(text) {
    try {
        dataOther = JSON.parse(text);
        load2();
    } catch (e) {
        document.getElementById("test").innerHTML += e + text;
    }
}
function loadIR() {
    var data = {};
    try {
        readTextFile("IRButtons.txt", function (text) {
            try {
                data = JSON.parse(text);
            } catch (e) {
                document.getElementById("test").innerHTML += e;
            }
            jsonStrIR = data;
            optionsIR_array = (jsonStrIR.name) ? jsonStrIR.name : optionsIR_array[0] = "нет";
            optionsIR_array.unshift("нет");
            //makeIRsetup();
            readTextFile("other_setup.txt", callback_Other);

        });
    } catch (e) {
    }

}

function createXmlHttpObject() {
    if (window.XMLHttpRequest) {
        xmlHttp = new XMLHttpRequest();
    } else {
        xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');
    }
    return xmlHttp;
}

function readTextFile(file, callback) {
    //var rawFile = new XMLHttpRequest();
    var xmlHttp = createXmlHttpObject();
    xmlHttp.overrideMimeType("application/json");
    xmlHttp.open("GET", file, true);
    xmlHttp.onreadystatechange = function () {
        if (xmlHttp.readyState === 4) {
            if (xmlHttp.status === 200) {
                callback(xmlHttp.responseText);
            } else {
                callback({"response": 0});
            }
        }
    }
    xmlHttp.send(null);
}
///////////////////////////////////

function makeInputs(jsonStr) {

    var numberChosed = 0;
    if (document.getElementById("sel1")) {
        numberChosed = document.getElementById("sel1").value;

    } else if (jsonStr) {

        numberChosed = jsonStr.numberChosed;

    }
    set();

    number_buttons = 8;
    var add_option;
    var selected = "";
    for (i = 0; i <= number_buttons; i++) {

        if (i == numberChosed) {
            selected = " selected";
        } else {
            selected = "";
        }


        add_option += "<option" + selected + ">" + i + "</option>"
    }
    document.getElementById("sel1div").innerHTML = "<select class='form-control' id='sel1' onchange='makeInputs();'>" + add_option + "</select>";

}

function set(jsonStr) {


    if (document.getElementById("sel1")) {
        numberChosed = document.getElementById("sel1").value;
        //loadAllStrings(numberChosed);
    }

    if (jsonStr) {
        numberChosed = jsonStr.numberChosed;
    }
    saveAllStrings(numberChosed);

    //numberChosed = 8;
    var one_string = "";
    var one_str_pinmode = "";
    var one_string_page = "";
    var one_string_descr = "";
    var one_string_widget = "";
    var one_string_pin = "";
    var one_string_defaultVal = "";
    var one_string_id = "";
    var one_string_href = "";
    var description = "";
    var delimeterVal = 0;
    numberInputOptions = 2;


    var result_table =
        "<tr>" +
        "<td >вход/выход</td>" +

        "<td >имя</td>" +
        "<td >тип</td>" +
        "<td>порт</td>" +
        "<td>пульт IR</td>" +
        "<td >умолч.</td>" +
        "<td>условие</td>" +
        "</tr>";
    for (var i1 = 0; i1 < numberChosed; i1++) {
        btnId = i1;
        result_table += "<tr id='tr" + btnId + "'>" +
            "<td id='Inputs_table" + btnId + "'></td>" +

            "<td id='Inputs_descr_table" + btnId + "'></td>" +
            "<td id='Inputs_widget_table" + btnId + "'></td>" +
            "<td id='Inputs_pin_table" + btnId + "'></td>" +
            "<td id='Inputs_IR_table" + btnId + "'></td>" +
            "<td id='Inputs_defaultVal_table" + btnId + "'></td>" +

            "<td id='condition_table" + btnId + "'></td>" +
            "<td id='MQTT_adress" + btnId + "'></td>" +
            "</tr>";
    }


    document.getElementById("table").innerHTML += "<tbody id='body_table'><tbody>";
    document.getElementById("body_table").innerHTML = result_table;


    for (var i = 0; i < numberChosed; i++) {
        // var options = "";
        //var optionsPage = "";


        if (jsonStr) {
            //inputPin = jsonStr.pin;
            inputId = jsonStr.id;
            description = jsonStr.descr[i];
            defaultVal = jsonStr.defaultVal[i];

            string_delimeterVal_saved[i] = 0;
            if (jsonStr.delimVal) {
                string_delimeterVal_saved[i] = jsonStr.delimVal[i];
                if (typeof jsonStr.delimVal[i] === 'undefined' || jsonStr.delimVal[i] === null) {
                    string_delimeterVal_saved[i] = 0;
                }
            }

            //inputPinmode=jsonStr.pinmode[i];
        } else {
            description = "";
        }
        //options = makeinOption(inputOption, i);
        optionsIR = makeinOption(optionsIR_array, 0);
        optionsPin = makeinOption(inputPin, 0);
        optionsWidjet = makeinOption(inputWidjet, 0);
        optionsPinmode = makeinOption(inputPinmode, 0);
        description = "button:" + i;
        defaultVal = 0;
        if (jsonStr) {
            // optionsPage = makeinOption(inputPage, inputPage.indexOf(jsonStr.page[i]));
            //optionsWidjet = makeinOption(inputWidjet, inputWidjet.indexOf(jsonStr.widget[i]));
            optionsWidjet = makeinOption(inputWidjet, jsonStr.widget[i]);
            optionsPinmode = makeinOption(inputPinmode, jsonStr.pinmode[i]);
            optionsPin = makeinOption(inputPin, inputPin.indexOf(parseInt(jsonStr.pin[i])));
            description = jsonStr.descr[i];
            if (jsonStr.IrBtnId) {
                optionsIR = makeinOption(optionsIR_array, parseInt(jsonStr.IrBtnId[i]) + 1);
            }
            defaultVal = jsonStr.defaultVal[i];
            //
            MQTT_adress = "MQTT топик: " + dataOther.deviceID + "/" + description + "/" + i + "\n";
            MQTT_adress += "MQTT топик управление: " + dataOther.deviceID + "/" + description + "/" + i + "/" + "status" + "\n";
            MQTT_adress += "удаленное управление HTTP: " + window.location.host + "/aRest?Json={pin:" + inputPin.indexOf(parseInt(jsonStr.pin[i])) + ",val:1}";
        }
        if (jsonStr) {
            saveAllStrings(numberChosed);
        }

        document.getElementById("Inputs_table" + i).innerHTML = "<select class='form-control' id='one_string" + i + "' onchange='choisedPinmode(" + i + ",true);'>" + optionsPinmode + "</select>";
        document.getElementById("Inputs_descr_table" + i).innerHTML += "<input type='text' class='form-control' id='string_descr" + i + "' value='" + description + "'>";
        document.getElementById("Inputs_widget_table" + i).innerHTML += "<select class='form-control' id='string_widget" + i + "' onchange='makesmth2();'>" + optionsWidjet + "</select>";
        document.getElementById("Inputs_pin_table" + i).innerHTML += "<select class='form-control' id='string_pin" + i + "' onchange='makesmth2();'>" + optionsPin + "</select>";
        setHTML("Inputs_IR_table" + i, "<select class='form-control' id='string_IR" + i + "''>" + optionsIR + "</select>");
        document.getElementById("Inputs_defaultVal_table" + i).innerHTML += "<input type='text' class='form-control' id='string_defaultVal" + i + "'value='" + defaultVal + "'</input>";
        //setHTML("Inputs_delimeter_table" + i, "<input type='text' class='form-control' id='string_delimeterVal" + i + "'value='" + string_delimeterVal_saved[i] + "'</input>");
        if (i < 3) {
            document.getElementById("condition_table" + i).innerHTML += "<a  class='btn btn-primary btn-xs' href='condition.htm?id=" + i + "'>условие:" + i + "</a><br>";
        }
        setHTML("MQTT_adress" + i, "<div title='" + MQTT_adress + "'><p>?</p></div>");

    }



    if (jsonStr) {
        for (var i = 0; i < numberChosed; i++) {

            setVal("one_string" + i, inputPinmode[jsonStr.pinmode[i]]);
            setVal("string_widget" + i, inputWidjet[jsonStr.widget[i]]);
            if (jsonStr.pin[i]) { //если содержится в файле
                index = jsonStr.pin[i]; //узнаем какой индекс у кнопки
                setVal("string_pin" + i, index);
            }
        }
        saveAllStrings(numberChosed);
    }

    loadAllStrings(numberChosed);

    for (var i = 0; i < numberChosed; i++) {
        choisedPinmode(i, false);
    }


    loadAllStrings(numberChosed);
}

//var IR_receive_output=0;
function makeIRsetup() {
    var ir_string = "<table class='table'>";
    var AviailablePinsT = inputPin.slice();
    AviailablePinsT.unshift(inputPin[-1]);
    var AviailablePinsR = inputPin.slice();
    AviailablePinsR.unshift(inputPin[-1]);
    AviailablePinsR.splice(AviailablePinsR.indexOf(0), 1);
    //AviailablePins.splice(inputPin.indexOf(null),1);
    var optionsIR_receive_output = makeinOption(AviailablePinsR, jsonStr.IR_rec ? jsonStr.IR_rec : 0);
    var optionsIR_output_output = makeinOption(AviailablePinsT, jsonStr.IR_LED ? jsonStr.IR_LED : 0);
    ir_string += "<tr><td>IR приемник</td><td><select class='form-control' id='IR_receive_output'>" + optionsIR_receive_output + "</select></td></tr>";
    ir_string += "<tr><td>IR LED светодиод</td><td><select class='form-control' id='IR_output_output'>" + optionsIR_output_output + "</select></td></tr></table>";
    setHTML("IR_setup", ir_string);
    IR_receive_output = jsonStr.IR_rec ? jsonStr.IR_rec : 0;
    IR_output_output = jsonStr.IR_LED ? jsonStr.IR_LED : 0;
    setVal("IR_receive_output", IR_receive_output);
    setVal("IR_output_output", IR_output_output);
    //var form = document.getElementById("form");

}
var analogDivider = 0;
var analogSubtracter = 0;
var router;
function makeDivider() {
    var divider = "";

    if (jsonStr) {
        analogDivider = !isNaN(parseInt(jsonStr.aDiv)) ? parseInt(jsonStr.aDiv) : 1;
        analogSubtracter = !isNaN(parseInt(jsonStr.aSusbt)) ? parseInt(jsonStr.aSusbt) : 0;
    }
    divider += "<table class='table' id='Divider_table'><tr><td>делитель аналогового выхода:</td>";
    divider += "<td><input type='text' class='form-control' id='analogDivider' value='" + analogDivider + "'</input></td></tr>";
    divider += "<tr><td>отнять:</td><td><input type='text' class='form-control' id='analogSubtracter'value='" + analogSubtracter + "'</input></td></tr></table>";
    setHTML("Divider", divider);

}
function setRouterPin() {
    ////////////////////////////////////
    var Divider_table = document.getElementById("Divider_table");
    var array = inputPin;
    var selectList = document.createElement("select");
    selectList.id = "router";
    // selectList.setAttribute("id", "router");
    selectList.className = "form-control";
    Divider_table.appendChild(selectList);

    if (jsonStr) {
        router = !isNaN(parseInt(jsonStr.router)) ? parseInt(jsonStr.router) : -1;
    }

    for (var i = -1; i < array.length; i++) {
        if (array[i] !== undefined) {
            var option = document.createElement("option");
            if (array[i]==router){
                //option.setAttribute("selected","true");
            }
            option.text = array[i];
            selectList.appendChild(option);
        }
    }
    // selectList.selectedIndex=2;
    if (jsonStr) {
        //router = !isNaN(parseInt(jsonStr.router)) ? parseInt(jsonStr.router) : -1;
    }
    //var table = document.getElementById("Divider_table");
    var row = Divider_table.insertRow(0);
    var cell1 = row.insertCell(0);
    var cell2 = row.insertCell(1);
    selectList.value=router;
    cell2.appendChild(selectList);
    cell1.innerHTML = "пин роутер";
    ////////////////////////////////////

}
function choisedPinmode(i, makeInput) {
    if (document.getElementById("one_string" + i)) {
        if (makeInput) {
            makeInputs();
        }
        //choised = document.getElementById("one_string" + i).value;
        choised = getVal("one_string" + i);
        switch (choised) {
            case inputPinmode[2]: //out

                break;
            case inputPinmode[3]: //pwm
                setVal("string_widget" + i, inputWidjet[3]); //range
                setDisable("string_widget" + i);
                break;
            case inputPinmode[4]: //adc
                setVal("string_widget" + i, inputWidjet[4]); //small-badge
                setVal("string_pin" + i, inputPin[14]); //17
                if (inputWidjet[jsonStr.widget[i]]) {
                    setVal("string_widget" + i, inputWidjet[jsonStr.widget[i]]); //small-badge
                }
                //setDisable("string_widget" + i);
                //setDisable("string_pin" + i);
                break;
            case inputPinmode[5]: //low_pwm
                if (document.getElementById("string_widget" + i)) {
                    document.getElementById("string_widget" + i).value = inputWidjet[3]; //range
                }
                break;
            case inputPinmode[6]:

                break;
            case inputPinmode[100]: //IR
                if (typeof jsonStrIR.name === "undefined") {
                    value = "<a class='btn btn-block btn-primary' href='\IR_setup'>Настройка IR</a>";
                    setHTML("Inputs_descr_table" + i, value);
                } else {

                    descr_new = "<select class='form-control' onchange='selectidIR(" + i + ")' id='string_descr" + i + "'></select>";
                    make_string_pin();
                    descr_new += "<a href='\IR_setup'>IR</a>";
                    setHTML("Inputs_descr_table" + i, descr_new);
                    value = makeinOption(jsonStrIR.name, 0);
                    setHTML("string_descr" + i, value);
                    setVal("string_widget" + i, inputWidjet[2]); //button
                    document.getElementById("string_widget" + i).disabled = true;
                    document.getElementById("string_defaultVal" + i).disabled = true;
                    setDisable("string_id" + i);
                    setDisable("string_pin" + i);
                    var id_my = [];
                    for (i1 = 0; i1 < jsonStrIR.name.length; i1++) {
                        id_my[i1] = i1;
                    }
                    value2 = makeinOption(id_my, jsonStrIR.name.indexOf(getVal("string_descr" + i)));
                    setHTML("string_pin" + i, value2);
                    selectidIR(i);
                }
                break;

        }

    }
}

function setDisable(ID) {
    if (document.getElementById(ID)) {
        document.getElementById(ID).disabled = true;
    } else {
        if (document.getElementById("test")) {

        }
    }
}

function selectidIR(i) { //сделать выбор изjsonStr.pin в string_descr5
    index = 0;

    index = document.getElementById("string_descr" + i).selectedIndex;
    setVal("string_defaultVal" + i, jsonStrIR.code[index]);
    document.getElementById("string_pin" + i).selectedIndex = document.getElementById("string_descr" + i).selectedIndex;


}

function make_string_pin() {
    //getVal("val");
}

function setHTML(ID, value) {
    if (document.getElementById(ID)) {
        document.getElementById(ID).innerHTML = value; //range
    }
}

function getHTML(ID) {
    var value;
    if (document.getElementById(ID)) {
        value = document.getElementById(ID).innerHTML; //range
        return value;
    } else {
        if (document.getElementById("test")) {
            //document.getElementById("test").innerHTML += "<br>wrong_getHTML:'" + ID + "'"; //range
        }
    }
    return undefined;
}

function getVal(ID) {
    var value = -1;
    var object;
    if (document.getElementById(ID)) {
        object = document.getElementById(ID);
        if (object.type == "checkbox") {
            value = document.getElementById(ID).checked;
            //alert(value);
        } else {
            value = document.getElementById(ID).value; //range
        }
    }
    return value;
}

function setVal(ID, value) {
    var object;
    if (document.getElementById(ID)) {
        object = document.getElementById(ID);
        if (object.type == "checkbox") {
            document.getElementById(ID).checked = value;
        }
        else if (object.type == "select-one") {
            object.value = value;
        } else {
            document.getElementById(ID).value = value;
        }
    } else {
        return "no " + ID;
    }
    return "ok";
}


function saveAllStrings(numberChosed) {
    //document.getElementById("test").innerHTML += numberChosed + "!!!!!!";
    for (var i = 0; i < numberChosed; i++) {
        if (document.getElementById("string_descr" + i)) {
        }
        if (document.getElementById("one_string" + i)) {
            one_string_saved[i] = document.getElementById("one_string" + i).value;
        }
        if (document.getElementById("string_page" + i)) {
            string_page_saved[i] = document.getElementById("string_page" + i).value;
        }
        if (document.getElementById("string_descr" + i)) {
            if (document.getElementById("string_descr" + i).value === "undefined") {
                //alert("ok");
                return;
            }
            string_descr_saved[i] = document.getElementById("string_descr" + i).value;

        } else {
            //string_descr_saved[i] = "button:" + i;
        }
        if (document.getElementById("string_widget" + i)) {
            string_widget_saved[i] = document.getElementById("string_widget" + i).value;
        } else {
            //string_widget_saved[i] = inputWidjet[2];
        }
        if (document.getElementById("string_pin" + i)) {
            string_pin_saved[i] = (document.getElementById("string_pin" + i).value);


        } else {
            // string_pin_saved[i] =inputPin[0];
        }
        if (document.getElementById("string_id" + i)) {
            string_id_saved[i] = document.getElementById("string_id" + i).value;
        }
        if (document.getElementById("string_defaultVal" + i)) {
            string_defaultVal_saved[i] = parseInt(document.getElementById("string_defaultVal" + i).value);
        } else {
            //string_defaultVal_saved[i] = "0";
        }

        string_delimeterVal_saved[i] = getVal("string_delimeterVal" + i);
        if (getVal("string_delimeterVal" + i) === -1) {
            string_delimeterVal_saved[i] = 0;
        }
        string_IR_saved[i] = getVal("string_IR" + i);

    }
    analogDivider = getVal("analogDivider");
    analogSubtracter = getVal("analogSubtracter");
    router = getVal("router");

}

function loadAllStrings(numberChosed) {
    var ok = true;
    if (ok) {
        for (var i = 0; i < numberChosed; i++) {
            if ((document.getElementById("one_string" + i))) {
                if (one_string_saved[i] === undefined) {
                    return;
                }

                document.getElementById("one_string" + i).value = one_string_saved[i];
            }
            if ((document.getElementById("string_page" + i))) {
                document.getElementById("string_page" + i).value = string_page_saved[i];
            }
            if ((document.getElementById("string_descr" + i))) {
                document.getElementById("string_descr" + i).value = string_descr_saved[i];
            }
            if ((document.getElementById("string_widget" + i))) {
                document.getElementById("string_widget" + i).value = string_widget_saved[i];
            }
            if ((document.getElementById("string_pin" + i))) {
                document.getElementById("string_pin" + i).value = string_pin_saved[i];
            }
            if ((document.getElementById("string_id" + i))) {
                //document.getElementById("string_id" + i).value = string_id_saved[i];
            }
            if ((document.getElementById("string_defaultVal" + i))) {
                // alert("ok");
                document.getElementById("string_defaultVal" + i).value = string_defaultVal_saved[i];
            }
            setVal("string_delimeterVal" + i, string_delimeterVal_saved[i]);
            setVal("string_IR" + i, string_IR_saved[i]);

        }
    }
    setVal("analogDivider", analogDivider);
    setVal("analogSubtracter", analogSubtracter);
    setVal("router", router);
}

function makeinOption(inputOption, choosed) {
    var options = "";
    var i1;
    var selected = "";
    for (i1 = -1; i1 < inputOption.length; i1++) {
        if (i1 == choosed) {
            selected = " selected";
        } else {
            selected = "";
        }
        if (inputOption[i1] != null) {
            options += "<option" + selected + ">" + inputOption[i1] + "</option>";
        }
    }
    return options;
}
