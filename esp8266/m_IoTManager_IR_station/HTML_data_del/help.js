/**
 * Created by Башня1 on 17.02.2018.
 */
document.addEventListener("DOMContentLoaded", loadPinSetup);
var PinSetup = {};
var dataOther = {};
var Conditions = [{}];
var MAX_COND_NUMBER = 4;
function loadPinSetup() {
    readTextFile("pin_setup.txt", PinSetupLoaded);
}
function PinSetupLoaded(data) {
    if (data == null) {
        setHTML("bodyNode", "не удалось загрузить настройки пинов");
        return;
    }
    readTextFile("other_setup.txt", other_setupLoaded);
    PinSetup = JSON.parse(data);
}

function loadOtherHelp() {
    var bodyNode="";
    bodyNode+= window.location.host +"/WaitIR";
        setHTML("bodyNodeIR", bodyNode);
    var bodyNode="";
    bodyNode+= window.location.host +"/sendAJAX?json={t:0,v:0}";
    setHTML("bodyNodeStat", bodyNode);
    var bodyNode="";
    bodyNode+= window.location.host +"/sendEmail?Email=текст сообщения";
    setHTML("bodyNodeEmail", bodyNode);
}

function other_setupLoaded(data) {
    if (data == null) {
        setHTML("bodyNode", "не удалось загрузить другие настройки");
        return;
    }
    dataOther = JSON.parse(data);
    loadBody();
    loadConditons(0);
    loadOtherHelp();
}
function loadConditons(id) {
    if (id < MAX_COND_NUMBER) {
        readTextFile("Condition" + id + ".txt", function (data) {
            if (data == null) {
                loadConditons(id + 1);
                return;
            }
            Conditions[id] = JSON.parse(data);
            loadConditons(id + 1);
        });
    }
    else {
        setHTMLControlConditions();
    }
}
function setHTMLControlConditions() {
    var count = 0;
    var adress = [];
    for (condID = 0; condID < MAX_COND_NUMBER; condID++) {
        if (Conditions[condID] !== undefined) {
            for (i = 0; i < 10; i++) {
                if (Conditions[condID].bySignal !== undefined) {
                    if ((parseInt(Conditions[condID].bySignal[i]) == 2) || (parseInt(Conditions[condID].bySignal[i]) == 3)) {
                        adress[adress.length] = i;
                        //count++;
                    }
                }
            }
        }
    }

    var bodyNode = "";
    bodyNode +=
        "<table class='table'>" +
        "<tr><td>" +
        "MQTT топик: " +
        "</td><td>" +
        "MQTT топик управление: " +
        "</td><td>" +
        "удаленное управление HTTP: " +
        "</td></tr>";
    for (condID = 0; condID < MAX_COND_NUMBER; condID++) {
        if (Conditions[condID] !== undefined) {
            for (i = 0; i < 10; i++) {
                if (Conditions[condID].bySignal !== undefined) {
                    if ((parseInt(Conditions[condID].bySignal[i]) == 2) || (parseInt(Conditions[condID].bySignal[i]) == 3)) {
                        MQTT_topic = dataOther.deviceID + "/PLUS/" + condID + "/" + i + "\n";
                        MQTT_control = dataOther.deviceID + "/PLUS/" + condID + "/" + i + "/" + "status" + "\n";
                        MQTT_json = window.location.host + "/aRest?Json={C:1,pin:" + condID + ",v3:" + i + ",val:" + Conditions[condID].bySignalPWM[i] + "}\n";
                        bodyNode +=
                            "<tr><td><code>" +
                            MQTT_topic +
                            "</code></td><td>" +
                            MQTT_control +
                            "</td><td><code>" +
                            MQTT_json +
                            "</code></td></tr>";
                    }
                }
            }
        }
    }

    bodyNode += "</table>";
    setHTML("bodyNodeCond", bodyNode);
}
function loadBody() {
    var bodyNode = "";
    bodyNode +=
        "<table class='table'>" +
        "<tr><td>" +
        "MQTT топик: " +
        "</td><td>" +
        "MQTT топик управление: " +
        "</td><td>" +
        "удаленное управление HTTP: " +
        "</td></tr>";
    for (i = 0; i < PinSetup.numberChosed; i++) {
        MQTT_topic = dataOther.deviceID + "/" + PinSetup.descr[i] + "/" + i + "\n";
        MQTT_control = dataOther.deviceID + "/" + PinSetup.descr[i] + "/" + i + "/" + "status" + "\n";
        MQTT_json = window.location.host + "/aRest?Json={pin:" + PinSetup.pin[i] + ",val:1}";
        bodyNode +=
            "<tr><td><code>" +
            MQTT_topic +
            "</code></td><td>" +
            MQTT_control +
            "</td><td><code>" +
            MQTT_json +
            "</code></td></tr>";
    }
    bodyNode += "</table>";
    setHTML("bodyNode", bodyNode);
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
                callback(null);
            }
        }
    }
    xmlHttp.send(null);
}
function setHTML(ID, value) {
    if (document.getElementById(ID)) {
        document.getElementById(ID).innerHTML = value; //range
    }
}