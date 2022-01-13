var http = require('http');
var fs = require("fs");
const mDnsSd = require('node-dns-sd');

var path = "../.pio/build/esp12e/firmware.bin";
var url = "/ledStrip/update"

var PORT = 8266; //Set whatever port you want to listen on
var ip = require("ip");
var myAdrr = ip.address();
//server

console.log("Starting Nodejs ESP8266 OTA Update Server.");
console.log("Listening on " + myAdrr + ":" + PORT.toString() + url);

var server = http.createServer(function (request, response) {

  if (request.url === url) {
    user_agent = request.headers['user-agent']
    user_ip = request.connection.remoteAddress;
    console.log('\b\b' + "User-Agent:" + user_agent);
    console.log('\b\b' + "User-ip:" + user_ip);
    path_to_file = path;
    fs.readFile(path_to_file, "binary", function (err, file) {
      if (err) {
        response.writeHead(500, {
          "Content-Type": "text/plain"
        });
        response.write(err + "\n");
        response.end();
        console.log(err);
        return;
      } //End Error Code

      //Write file to resposne

      var stats = fs.statSync(path_to_file); //Get File Size
      response.setHeader("Content-Type", "text/html");
      response.setHeader("Content-Disposition", "attachment");
      response.setHeader("Content-Length", stats.size.toString());
      response.write(file, "binary");
      response.end();
      //server.close();
      console.log("Update successfull");
    });
  }
});

server.listen(PORT);

//console input
let promptly = require('promptly');

void async function () {
  let command = undefined;
  while (command != 'stop') {
    command = await promptly.choose('>', ['changepath', 'path', 'up', 'changeurl', 'url', 'stop']);
    switch (command) {
      //para cambiar la ruta del archivo
      case 'changepath': {
        path = await promptly.prompt('path=');
        break;
      }
      //ver la ruta del archivo
      case 'path': {
        console.log(path);
        break;
      }
      //mandar el comando de update y subir el codigo
      case 'up': {
        console.log("Searching ESP8266");

        mDnsSd.discover({
          name: '_esp._tcp.local'
        }).then((device_list) => {
          console.log("Has been found: " + device_list[0].fqdn.toString());
          console.log("Running on : " + device_list[0].address.toString());

          console.log("sending update post");

          var request = require('request')
          var JSONdata = JSON.stringify({
            update: 1,
            ip: myAdrr
          });
          var address = "http://"+device_list[0].address.toString()+"/settingsJSON";
          request.post({
            url: address,
            form: JSONdata
          });
        }).catch((error) => {
          console.error("Not found");
        });
      }
      break;
    case 'changeurl': {
      url = await promptly.prompt('url=');
      break;
    }
    case 'url': {
      console.log(url);
      break;
    }
    case 'stop': {
      server.close();
      break;
    }
    }
  }
}().catch((error) => {
  server.close();
});