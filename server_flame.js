//---------------------------------------------------------------------------------------------
var fs = require('fs');                     // create a module to read of upload file to server
var url = require('url');                   // write a module to parse the URL
var http = require('http');                 // import the HTTP module to transfer data
var querystring = require('querystring');   // module to transfer string to object
var db = [];                                // array to save data of temperature and humidity
//---------------------------------------------------------------------------------------------

// create a function to send response from server or recieve request from client
function requestHandler(request, response) {

    // update data to address 192.168.56.1
    var uriData = url.parse(request.url);
    var pathname = uriData.pathname;
    var query = uriData.query;
    var queryData = querystring.parse(query);
    //-----------------------------------------------------------------------------------------
    if (pathname == '/update') {            // update new data and send to client
        var newData = {
            warn: queryData.warn,
            time: new Date()                // get current time
        };
        db.push(newData);                   // push data to array
        console.log(newData);               // show data in log
        response.end();                     // get HTTP code
    //-----------------------------------------------------------------------------------------
    } else if (pathname == '/get') {        // get data by JSON format
        response.writeHead(200, {
            'Content-Type': 'application/json'
        });
        response.end(JSON.stringify(db));
        db = [];
    //-----------------------------------------------------------------------------------------
    } else {
        fs.readFile('./flame_detection.html', function(error, content) { //read file html
            response.writeHead(200, {
                'Content-Type': 'text/html'
            });
            response.end(content);
        });
    }
    //-----------------------------------------------------------------------------------------
}
var server = http.createServer(requestHandler); // create http server
server.listen(8000);                            // open port 8000
console.log('Server listening on port 8000');
