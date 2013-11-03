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

    POST /api/auth
    Content-Type: application/json

    {
        "card": "<card id>",
        "pin" : "<PIN>"
    }

Successful response:

    200 OK
    Content-Type: application/json

    {
        "token": "<unique API token>"
    }

Failed response:

    401 Unauthorized


### Actions

    GET /api/balance
    Authentication: <unique API token>
