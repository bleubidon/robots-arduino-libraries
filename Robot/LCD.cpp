#include "LCD.h"

LCD::LCD()
{
}

void LCD::setup(LCD::Config config)
{
	this->lcd_id = config.lcd_id;
	this->LCD_length = config.LCD_length;
	this->LCD_height = config.LCD_height;
	this->default_text = config.default_text;

	lcd_handle = new LiquidCrystal(config.rs, config.enable, config.d4, config.d5, config.d6, config.d7);
	lcd_handle->begin(config.LCD_length, config.LCD_height);
	this->clear_LCD();
}

void LCD::clear_LCD()
{
	lcd_handle->clear();
	affichage(default_text, 0, false, true);
}

void LCD::clear_line(unsigned int ligne)
{
	// Generate blank string according to LCD_length
	String blank = String(this->LCD_length, " ");
	lcd_handle->setCursor(0, ligne);
	lcd_handle->print(blank);
}

void LCD::affichage(String message, unsigned int ligne, bool clear_display, bool log)
{
	if (log)
		LOG(Serial << "LCD: " << message << endl);

	if (ligne > 1)
		ligne = 1;
	if (clear_display)
		this->clear_LCD();

	int message_length = message.length();
	int cursor = 0;
	if (message_length < this->LCD_length)
		cursor = (message_length % 2 == 0) ? this->LCD_length / 2 - message_length / 2 : this->LCD_length / 2 - (message_length + 1) / 2;

	this->clear_line(ligne);
	lcd_handle->setCursor(cursor, ligne);
	lcd_handle->print(message);
}
