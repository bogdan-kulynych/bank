Bank native layer
-----------------


### Prerequisites

Runtime dependencies:

    libssl-dev

Dev dependencies:

    npm install node-gyp -g

C++11-compatible compiler is needed.

### Building

Prior to building, secrets have to be changed in `config/config.h`. Then, to build with GYP:

    cd native
    node-gyp build

Built version will be in `native/build/Release`

