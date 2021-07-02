//var HelperLoaded=true;
function createXmlHttpObject() {
    var xmlHttp;
    if (window.XMLHttpRequest) {
        xmlHttp = new XMLHttpRequest();
    } else {
        xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');
    }
    return xmlHttp;
}
function saveData(filename, data,returnCallback) {
    var xmlHttp = createXmlHttpObject();
    var file = new Blob([JSON.stringify(data, null, 2)], {type: "text/plain;charset=utf-8"});
    var a = new FormData();
    a.append("data", file, filename);
    xmlHttp.open("POST", "/edit");
    xmlHttp.send(a);
    xmlHttp.onreadystatechange = function () {
        if (xmlHttp.readyState == 4) {
            if (xmlHttp.status != 200) {
                returnCallback("ERROR[" + xmlHttp.status + "]: " + xmlHttp.responseText)
            }
        else
            {
                returnCallback(xmlHttp.responseText);
            }
        }
    };
    xmlHttp.onloadend = function () {
        if(xmlHttp.status === 404){
           returnCallback("404");
        }
    };
    returnCallback(null);
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
        if(xmlHttp.status === 404){
            callback("404");
        }
    };


    xmlHttp.send(null);
}
function set_cookie(name, value, exp_y, exp_m, exp_d, path, domain, secure) {
    var cookie_string = name + "=" + escape(value);

    if (exp_y) {
        var expires = new Date(exp_y, exp_m, exp_d);
        cookie_string += "; expires=" + expires.toGMTString();
    }
    if (path)
        cookie_string += "; path=" + escape(path);
    if (domain)
        cookie_string += "; domain=" + escape(domain);
    if (secure)
        cookie_string += "; secure";
    document.cookie = cookie_string;
}
function get_cookie(cookie_name) {
    var results = document.cookie.match('(^|;) ?' + cookie_name + '=([^;]*)(;|$)');
    if (results)
        return ( unescape(results[2]) );
    else
        return null;
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
function loadjQuery(url, success) {
    var script = document.createElement('script');
    script.src = url;
    var head = document.getElementsByTagName('head')[0],
        done = false;
    head.appendChild(script);
    // Attach handlers for all browsers
    script.onload = script.onreadystatechange = function () {
        if (!done && (!this.readyState || this.readyState == 'loaded' || this.readyState == 'complete')) {
            done = true;
            success();
            script.onload = script.onreadystatechange = null;
            head.removeChild(script);
        }
    };
}
function loadBootstrap() {
    var element = document.body,
        style = window.getComputedStyle(element),
        top = style.getPropertyValue('color');
    if (top != 'rgb(51, 51, 51)') {
        var el = document.head,
            elChild = document.createElement('link');
        elChild.innerHTML = '<link sync rel="stylesheet" href="bootstrap.min.css">';
        el.insertBefore(elChild, el.firstChild);
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
function createTD(tr, text) {
    var td = document.createElement('td');
    if (typeof (text) === "object") {
        td.appendChild(text);
    } else if (typeof (text) === "string") {
        td.appendChild(document.createTextNode(text));
    }
    tr.appendChild(td);

}
function createTR(tr) {
    var Newtr = document.createElement('tr');
    //Newtr.id = "tr" + i;
    Newtr.appendChild(tr);
    return Newtr;
}