#include "mbed.h"

// Hardware Pin Definitions
DigitalOut led_out(A6);          // Status LED
DigitalOut signal_out(D5);        // Signal output
DigitalIn mode_btn(D10, PullUp);  // Mode button
DigitalIn reset_btn(D11, PullUp); // Reset button
AnalogIn pot(A0);                 // Potentiometer for frequency

// Operation Modes
#define MODE_IDLE    0
#define MODE_SQUARE  1
#define MODE_PATTERN 2

// Timing Constants (us)
#define BLINK_DELAY 500000U   // 500ms LED blink
#define DEBOUNCE    4500U     // Button debounce
#define LOOP_DELAY  100U      // 100us main loop delay

// Frequency Range (Hz)
#define MIN_FREQ    100
#define MAX_FREQ    2000

int main() {
    int current_mode = MODE_IDLE;
    uint32_t last_blink = 0;
    uint32_t last_signal = 0;
    bool signal_state = false;

    led_out = 0;
    signal_out = 0;

    while (true) {
        uint32_t now = us_ticker_read();

        // Mode button with debounce
        if (mode_btn == 0) {
            wait_us(DEBOUNCE);
            if (mode_btn == 0) {
                current_mode++;
                if (current_mode > MODE_PATTERN) current_mode = MODE_IDLE;
                last_signal = now;
                signal_state = false;
                while (mode_btn == 0);
            }
        }

        // Reset button with debounce
        if (reset_btn == 0) {
            wait_us(DEBOUNCE);
            if (reset_btn == 0) {
                current_mode = MODE_IDLE;
                last_signal = now;
                signal_state = false;
                while (reset_btn == 0);
            }
        }

        // Mode execution
        switch (current_mode) {
            case MODE_IDLE:
                led_out = 0;
                signal_out = 0;
                break;

            case MODE_SQUARE: {
                // 1Hz LED blink
                if (now - last_blink > BLINK_DELAY) {
                    led_out = !led_out;
                    last_blink = now;
                }
                // Read potentiometer and calculate frequency
                float pot_val = pot.read();
                int freq = MIN_FREQ + (pot_val * (MAX_FREQ - MIN_FREQ));
                uint32_t half_period = 500000 / freq;
                // Non-blocking square wave output
                if (now - last_signal >= half_period) {
                    signal_state = !signal_state;
                    signal_out = signal_state;
                    last_signal = now;
                }
                break;
            }

            case MODE_PATTERN:
                led_out = 1;
                signal_out = 0;
                break;

            default:
                current_mode = MODE_IDLE;
                led_out = 0;
                signal_out = 0;
                break;
        }
        wait_us(LOOP_DELAY);
    }
}