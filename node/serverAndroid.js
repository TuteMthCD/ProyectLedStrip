const http = require('http');

http.createServer((request, response) => {
  let body = [];
  request.on('data', (chunk) => {
    body.push(chunk);
  }).on('end', () => {
    body = JSON.stringify({
      led_1: 0
    });
    response.end(body);
  });
}).listen(8080);
