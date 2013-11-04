Bank API Server
---------------


## Running


Requires `node.js v0.10.x`.

Running locally on port `4443`:

    npm install
    node server.json 4443


## Protocol


Every client is required to have a key / certificate signed with Bank CA certificate.

Protocol is via HTTPS RESTful API.


### Greetings


Request:

    GET /api
    Content-Type: *

Response:

    200 OK


### Authentication


Request:

    GET /api/auth
    Content-Type: application/json

    {
        "card": "<card id>",
        "pin" : "<PIN>"
    }

Successful response:

    200 OK
    Content-Type: application/json

    {
        "token": "<API session token>"
    }

Unauthorized error

    401 Unauthorized


### Balance inquiry


Request:

    GET /api/balance
    Content-Type: application/json

    {
        "token": "<API session token>"
    }

Successful response:

    200 OK
    Content-Type: application/json

    {
        "available": "<available funds>",
        "hold": "<funds on hold>"
    }

Unauthorized error:

    401 Unauthorized


### Money withdrawal


Request:

    POST /api/withdraw
    Content-Type: application/json

    {
        "token": "<API session token>",
        "amount": <Amount to withdraw>
    }

Successful response:

    200 OK

Unauthorized error:

    401 Unauthorized

Not sufficient funds:

    403 Forbidden

