idf_component_register(SRCS "cmd_ethernet.c"
                            "ethernet_iperf_main.c"
                       PRIV_REQUIRES fatfs esp_netif
                       INCLUDE_DIRS "."
                              "../components/esp_eth/include"
                              "../components/esp_eth"
                       )
