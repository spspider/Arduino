/**
 * Created by sergey on 02.04.2019.
 */
var set_real_time;
var xmlHttp = createXmlHttpObject();

//////////for-json

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


document.addEventListener("DOMContentLoaded", function () {

    //loadAuthentication(); //включить это
load();
});

/*
 function onLoad() {
 var form = document.getElementById("form");
 var output = document.getElementById("output");
 form.addEventListener("submit", function (e) {
 e.preventDefault();
 var json = "jsonArray="+toJSONString(this);
 output.innerHTML = json;
 xmlHttp.open("POST", '/other_setup', true);
 xmlHttp.setRequestHeader('Content-Type', 'application/json; charset=UTF-8');
 xmlHttp.send(json_upload);
 alert(json);
 }, false);
 }
 */
function CatchForm() {
    var form = document.getElementById("form");
    // var output = document.getElementById("output");
    var JsonString = toJSONString(form);

    var JsonStringParse = JSON.parse(JsonString);
    JsonStringParse.open_type = select_option.indexOf(getVal("select_type_open_id"));

    var json = "jsonArray=" + JSON.stringify(JsonStringParse, null, 2);
    // output.innerHTML = json;
    setHTML("output", json);

    saveData("setup_lock.txt", JsonStringParse);
    /*
     xmlHttp.open("POST", '/other_setup', true);
     xmlHttp.setRequestHeader('Content-Type', 'application/json; charset=UTF-8');
     xmlHttp.send(json);

     xmlHttp.onloadend = function () {
     setHTML("output", json);
     };
     xmlHttp.onreadystatechange = function () {
     if (xmlHttp.readyState === 4 && xmlHttp.status == "200") {
     //callback(xmlHttp.responseText);
     alert(xmlHttp.responseText);
     }
     }
     */
    //alert(json);
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
                // alert(xmlHttp.responseText);

            }
        }


    }
    xmlHttp.onloadend = function () {

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

function loadAuthentication() {
    readTextFile("/function?json={Authentication:0}", function (callback) {
        if (parseInt(callback) === 1) {
            load();
        } else {
            window.location.href = "/setup_lock";
            //load();
            //readTextFile("/function?json={Authentication:1}", function (callback) {
            //})
        }
    });

}

function setTextVisible() {
    var container = document.getElementById("container");
    container.style = "display: unset";
    var loading = document.getElementById("loading");
    loading.style = "display: none";
}

function load() {

    try {
        readTextFile("setup_lock.txt", function (settings) {
            try {
                setHTML("input", getHTML("input") + settings);

                //jsonResponse = jsonResponse !== "" ? $.parseJSON(settings) : {};
                sonResponse = JSON.parse(settings);
                loadBlock(settings);
                setTextVisible();
            } catch (e) {
                setHTML("input", getHTML("input") + e);
            }
        });
    } catch (e) {
        setHTML("input", getHTML("input") + e);
    }

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
        } else {
            //callback(null);
        }
    }
    xmlHttp.send(null);
}

/*
 function load2() {
 if (xmlHttp.readyState == 0 || xmlHttp.readyState == 4) {
 xmlHttp.open('PUT', 'settings.txt', true);

 xmlHttp.send(null);
 xmlHttp.onload = function (e) {
 jsonResponse = JSON.parse(xmlHttp.responseText);
 loadBlock();

 }
 }
 }
 */
function TestIOT(button) {

}

var select_option = ["по кнопке", "mac адресу", "кнопка и адрес"];

function make_additional_fields() {
    if (!document.getElementById("select_type_open_id")) {

        var select_type_open = document.getElementById("select_type_open");
        select_type_open.appendChild(makeinOption_child(select_option, "select_type_open_id", false));
    }
}

function loadBlock(settings) {
    make_additional_fields();


    data2 = JSON.parse(settings);

    data = document.getElementsByTagName('body')[0].innerHTML;
    var new_string;
    for (var key in data2) {
        new_string = data.replace(new RegExp('{{' + key + '}}', 'g'), data2[key]);
        data = new_string;
    }
    document.getElementsByTagName('body')[0].innerHTML = new_string;

    setVal("select_type_open_id", select_option[data2.open_type]);
    //handleServerResponse();
    //onLoad();
}

function submit_disabled(request) {
    var inputs = document.getElementsByTagName("input");
    for (var i = 0; i < inputs.length; i++) {
        if (inputs[i].type === 'submit') {
            inputs[i].disabled = request;
        }
    }
}

function setHTML(ID, value) {
    if (document.getElementById(ID)) {
        document.getElementById(ID).innerHTML = value; //range
    } else {
        if (document.getElementById("test")) {
            document.getElementById("test").innerHTML = "wrong_setHTML:'" + ID + "' value:" + value; //range
        }
    }
}

function getHTML(ID) {
    var value;
    if (document.getElementById(ID)) {
        value = document.getElementById(ID).innerHTML; //range
        return value;
    } else {
        if (document.getElementById("test")) {
            document.getElementById("test").innerHTML = "wrong_getHTML:'" + ID + "'"; //range
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
    } else {
        if (document.getElementById("test")) {
            //  document.getElementById("test").innerHTML += "<br>wrong:'" + ID + "'"; //range
        }
    }
    return value;
}
function makeinOption_child(inputOption, id, onChange) {
    var array = inputOption;
    var selectList = document.createElement("select");
    selectList.className = 'form-control';
    selectList.id = id;
    selectList.onchange = onChange;

    for (var i = 0; i < array.length; i++) {
        var option = document.createElement("option");
        option.value = array[i];
        option.text = array[i];
        selectList.appendChild(option);
    }
    return selectList;
}
function setVal(ID, value) {
    var object;

    if (value !== undefined) {
        //alert(value);
        if (document.getElementById(ID)) {
            object = document.getElementById(ID);
            if (object.type == "checkbox") {
                document.getElementById(ID).checked = value;
            } else {
                document.getElementById(ID).value = value;
            }
        } else {
            if (document.getElementById("test")) {
                //document.getElementById("test").innerHTML += "<br>wrong_setVal:'" + ID + "' value:" + value; //range
            }
        }
    }
}


