#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "pico/cyw43_arch.h"

// UART defines

#define UART_ID uart0
#define BAUD_RATE 115200

// Use pins 16 and 17 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 16
#define UART_RX_PIN 17

bool validatePacket(uint8_t *packet, uint size);

int main()              //RX
{
    stdio_init_all();
    sleep_ms(1000);
    cyw43_arch_init();
    uint8_t RX[4]; // Required 2 bytes of DATA to have Onboard LED on is 0xFF and 0x1C
    uint8_t byte;

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    while (true)
    {
        uint8_t byte = uart_getc(uart0); // wait for new byte

        if (byte == 0x01)
        {
            RX[0] = byte;                         // start-of-packet marker
            uart_read_blocking(uart0, &RX[1], 3); // get rest of packet

            printf("Packet: 0x%02X 0x%02X 0x%02X 0x%02X\n",
                   RX[0], RX[1], RX[2], RX[3]);

            if (validatePacket(RX, 4))
            {
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
                printf("light on\n");
            }
        }
        else
        {

            printf("Invalid start byte: 0x%02X\n", byte);
            // Blink LED fast to show bad start
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
            sleep_ms(100);
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
            sleep_ms(100);
        }
    }
}

bool validatePacket(uint8_t *packet, uint size)
{
    uint8_t Checksum = 0;
    for (uint i = 0; i < size - 1; i++)
    {
        Checksum ^= packet[i];
    }
    if (packet[0] == 0x01 && packet[1] == 0xFF && packet[2] == 0x1C && packet[3] == Checksum) // 0x01 is required CMD
    {
        printf("returned true\n");
        return true;
    }
    else
    {
        printf("returned false\n");
        return false;
    }
}