Bank native layer
-----------------


### Prerequisites

C++ dependencies:

    libssl-dev

Global node.js dependencies:

    node-gyp

C++11-compatible compiler is needed.

### Building

Prior to building, secrets have to be changed in `config/config.h`. Then, to build with GYP run:

    cd /path/to/server
    bin/build

Built version will be in `native/build/Release`

