{
    'make_global_settings': [
        ['CXX','/usr/bin/clang++'],
        ['LINK','/usr/bin/clang++'],
    ],

    "configurations": {
        "Debug": {
            "defines": [
                "NDEBUG"
            ]
        },
        "Release": {
        }
    },

    "targets": [

        {
            "target_name": "config",
            "type": "static_library",
            "sources": [
                "config/config.h"
            ],
            'direct_dependent_settings': {
                'include_dirs': [
                    'config/'
                ],
            },
        },

        {
            "target_name": "libbank",
            "type": "static_library",
            "sources": [
                "src/auth.h",
                "src/auth.cpp",
                "src/db.h",
                "src/db.cpp",
                "src/transactions.h",
                "src/transactions.cpp",
                "src/utils/base64.h",
                "src/utils/base64.cpp"
            ],
            "dependencies": [
                "config"
            ],
            "link_settings": {
                "libraries": [
                    "-lcrypto"
                ]
            },
            'include_dirs': [
                '..'
            ],
            'direct_dependent_settings': {
                'include_dirs': [
                    'src/'
                ],
            },
            'export_dependent_settings': [
                'config'
            ],
            "cflags": [
                "-std=c++11",
            ],
            'cflags!': ['-fno-exceptions'],
            'cflags_cc!': ['-fno-exceptions']
        },

        {
            "target_name": "demo",
            "type": "executable",
            "sources": [
                "src/main.cpp"
            ],
            "dependencies": [
                "libbank"
            ],
            "cflags": [
                "-std=c++11",
            ],
            'cflags!': ['-fno-exceptions'],
            'cflags_cc!': ['-fno-exceptions']
        },

        {
            "target_name": "bank",
            "sources": [
                "wrapper.cpp"
            ],
            "dependencies": [
                "libbank"
            ],
            "cflags": [
                "-std=c++11",
            ],
            'cflags!': ['-fno-exceptions'],
            'cflags_cc!': ['-fno-exceptions']
        }
    ]
}
