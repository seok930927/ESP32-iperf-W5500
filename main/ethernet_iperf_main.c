/*
 * SPDX-FileCopyrightText: 2018-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <string.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_event.h"
#include "esp_vfs_fat.h"
#include "cmd_system.h"
#include "cmd_ethernet.h"

#include "W5500/w5500.h"
#include "wizchip_conf.h"
#include "socket.h"

#include "wizchip_spi.h"


#include "iperf_cmd.h"
static const char *TAG = "eth_example";

static esp_eth_handle_t *s_eth_handles = NULL;
static uint8_t s_eth_port_cnt = 0;

#if CONFIG_EXAMPLE_STORE_HISTORY

#define MOUNT_PATH "/data"
#define HISTORY_PATH MOUNT_PATH "/history.txt"

/* iperf */
static uint8_t g_iperf_buf[1024 * 16] = {
    0,
};
uint8_t dest_ip[4];
uint16_t destport;



static wiz_NetInfo g_net_info = {
    .mac = {0x00, 0x08, 0xDC, 0x12, 0x34, 0x56}, // MAC address
    .ip = {192, 168, 11, 2},                     // IP address
    .sn = {255, 255, 255, 0},                    // Subnet Mask
    .gw = {192, 168, 11, 1},                     // Gateway
    .dns = {8, 8, 8, 8},                         // DNS server
    .dhcp = NETINFO_STATIC
};

#define ETHERNET_BUF_MAX_SIZE (1024 * 16)
static uint8_t g_tcp_server_buf[ETHERNET_BUF_MAX_SIZE] = {
    0,
};



static void initialize_filesystem(void)
{
    static wl_handle_t wl_handle;
    const esp_vfs_fat_mount_config_t mount_config = {
        .max_files = 4,
        .format_if_mount_failed = true
    };
    esp_err_t err = esp_vfs_fat_spiflash_mount_rw_wl(MOUNT_PATH, "storage", &mount_config, &wl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return;
    }
}
#endif // CONFIG_EXAMPLE_STORE_HISTORY

void init_ethernet_and_netif(void)
{
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wizchip_reset();
    spi_init(); 
    vTaskDelay(pdMS_TO_TICKS(1000));
    wizchip_initialize();
    network_initialize(g_net_info);
    print_network_information(g_net_info);

}

void app_main(void)
{
    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
#if CONFIG_EXAMPLE_STORE_HISTORY
    initialize_filesystem();
    repl_config.history_save_path = HISTORY_PATH;
#endif
    repl_config.prompt = "iperf>";
    // init console REPL environment
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &repl));

    // init Ethernet and netif
    init_ethernet_and_netif();

    /* Register commands */
    register_system_common();
    app_register_iperf_commands();
    register_ethernet_commands();

    printf("\n =======================================================\n");
    printf(" |       Steps to Test Ethernet Bandwidth              |\n");
    printf(" |                                                     |\n");
    printf(" |  1. Enter 'help', check all supported commands      |\n");
    printf(" |  2. Wait ESP32 to get IP from DHCP                  |\n");
    printf(" |  3. Enter 'ethernet info', optional                 |\n");
    printf(" |  4. Server: 'iperf -u -s -i 3'                      |\n");
    printf(" |  5. Client: 'iperf -u -c SERVER_IP -t 60 -i 3'      |\n");
    printf(" |                                                     |\n");
    printf(" =======================================================\n\n");

    // start console REPL
    ESP_ERROR_CHECK(esp_console_start_repl(repl));
}
