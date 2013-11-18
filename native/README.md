Bank native layer
-----------------


## Building

### Prerequisites

Clang 3.2 is used to compile library. To use another compiler, change `binding.gyp` `varibles` section appropriately. C++ dependencies:

    libssl-dev

Global node.js dependencies:

    node-gyp


### Configuring and building

1. Change secrets in `config/config.h`
2. Build library with GYP:

        cd /path/to/server
        bin/build

Built version will be in `build/Release`
