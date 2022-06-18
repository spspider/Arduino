/**
 * Created by sergey on 02.04.2019.
 */
document.addEventListener("DOMContentLoaded", load);
var IRjson = {code: [], num: 0};
var progress;
var CODE = {};
//  var ModifiedJson;
function load() {
    //alert(typeof HelperLoaded);
    //IRjson = {code: [], name: [], rawID: [], rawCode: [], rawCodeLen: []};
    load2();
}
function load2() {


    readTextFile("setup_433.txt", function (text) {

        try {
            //IRjson={};
            //document.getElementById("test").innerHTML += text;
            IRjson = JSON.parse(text);
            //IRjson.code === undefined ? IRjson.code = [] : {};
            makeIRList(IRjson);
            //document.getElementById("codelist").innerHTML += IRjson.code[0];
        } catch (e) {
            makeIRList(IRjson);
            //document.getElementById("test").innerHTML += "JSON.parse: " + e;

        }
    });


}
function delCb(xmlHttp, a) {
    return function () {
        if (xmlHttp.readyState == 4) {
            if (xmlHttp.status != 200) {
                //alert("ERROR[" + xmlHttp.status + "]: " + xmlHttp.responseText)
            } else {
                //saveCode();
            }
        }
    }
}
function httpDelete(a) {
    var xmlHttp = createXmlHttpObject();
    xmlHttp.onreadystatechange = delCb(xmlHttp, a);
    var b = new FormData();
    b.append("path", a);
    xmlHttp.open("DELETE", "/edit");
    xmlHttp.send(b);
}
function deleteRow(value) {

    arr = IRjson.code;
    for( var i = 0; i < arr.length; i++){ 
    
        if ( arr[i] == value) { 
            //alert(arr[i]);
            IRjson.code.splice(i, 1); 
        }
    
    }
    makeIRList(IRjson);
    //httpDelete("/IrRaw_Code" + i + ".txt");
}

function makeIRList(IRjson) {
    //document.getElementById("table").innerHTML = "<tbody id='body_table'><tbody>";
    var result = "",
        table_res = "";
    table_res = "<tr>" +
        "<td>номер</td>" +
        "<td>код</td>" +
        "<td>имя</td>" +
        // "<td>rawID</td>" +
        "<td>удалить</td>" +
        "</tr>";

    if (IRjson) {
        if (IRjson.code) {
            //IRjson.name.splice(IRjson.code.length);

            for (i = (IRjson.code.length); i > 0; i--) {
                //setHTML("output",getHTML("output")+" "+i);
                table_res +=
                    "<tr id='number'>" +
                    "<td id='number'>" + i + "</td>" +
                    "<td id='code'>" + IRjson.code[i - 1] + "</td>" +
                    // "<td id='rawID'>" + IRjson.rawID[i] + "</td>" +
                    "<td id='del'><button class='form-control' onclick='deleteRow(" + IRjson.code[i - 1] + ")'>X</button></td>" +
                    "</tr>";
            }
        }
    }

    //table_res="<tr><td></td><td><input type='text' id='IRcode' class='form-control'></td><td><input type='text' id='IRcodуName' class='form-control'></td><td></td></tr>";
    document.getElementById("table").innerHTML = table_res;

    //document.getElementById("codelist").innerHTML = result;
}
function WaitIR(submit) {
    //progress = document.createElement("div");
    //progress.id = "progress_IR";
    //progress.className = "progress-bar progress-bar-striped active";
    //progress.innerHTML("class='progress-bar progress-bar-striped active'");
    //  my_div = document.getElementById("progress");
    // my_div.appendChild(progress);
    //progress.style.width = 100 + "%";
    // progress.style.role = "progressbar";

    setVal("IRcode", "");
    server = "/WaitIR";
    old_submit = submit.value;
    submit.value = "нажмите кнопку...";
    readTextFile(server, function (data) {
        if (data === null) {
            //progress.style.width = 0 + "%";
            //send_request(submit, server);
            //take_progress_zero(submit);
            //submit.value="Взять IR";
            submit.value = "no data:Взять IR";
        }
        else {
            sendReguestCode(data);
            submit.value = "OK!:Взять IR";
            //progress.style.width = 0 + "%";
            //take_progress_zero(submit);
        }
    });

    setTimeout(function () {
        take_progress_zero(submit);
        submit.value = "Взять IR";
        submit_disabled(false);

    }, 5000);
    //send_request(submit, server);
    //submit.value = 'нажмите кнопку...';
    //submit_disabled(true);
}

