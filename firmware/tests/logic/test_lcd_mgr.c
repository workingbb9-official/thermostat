#include "logic/lcd_mgr.h"

#include <util/delay.h>

#define LENGTH (sizeof(sentence) / sizeof(sentence[0]))

int main(void) {
    lcd_mgr_init();
    const char first_word[] = "Hold ";
    const char next_word[] = "Planet ";
    const char third_word[] = "Fitness ";
    const char last_word[] = "2027";
    
    const char *sentence[] = {first_word, next_word, third_word, last_word};
    
    int word = 0;
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
