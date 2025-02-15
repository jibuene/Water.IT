cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_examples C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()


add_executable(picow_httpd_background
        main.c
        )
target_compile_definitions(picow_httpd_background PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        )
target_include_directories(picow_httpd_background PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
        ${PICO_LWIP_CONTRIB_PATH}/apps/httpd
        )
target_link_libraries(picow_httpd_background PRIVATE
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip_http
        pico_lwip_mdns
        pico_httpd_content
        pico_stdlib
        )
pico_add_extra_outputs(picow_httpd_background)

pico_add_library(pico_httpd_content NOFLAG)
pico_set_lwip_httpd_content(pico_httpd_content INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/content/404.html
        ${CMAKE_CURRENT_LIST_DIR}/content/index.shtml
        )