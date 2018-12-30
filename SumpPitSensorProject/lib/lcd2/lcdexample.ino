#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(3,1);
  lcd.print("by Ravee.work");
  lcd.setCursor(5,2);
  Time.zone(-5.00);
  lcd.setCursor(1,3);
  Time.zone(-5.00);
}


void loop()
{    lcd.setCursor(5,2);
    // Format from C library: https://www.gnu.org/software/libc/manual/html_node/Low_002dLevel-Time-String-Parsing.html
    lcd.print(Time.format(Time.now(), "%T"));
    delay(1000);
        lcd.setCursor(1,3);
    // Format from C library: https://www.gnu.org/software/libc/manual/html_node/Low_002dLevel-Time-String-Parsing.html
    lcd.print(Time.format(Time.now(), "%a %b %d,%Y"));
    delay(1000);
}