/**
 * Bank subsystem
 * Author: Bogdan Kulynych
 *
 * Main API router
 */


var express = require('express'),

    utils = require('./utils'),
    bank = require('../native/build/Release/bank.node');


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
api.post('/api/auth', function(req, res) {
    var card = req.body["card"],
        pin = req.body["pin"];

    var token;
    try {
        token = bank.requestAuthToken(card, pin);
    } catch (e) {
        res.send(400);
    }

    if (!token) {
        res.send(401);
    } else {
        res.send(200, JSON.stringify({"token": token}));
    }
});

// Balance
api.get('/api/balance', function(req, res) {
    var token = req.query.token;

    var balance;
    try {
        balance = bank.balanceInquiry(token);
    } catch (e) {
        console.warn(e);
        res.send(400);
    }

    if (utils.emptyObject(balance)) {
        res.send(401);
    } else {
        res.send(200, JSON.stringify(balance))
    }
});
