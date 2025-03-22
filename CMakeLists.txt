cmake_minimum_required(VERSION 3.12)

set(PICO_BOARD pico_w)
SET(ProjectName Samisensor)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${ProjectName} C CXX ASM)
pico_sdk_init()

include_directories($ENV{Path})

add_executable(${ProjectName}
        main.cpp
        cJSON.c
        cJSON.h
)

target_include_directories(${ProjectName} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)


target_link_libraries(${ProjectName}
        pico_stdlib
        hardware_i2c
)

target_compile_definitions(${ProjectName} PRIVATE
        #WIFI_SSID=\"$ENV{WIFI_SSID}\"
        #WIFI_PASSWORD=\"$ENV{WIFI_PASSWORD}\"
        NO_SYS=0            # don't want NO_SYS (generally this would be in your lwipopts.h)
        PICO_CYW43_ARCH_DEFAULT_COUNTRY_CODE=CYW43_COUNTRY_FINLAND
)

set_source_files_properties(
        ${PICO_LWIP_PATH}/src/apps/altcp_tls/altcp_tls_mbedtls.c
        PROPERTIES
        COMPILE_OPTIONS "-Wno-unused-result"
)

pico_add_extra_outputs(${ProjectName})

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
