var fs      = require('fs'),
    https   = require('https'),
    express = require('express'),

    config  = require('./config.json');

// Inferring options from config file
var options = config;
['key', 'cert', 'ca'].map(function(name) {
    options[name] = fs.readFileSync(config[name]);
});

// Setting api server
var api = module.exports = express();
api.get('/', function(req, res) {
    console.log('Incoming request');
});

https.createServer(options, api)
     .listen(options.port, function() {
        console.log('Server is up and running on port ' + options.port);
     });
