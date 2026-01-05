#include "lcd.h"

#include <util/delay.h>

#define LENGTH (sizeof(sentence) / sizeof(sentence[0]))

int main(void) {
    lcd_mgr_init();
    const char first_word[] = "This ";
    const char next_word[] = "is ";
    const char third_word[] = "a ";
    const char last_word[] = "test ";
    
    const char *sentence[] = {first_word, next_word, third_word, last_word};
    
    unsigned int word = 0;
    for (;;) {
        _delay_ms(500);
        lcd_mgr_write(sentence[word]);
        ++word;

        if (word >= LENGTH) {
            _delay_ms(1000);
            lcd_mgr_clear();
            word = 0;
        }
    } 

    return 0;
}
