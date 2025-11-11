/*
 * SPDX-FileCopyrightText: 2018-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <string.h>
#include <inttypes.h>
#include "cmd_ethernet.h"
#include "esp_netif.h"
#include "esp_console.h"
#include "esp_eth.h"
#include "esp_mac.h"
#include "argtable3/argtable3.h"

#include "W5500/w5500.h"
#include "wizchip_conf.h"
#include "socket.h"

#include "wizchip_spi.h"


/* "ethernet" command */
static struct {
    struct arg_str *control;
    struct arg_end *end;
} eth_control_args;


static wiz_NetInfo g_net_info = {
    .mac = {0x00, 0x08, 0xDC, 0x12, 0x34, 0x56}, // MAC address
    .ip = {192, 168, 11, 2},                     // IP address
    .sn = {255, 255, 255, 0},                    // Subnet Mask
    .gw = {192, 168, 11, 1},                     // Gateway
    .dns = {8, 8, 8, 8},                         // DNS server
    .dhcp = NETINFO_STATIC
};


static int eth_cmd_control(int argc, char **argv)
{
    print_network_information(g_net_info);
  
    printf("will be Define \r\n") ; 
    return 0;
}

void register_ethernet_commands(void)
{
    eth_control_args.control = arg_str1(NULL, NULL, "<info>", "Get info of Ethernet");
    eth_control_args.end = arg_end(1);
    const esp_console_cmd_t cmd = {
        .command = "ethernet",
        .help = "Ethernet interface IO control",
        .hint = NULL,
        .func = eth_cmd_control,
        .argtable = &eth_control_args
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}
