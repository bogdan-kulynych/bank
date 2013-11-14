/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Server bootstraping
 */


var https = require('https'),

    api   = require('./api/router.js'),
    utils = require('./api/utils.js'),

    SSL_CONFIG_FILE = require('./api/config/ssl.json');


// CONFIGURATION


SSL_CONFIG = utils.processSslConfig(SSL_CONFIG_FILE);

var options = {
    'port': 4443
};


// HTTPS SERVER


var server = https.createServer(SSL_CONFIG, api)
    .listen(options.port, function() {
        console.log('Server is up and running on port ' + options.port);
     });

// Request logging
server.on('request', function(req) {
    console.log('[' + utils.getDateTime() + '] Incoming request:',
        req.ip, req.method, req.url);
});
