cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_examples C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


pico_sdk_init()

add_executable(picow_httpd_background
        main.c
        )

target_compile_definitions(picow_httpd_background PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        )

target_link_libraries(picow_httpd_background pico_stdlib 
      pico_cyw43_arch_lwip_threadsafe_background 
                                      pico_lwip_http)

target_include_directories(picow_httpd_background PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts and mbedtls_config
        )
pico_add_extra_outputs(picow_httpd_background)