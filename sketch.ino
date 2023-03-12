#include <LiquidCrystal_I2C.h>
#include "LcdGraph.h"

#define I2C_ADDR    0x27
#define LCD_COLUMNS 20
#define LCD_LINES   4

int i = 0;
int lastValue = 0;
LcdGraph::Style graphStyle = LcdGraph::Style::NORMAL;

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);
LcdGraph graph;

void setup() {
  // Init
  lcd.init();
  lcd.backlight();
  graph.init(LCD_COLUMNS, LCD_LINES, 0, 0, 31);
  graph.createChars(&lcd, graphStyle);

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("LDC Graph Demo!");

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
}

void loop() {
  delay(100); // this speeds up the simulation
  ++i;

  if(i%25==0){
    graphStyle = (LcdGraph::Style)((graphStyle+1)
        %LcdGraph::Style::NumStyles);
    LcdGraph::createChars(&lcd, graphStyle);
  }

  int value = max(0, lastValue + int(random(0, 30)) - int(random(0, 30)) );
  graph.addValue(value);

  graph.drawGraph(&lcd, graphStyle);

  lcd.setCursor(0,0);
  lcd.print(value);

  lastValue = value;
}
