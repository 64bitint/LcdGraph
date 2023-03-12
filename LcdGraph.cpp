=#include "LcdGraph.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"


void LcdGraph::init(uint8_t cols, uint8_t rows, uint8_t col, uint8_t row, int max)
{
  _cols = cols;
  _rows = rows;
  _col = col;
  _row = row;
  _max = max;
  _buff_pos = 0;
  _buff_fill_size = 0;
}
  
void LcdGraph::createChars(LCD_CLASS* lcd, Style graph_style)
{
  uint8_t bar_top_solid = 0b11111;
  uint8_t pattern_50percent [8] = {
    0b01010,
    0b10101,
    0b01010,
    0b10101,
    0b01010,
    0b10101,
    0b01010,
    0b10101,
  };
  uint8_t pattern_none[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
  };
  uint8_t pattern_solid[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
  };
  uint8_t pattern_thin[8] = {
    0b01110,
    0b01110,
    0b01110,
    0b01110,
    0b01110,
    0b01110,
    0b01110,
    0b01110,
  };
  uint8_t pattern_side_shade[8] = {
    0b11010,
    0b10101,
    0b11010,
    0b11101,
    0b11010,
    0b10101,
    0b11010,
    0b11101,
  };
  // set defaults
  uint8_t* bg_pattern = pattern_none;
  uint8_t* bar_pattern = pattern_50percent;
  uint8_t bar_top = bar_top_solid;

  switch(graph_style)
  {
    case Style::Solid:
      bar_pattern = pattern_solid;
    break;

    case Style::Thin:
      bar_top = 0b01110;
      bar_pattern = pattern_thin;
    break;
    
    case Style::Line:
      bar_pattern = pattern_none;
    break;

    case Style::LineDotted:
      bar_top = 0b10101;
      bar_pattern = pattern_none;
    break;
  }
  

  createChars(lcd, bar_pattern, bar_top, bg_pattern);
}

void LcdGraph::createChars(LCD_CLASS* lcd, uint8_t bar_pattern[8], uint8_t bar_top,  uint8_t bg_pattern[8])
{
  for(uint8_t i=0; i<8; i++)
  {
    uint8_t graph_char[8] = {0};
    
    for(uint8_t j=0; j<8; j++)
    {
      if( i == j )
      {
        graph_char[j] = bar_top;
      }
      else if( i < j )
      {
        graph_char[j] = bar_pattern[j];
      }
      else
      {
        graph_char[j] = bg_pattern[j];
      }
    }
    lcd->createChar((byte)(7-i),graph_char);
  }
}

void LcdGraph::setSize(uint8_t cols, uint8_t rows)
{
  _cols = cols;
  _rows = rows;
}

void LcdGraph::setOffset(uint8_t col, uint8_t row)
{
  _col = col;
  _row = row;
}

void LcdGraph::setMax(int max)
{
  _max = max;
}

void LcdGraph::addValue(int value)
{
  _max = max(_max, value);
  
  _buff_pos = (_buff_pos + 1) % LCD_GRAPH_BUFF_SIZE;
  _buff[_buff_pos] = value;

  if(_buff_fill_size < LCD_GRAPH_BUFF_SIZE){
    ++_buff_fill_size;
  }
}

void LcdGraph::drawGraph(LCD_CLASS* lcd, Style style)
{
  uint8_t max_buff_val = _rows*8-1;
  uint8_t div = (_max-1) / (max_buff_val) + 1;
  uint8_t start_col = min(_buff_fill_size, _cols)-1;

  for(int8_t col=start_col; col>=0; col--)
  {
    uint8_t buff_pos = LCD_GRAPH_BUFF_SIZE+_buff_pos+col-start_col;
    uint8_t buff_val = _buff[buff_pos%LCD_GRAPH_BUFF_SIZE] / div;
    buff_val = min((uint8_t)buff_val, max_buff_val);

    uint8_t val_full = style == Style::Line 
        || style == Style::LineDotted ? ' ' : 7;

    for(uint8_t row=0; row < _rows; row++)
    {
      int8_t row_val = buff_val-row*7-row;

      if(row_val < 0){
        row_val = ' ';
      } else if(row_val > 7) {
        row_val = val_full;
      }

      lcd->setCursor(_col+col,_rows-(_row+row+1));
      lcd->write(row_val);
    }
  }
}

void LcdGraph::clear(LCD_CLASS* lcd)
{
  _buff_pos = 0;
  _buff_fill_size = 0;

  if(NULL != lcd){
    for(int row=0; row<_rows; row++)
    {
      lcd->setCursor(_col, _row+row);
      for(int col=0; col<_cols; col++)
      {
        lcd->print(" ");
      }
    }
  }
}
