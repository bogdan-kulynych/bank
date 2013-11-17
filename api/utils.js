/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Utilities
 */


var fs = require('fs');

function emptyObject(obj) {
    for (var prop in obj) {
        return false;
    }
    return true;
}

function processSslConfig(config) {
    // SSL Keys and Certificates
    ['key', 'cert', 'ca'].map(function(name) {
        if (name in config) {
            config[name] = fs.readFileSync(config[name]);
        }
    });

    return config;
};

function getDateTime() {
    var date = new Date();
    var hour = date.getHours();
    hour = (hour < 10 ? "0" : "") + hour;
    var min  = date.getMinutes();
    min = (min < 10 ? "0" : "") + min;
    var sec  = date.getSeconds();
    sec = (sec < 10 ? "0" : "") + sec;
    var year = date.getFullYear();
    var month = date.getMonth() + 1;
    month = (month < 10 ? "0" : "") + month;
    var day  = date.getDate();
    day = (day < 10 ? "0" : "") + day;
    return year + ":" + month + ":" + day + ":" + hour + ":" + min + ":" + sec;
};

module.exports = {
    'emptyObject': emptyObject,
    'processSslConfig': processSslConfig,
    'getDateTime': getDateTime
};
