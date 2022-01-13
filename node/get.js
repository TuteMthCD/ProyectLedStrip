var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
var url = "http://esp8266.local/settingsJSON";
var Http = new XMLHttpRequest();
Http.open("GET", url);
Http.send(null);

Http.onreadystatechange = (e) => {
  if (Http.readyState == 4 && Http.status == 200) {
    var EspSettings = JSON.parse(Http.responseText);
    console.log(EspSettings);
    //console.log();
    //console.log(Http.responseText);
  }
}
