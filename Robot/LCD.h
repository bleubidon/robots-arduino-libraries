#pragma once

#include <helpers.h>
#include <LiquidCrystal.h>

class LCD
{
public:
    LCD();

    struct Config
    {
        uint8_t rs, enable, d4, d5, d6, d7, LCD_length, LCD_height, lcd_id;
        String default_text;
    };

    void setup(LCD::Config config);
    void clear_LCD();
    void clear_line(unsigned int ligne);
    void affichage(String message, unsigned int ligne = 1, bool clear_display = true, bool log = true);

private:
    LiquidCrystal *lcd_handle;

    unsigned int lcd_id;
    unsigned int LCD_length;
    unsigned int LCD_height;
    String default_text;
};
