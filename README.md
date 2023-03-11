# LcdGraph

Arduino/ESP32 Bar graph library uses custom charactors to draw a bar graph on
LCD displays.

## How to use

Copy [LcdGraph.cpp](LcdGraph.cpp) and [LcdGraph.h](LcdGraph.h) into your project.

Call ```addValue()``` to update the graph's values

Call ```drawGraph()``` to update the LCD display with the update graph

To use non a I2C LCD remove
```#define USE_I2C```
from the [LcdGraph.h](LcdGraph.h) header file.

## Example

Try it out in the Wokwi online [ESP32 simulator](https://wokwi.com/projects/358930804474816513)