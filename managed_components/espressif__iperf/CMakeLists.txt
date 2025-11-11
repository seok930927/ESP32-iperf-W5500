include($ENV{IDF_PATH}/tools/cmake/version.cmake)

set(srcs  "iperf.c")

set(priv_requires freertos)

# version >= v4.4
if(${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR} VERSION_GREATER 4.3)
    list(APPEND priv_requires esp_timer)
endif()
idf_component_register(SRCS "${srcs}"
                            "../../port/ioLibrary_Driver/src/wizchip_spi.c"   
                            "../../lib/ioLibrary_Driver/Ethernet/wizchip_conf.c"
                            "../../lib/ioLibrary_Driver/Ethernet/socket.c"
                            "../../lib/ioLibrary_Driver/Ethernet/W5500/w5500.c"
                            "../../lib/ioLibrary_Driver/Ethernet/W6300/w6300.c"
                            "../../lib/iperf/iperf.c"
                            "../../lib/iperf/cJSON.c"
                    INCLUDE_DIRS "include"
                                    "../../lib/ioLibrary_Driver/Ethernet"
                                    "../../lib/ioLibrary_Driver/Ethernet/W5500"
                                    "../../lib/ioLibrary_Driver/Ethernet/W6300"
                                    "../../lib/ioLibrary_Driver/Internet"
                                    "../../lib/ioLibrary_Driver/Application"
                                    "../../lib/ioLibrary_Driver/Application/loopback"
                                    "../../port/ioLibrary_Driver/inc"
                                    "../../lib/iperf"
                    PRIV_REQUIRES ${priv_requires} driver)

if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 11.0)
    target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
endif()
