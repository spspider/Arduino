var xmlHttp;
function createXmlHttpObject() {
    if (window.XMLHttpRequest) {
        xmlHttp = new XMLHttpRequest();
    } else {
        xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');
    }
    return xmlHttp;
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
        }else{
            //callback(null);
        }

    }


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
        }
    } else {
        if (document.getElementById("test")) {
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
function loadBootstrap () {
    var element = document.body,
        style = window.getComputedStyle(element),
        top = style.getPropertyValue('color');
    if (top!='rgb(51, 51, 51)'){
        var el = document.head,
            elChild = document.createElement('link');
        elChild.innerHTML = '<link sync rel="stylesheet" href="bootstrap.min.css">';
        el.insertBefore(elChild, el.firstChild);
    }
}