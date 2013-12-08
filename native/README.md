Bank native layer
-----------------


## Building

### Prerequisites

Clang 3.2 is used to compile library. To use another compiler, change `binding.gyp` `varibles` section appropriately. C++ dependencies:

    libssl-dev
    libsqlite3-dev

Global node.js dependencies:

    node-gyp


### Configuring and building

1. Change parameters in `config/config.h`
3. Build library with GYP:

        cd /path/to/server
        bin/build

        # Or

        cd /path/to/server/native
		node-gyp build

        # Debug

		cd /path/to/server/native
		node-gyp build --debug

Built version will be in `native/build/Release`
