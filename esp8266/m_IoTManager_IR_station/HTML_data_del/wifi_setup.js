/**
 * Created by sergey on 19.09.2017.
 */
document.addEventListener("DOMContentLoaded", function () {
    //load();
    var data = {
        ssid: "Home",
        softAP_ssid: "dev01-kitchen",
        "n": 10,
        WiFisoftAPIP: "0.0.0.0",
        ssid: "Home",
        WiFilocalIP: "192.168.1.108",
        "scan": ["Fregat-74", "misha_home", "topol_3_1", "netis_F20B65", "Camel", "Home", "Evil", "TP-LINK_E1DEA0", "Link", "@HomeF328", "rockrobo-vacuum-v1_miap6DFF", "Vlados", "}|{eKa", "My netwok hamster", "kotunhome", "netis_91", "Tenda_2F4018", "Julia77"],
        enc: ["*", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""],
        RSSI: [-85, -60, -89, -94, -90, -29, -93, -89, -91, -92, -80, -91, -87, -88, -79, -89, -90, -87]
    };


    send_request("wifiList", callbackWifiList);
});
function callbackWifiList(respnose) {

    //data2 = JSON.parse(respnose);
    setHTML("output", respnose);

    try {
        var data = JSON.parse(respnose);
        createBodyHead(data);
        tableCreate(data);
    }catch (e){
        setHTML("input",e);
        setHTML("output",respnose);
    }


}
function createBodyHead(data) {
    var body = document.getElementById('connectis');
    var newLi = document.createElement('li');
    newLi.className = "list-group-item";
    newLi.innerHTML = "Вы подключены к точке досупа: " + data.ssid;
    body.appendChild(newLi);
    var newLi = document.createElement('li');
    newLi.className = "list-group-item";
    newLi.innerHTML = "Локальный IP:" + data.WiFilocalIP;
    body.appendChild(newLi);
    var newLi = document.createElement('li');
    newLi.className = "list-group-item";
    newLi.innerHTML = "Программная точка доступа: " + data.softAP;
    body.appendChild(newLi);
    var newLi = document.createElement('li');
    newLi.className = "list-group-item";
    newLi.innerHTML = "Программный IP точки доступа: " + data.WiFisoftAPIP;
    body.appendChild(newLi);


}
function tableCreate(data) {

    var body = document.getElementById('table_div');
    var tbl = document.createElement('table');
    //tbl.style.width = '100%';
    tbl.setAttribute('border', '1');
    var tbdy = document.createElement('tbody');
    var tr = document.createElement('tr');
    tbl.className = "table";
    var td = document.createElement('td');


    createTD(tr, "WiFi SSID");
    createTD(tr, "ENC");
    createTD(tr, "Level");
    tbdy.appendChild(tr);


    for (var i = 0; i < data.n; i++) {
        var tr = document.createElement('tr');
        //for (var j = 0; j < 3; j++) {
        var a = document.createElement('a');
        var linkText = document.createTextNode(data.scan[i]);
        a.appendChild(linkText);
        a.title = data.scan[i];
        a.href = "#";
        var text = data.enc[i];
        a.onclick = function (text) {
            setVal("ssid", this.title);
        }
        createTDLink(tr, a);
        createTD(tr, data.enc[i]);
        createTD(tr, data.RSSI[i]);
        tbdy.appendChild(tr);
    }
    tbl.appendChild(tbdy);
    body.appendChild(tbl)
}
function createTD(tr, text) {
    var td = document.createElement('td');
    td.appendChild(document.createTextNode(text));
    tr.appendChild(td);

}
function createTDLink(tr, append) {
    var td = document.createElement('td');
    td.appendChild(append);
    tr.appendChild(td);

}

function createXmlHttpObject() {
    if (window.XMLHttpRequest) {
        xmlHttp = new XMLHttpRequest();
    } else {
        xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');
    }
    return xmlHttp;
}
function send_request(server, callback) {
    request = createXmlHttpObject();
    request.open("GET", server, true);
    request.send();
    request.timeout = 5000;
    request.onreadystatechange = function () {
        if (xmlHttp.readyState === 4) {
            if (xmlHttp.status == "200") {
                callback(xmlHttp.responseText);
            }
        }
    }
}
function setHTML(ID, value) {
    if (document.getElementById(ID)) {
        document.getElementById(ID).innerHTML = value; //range
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
        }
    }
}