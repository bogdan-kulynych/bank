Bank API Server
---------------


## Deploying


### Requirements

    node@0.10.x

### Configuring and building

1. [Build native layer](native/README.md) or get `bank.node` binaries
2. Configure SSL in `api/config/ssl.json`
3. Run the server specifying a port:

        npm install
        node server.json 4443


## Protocol


It's a RESTful API via HTTPS. Every client is required to have a key / certificate signed with Bank CA certificate. There are *sample SSL certificates* for testing purposes in `ssl` directory.


### Greeting


Request:

    GET /api
    Content-Type: *

Response:

    200 OK


### Authentication


In order to use an API, user has to obtain an authentication token, and supply it on every request that requires authorization. Token contains card id, issue time, and server's digital signature: `<card id>:<unix timestamp>:<base64-encoded digital signature>`. Token is valid within 3 minutes after being issued.


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


### Transfer


Request:

    POST /api/transfer?token=<API session token>
    Content-Type: application/json

    {
        "recepient": "<Recepient card id>",
        "amount": "<Amount to transfer>",
    }

Successful response:

    200 OK

Unauthorized error:

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
