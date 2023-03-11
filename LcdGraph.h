#ifndef LcdGraph_h
#define LcdGraph_h

#ifndef LCD_GRAPH_BUFF_SIZE
#define LCD_GRAPH_BUFF_SIZE 20
#endif

// remove this define to use LiquidCrystal.h
#define USE_I2C

#ifdef USE_I2C
#define LCD_CLASS LiquidCrystal_I2C
#include <LiquidCrystal_I2C.h>
#else
#define LCD_CLASS LiquidCrystal
#include <LiquidCrystal.h>
#endif

class LcdGraph {
public:
  enum Style {
    NORMAL=0,
    Solid,
    Thin,
    Line,
    LineDotted,
    NumStyles,
  };
  void init(uint8_t cols, uint8_t rows, uint8_t col, uint8_t row, int max);
  static void createChars(LCD_CLASS* lcd, Style style=Style::NORMAL);
  static void createChars(LCD_CLASS* lcd, uint8_t bar_pattern[8], uint8_t bar_top,  uint8_t bg_pattern[8]);

  void setSize(uint8_t cols, uint8_t rows);
  void setOffset(uint8_t col, uint8_t row);
  void setMax(int max);

  void addValue(int value);
  
  void drawGraph(LCD_CLASS* lcd, Style style=Style::NORMAL);
  void clear(LCD_CLASS* lcd=NULL);
private:  
  int _buff[LCD_GRAPH_BUFF_SIZE];
  uint8_t _buff_pos;
  uint8_t _buff_fill_size;
  
  uint8_t _cols; 
  uint8_t _rows; 
  
  uint8_t _col; 
  uint8_t _row;
  
  int _max;
};

#endif
