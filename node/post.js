var request = require('request')
//var cookie = '**Here the cookie copied from the Network tab from the Chrome Dev Tools Bar**'
//var UA = '**Here the UA copied from the Network tab from the Chrome Dev Tools Bar**'

var JSONdata = JSON.stringify({
  led_1: 0
});

var url = "http://esp8266.local/settingsJSON";
//console.log(JSONdata);
//request.cookie(cookie)
request.post({
  url: url,
  //headers: {
  //  'User-Agent': "Yo";
  //},
  form: JSONdata
}, function(error, response, body) {
  console.log(response)
})
