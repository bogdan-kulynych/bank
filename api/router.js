/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Main API router
 */


var express = require('express');
//    bank = require('../lib/build/bank');


// OPTIONS


var api = module.exports = express();

api.use(express.json());
api.disable('x-powered-by');


// ROUTER


// Index
api.get('/api', function(req, res) {
    res.send(200);
});

// Authentication
api.get('/api/auth', function(req, res) {
/*
    var card = req.body['card'],
        pin = req.body['pin'];

    bank.requestAuthToken(card, pin, function(err, token) {
        if (err) {
            res.send(401);
        } else {
            res.send(JSON.stringify("token": token));
        }
    });
*/
    res.send(501);
});

// Balance
api.get('/api/balance', function(req, res) {
    res.send(501);
});