function AddNewButton() {
    var savedCode = getVal("IRcode");
    var NameIR = getVal("IRcodуName");
    if (IRjson === null) {
        IRjson = {code: []};
    }

    if ((savedCode.length < 20) && (savedCode.length > 1)) {
        if(IRjson.code.indexOf(savedCode)===-1) {
            IRjson.code.push(savedCode);
        }else{
            alert("код существует");
        }
    }

    //IRjson=JSON.stringify(IRjson);
    makeIRList(IRjson);
    //document.getElementById("demo").innerHTML += JSON.stringify(IRjson);
}


function saveCode() {
    var SendCodeJson = {};
    SendCodeJson.code = IRjson.code;
    SendCodeJson.num = IRjson.code.length;
    var json_upload = JSON.stringify(SendCodeJson);
    //json_upload.append("Number", btnId);
    setHTML("output", json_upload);
    saveData("setup_433.txt", SendCodeJson);
    /* xmlHttp.open("POST", '/SaveIR', true);
     xmlHttp.setRequestHeader('Content-Type', 'application/json; charset=UTF-8');
     xmlHttp.send(json_upload);
     xmlHttp.onloadend = function () {
     //document.getElementById("test").innerHTML += JSON.stringify(IRjson);
     };
     */
}


function test() {
    sendReguestCode(JSON.stringify({
        "raw": "true",
        "len": 200,
        "c": [1, 2218, 2174, 266, 824, 280, 267, 280, 811, 279, 813, 277, 268, 266, 283, 278, 813, 264, 283, 280, 268, 277, 813, 266, 281, 264, 283, 266, 824, 280, 811, 279, 268, 280, 814, 278, 267, 266, 824, 266, 826, 264, 826, 280, 811, 266, 281, 280, 813, 277, 813, 279, 811, 280, 268, 279, 268, 280, 267, 280, 268, 279, 811, 266, 281, 280, 270, 280, 810, 278, 813, 277, 813, 266, 281, 266, 281, 280, 267, 266, 281, 266, 283, 277, 268, 280, 267, 280, 267, 266, 826, 264, 826, 266, 824, 280, 813, 278, 810, 280, 2601, 2190, 2192, 264, 826, 264, 283, 264, 825, 278, 815, 250, 295, 266, 283, 264, 826, 264, 286, 264, 281, 266, 838, 252, 281, 266, 283, 264, 826, 264, 826, 264, 283, 264, 829, 264, 281, 264, 826, 280, 813, 264, 826, 264, 827, 264, 283, 264, 826, 266, 840, 250, 826, 264, 283, 264, 283, 264, 283, 264, 283, 264, 826, 266, 294, 253, 283, 264, 826, 264, 840, 251, 826, 266, 281, 266, 294, 253, 283, 264, 281, 266, 283, 264, 283, 264, 283, 264, 283, 264, 826, 264, 826, 264, 826, 264, 828, 264, 826, 280, 0]
    }));
    // sendReguestCode("code");

}
function sendReguestCode(code) {
    CODE = code;
    setVal("IRcode", code);
}

/////////////////////////////helper
function createXmlHttpObject() {
    var xmlHttp;
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
    xmlHttp.timeout = 1000;
    xmlHttp.ontimeout = function () {
        callback("no file");
    }
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
    xmlHttp.onloadend = function () {
        if (xmlHttp.status === 404) {
            callback("404");
        }
    };


    xmlHttp.send(null);
}
function saveData(filename, data) {

    var xmlHttp = createXmlHttpObject();

    var file = new Blob([JSON.stringify(data)], {type: "text/plain;charset=utf-8"});
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
function setVal(ID, value) {
    var object;
    if (document.getElementById(ID)) {
        object = document.getElementById(ID);
        if (object.type == "checkbox") {
            document.getElementById(ID).checked = value;
        } else {
            document.getElementById(ID).value = value;
            //document.getElementById(ID).innerHTML = value;
        }
    } else {
        if (document.getElementById("test")) {
            //document.getElementById("test").innerHTML += "<br>wrong_setVal:'" + ID + "' value:" + value; //range
        }
    }
}
function setHTML(ID, value) {
    if (document.getElementById(ID)) {
        document.getElementById(ID).innerHTML = value; //range
    }
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
