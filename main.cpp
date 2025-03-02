#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/http_client.h"
#include <bits/stdc++.h>
#include <hardware/adc.h>

#define SENSOR_PIN 26

using namespace std;


void connect_to_wifi()
{
    uint32_t country = CYW43_COUNTRY_NORWAY;
    uint32_t auth = CYW43_AUTH_WPA2_MIXED_PSK;

    cyw43_arch_init_with_country(country);
    cyw43_arch_enable_sta_mode();
    cyw43_arch_wifi_connect_async(":)", ":)", auth);

    int flashrate = 1000;
    int status = CYW43_LINK_UP + 1;
    while (status >= 0 && status != CYW43_LINK_UP)
    {
        int new_status = cyw43_tcpip_link_status(
             &cyw43_state,CYW43_ITF_STA);
        if (new_status != status)
        {
            status = new_status;
            flashrate = flashrate / (status + 1);
        }
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(flashrate);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(flashrate);
    }
}

err_t headers(httpc_state_t *connection, void *arg, 
    struct pbuf *hdr, u16_t hdr_len, u32_t content_len)
{
    return ERR_OK;
}

err_t body(void *arg, struct altcp_pcb *conn, 
                            struct pbuf *p, err_t err)
{
    return ERR_OK;
}

void blink_led() {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(500);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(500);
}

float read_humidity() {

    adc_init();

    adc_gpio_init(26);
    // Select ADC input 0 (GPIO26)
    adc_select_input(0);

    const float conversion_factor = 3.3f / (1 << 12);
    uint16_t result = adc_read();

    float humidity = result * conversion_factor;

    return humidity;
}

uint16_t convert_to_uint16(float value) {
    uint16_t converted_value = static_cast<uint16_t>(value * 10000);
    return converted_value;
}

int main()
{
    stdio_init_all();
    connect_to_wifi();

    ip_addr_t ip;
    IP4_ADDR(&ip, 10, 0, 0, 4);
    httpc_connection_t settings;
    settings.result_fn = NULL;
    settings.headers_done_fn = headers;
    while (true){
        float humidity = read_humidity();

        // A value of ~ 10000 is a fully saturated plant have I found.
        // Need to figure out what the humidity for a dry plant is.
        uint16_t humidity_uint16 = convert_to_uint16(humidity);

        string humidity_str = to_string(humidity_uint16);
        string url = "/?humidity=" + humidity_str;
        err_t err = httpc_get_file(
                &ip,
                8080,
                url.c_str(),
                &settings,
                body,
                NULL,
                NULL); 

        blink_led();

    }
}
