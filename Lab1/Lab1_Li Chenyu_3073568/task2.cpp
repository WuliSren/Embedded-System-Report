#include "mbed.h"

// Hardware Pin Definitions
DigitalOut led_out(A6);          // Status LED
DigitalIn mode_btn(D10, PullUp);  // Mode button
DigitalIn reset_btn(D11, PullUp); // Reset button

// Operation Modes
#define MODE_IDLE    0
#define MODE_SQUARE  1
#define MODE_PATTERN 2

// Timing Constants (us)
#define BLINK_DELAY 500000U   // 500ms LED blink
#define DEBOUNCE    4500U     // Button debounce
#define LOOP_DELAY  100U      // 100us main loop delay

int main() {
    int current_mode = MODE_IDLE;
    uint32_t last_blink = 0;

    led_out = 0;

    while (true) {
        uint32_t now = us_ticker_read();

        // Mode button with debounce
        if (mode_btn == 0) {
            wait_us(DEBOUNCE);
            if (mode_btn == 0) {
                current_mode++;
                if (current_mode > MODE_PATTERN) current_mode = MODE_IDLE;
                while (mode_btn == 0);
            }
        }

        // Reset button with debounce
        if (reset_btn == 0) {
            wait_us(DEBOUNCE);
            if (reset_btn == 0) {
                current_mode = MODE_IDLE;
                while (reset_btn == 0);
            }
        }

        // Mode execution
        switch (current_mode) {
            case MODE_IDLE:
                led_out = 0;
                break;

            case MODE_SQUARE:
                // 1Hz LED blink
                if (now - last_blink > BLINK_DELAY) {
                    led_out = !led_out;
                    last_blink = now;
                }
                break;

            case MODE_PATTERN:
                led_out = 1;
                break;

            default:
                current_mode = MODE_IDLE;
                led_out = 0;
                break;
        }
        wait_us(LOOP_DELAY);
    }
}