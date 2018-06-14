/**********************************************************
*   CDraw - helps with drawing text in console windows    *
*           Written by Norin                              *
* This library is built using the Common Language Runtime *
**********************************************************/
#ifndef __CONSOLE_DRAW_H__
#define __CONSOLE_DRAW_H__

namespace CDraw  {
	// places character at given position
	void putChar(System::Char character, unsigned int xPos, unsigned int yPos);
	// draws line right from given point of given length
	void line(unsigned int startX, unsigned int startY, unsigned int length, System::Char border = '#');
	// draws box with given outer dimensions, with upper left corner starting at given point. Can optionally specify border and fill 
	void box(unsigned int startX, unsigned int startY, unsigned int width, unsigned int height, System::Char border = '#', System::Char fill = ' ');
}
	
#endif