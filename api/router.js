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

function handleException(e, res) {
    if (e.code) {
        res.send(e.code, JSON.stringify({"message": e.message}));
    } else {
        res.send(400);
    }
};

// Index
api.get('/api', function(req, res) {
    res.send(200);
});

// Authentication
api.post('/api/auth', function(req, res) {
    var card = req.body["card"],
        pin = req.body["pin"];

    try {
        var token = bank.authTokenRequest(card, pin);
        if (token) {
            res.send(200, JSON.stringify({"token": token}));
        } else {
            res.send(500);
        }
    } catch (e) {
        handleException(e, res);
    }
});

// Balance
api.get('/api/balance', function(req, res) {
    var token = req.query.token;

    try {
        var balance = bank.balanceInquiry(token);
        if (!utils.emptyObject(balance)) {
            res.send(200, JSON.stringify(balance))
        } else {
            res.send(500);
        }
    } catch (e) {
        handleException(e, res);
    }
});


// Name
api.get('/api/name', function(req, res) {
    var token = req.query.token,
        card = req.query.card;

    try {
        var name = bank.nameRequest(token, card);
        res.send(200, JSON.stringify({"name": name}));
    } catch (e) {
        handleException(e, res);
    }
});


// Withdrawal
api.post('/api/withdraw', function(req, res) {
    var token = req.query.token,
        amount = req.body['amount'];

    try {
        bank.withdrawalRequest(token, amount);
        res.send(200);
    } catch (e) {
        handleException(e, res);
    }
});


// Transfer
api.post('/api/transfer', function(req, res) {
    var token = req.query.token,
        recepient_id = req.body['recepient'],
        amount = req.body['amount'];

    try {
        bank.transferRequest(token, recepient_id, amount);
        res.send(200);
    } catch (e) {
        handleException(e, res);
    }
});
