Bank API Server
---------------


## Running


Requires `node.js v0.10.x`.
First, [build native layer](native/README.md). Then you can run server on port 4443:

    npm install
    node server.json 4443


## Protocol


Protocol is via HTTPS RESTful API.
Every client is required to have a key / certificate signed with Bank CA certificate. There are *Toy SSL certificates* for testing purposes in `ssl` directory.


### Greeting


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
        "token": "<API session token>"
    }

Unauthorized error

    401 Unauthorized

Malformed request

    400 Bad request


### Balance inquiry


Request:

    GET /api/balance?token=<API session token>
    Content-Type: application/json

Successful response:

    200 OK
    Content-Type: application/json

    {
        "available": "<available funds>",
        "hold": "<funds on hold>"
    }

Bad or stale token (re-authentication needed):

    401 Unauthorized


### Money withdrawal


Request:

    POST /api/withdraw?token=<API session token>
    Content-Type: application/json

    {
        "amount": <Amount to withdraw>
    }

Successful response:

    200 OK

Bad or stale token (re-authentication needed):

    401 Unauthorized

Not sufficient funds:

    403 Forbidden


### Periodic transfer


Request:

    POST /api/transfer?token=<API session token>
    Content-Type: application/json

    {
        "receiver": "<Reciever card id>",
        "amount": "<Amount to transfer>",
        "start_date": "<Start date>",
        "frequency": "weekly|monthly|quarterly|yearly"
    }

Successful response:

    200 OK

Unauthorized error:

    401 Unauthorized

Not sufficient funds:

    403 Forbidden
