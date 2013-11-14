include_directories(../config)

add_library (bank STATIC
    auth.cpp
    utils/base64.cpp)

target_link_libraries (bank config)

find_package (OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
    target_link_libraries (bank ${OPENSSL_LIBRARIES})
endif (OPENSSL_FOUND)

add_executable (demo main.cpp)
target_link_libraries (demo bank)
