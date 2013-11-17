Bank native layer
-----------------


### Prerequisites

Runtime dependencies:

    libssl-dev

Dev dependencies:

    npm install node-gyp -g

C++11-compatible compiler is needed.

### Building

    cd native
    node-gyp build

Built version will be in `native/build/Release`

