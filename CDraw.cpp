/**********************************************************
*   CDraw - helps with drawing text in console windows    *
*           Written by Norin                              *
* This library is built using the Common Language Runtime *
**********************************************************/
#include "CDRAW.h"

// places character at given position
void CDraw::putChar(System::Char character, unsigned int xPos, unsigned int yPos) {
	System::Console::SetCursorPosition(xPos, yPos);
	System::Console::Write(character);
}

// draws line right from given point of given length
void CDraw::line(unsigned int startX, unsigned int startY, unsigned int length, System::Char border)  {
	for (unsigned int x = 0; x < length; x++)
		CDraw::putChar(border, startX + x, startY);
}

// draws box with given outer dimensions, with upper left corner starting at given point. Can optionally specify border and fill 
void CDraw::box(unsigned int startX, unsigned int startY, unsigned int width, unsigned int height, System::Char border, System::Char fill) {
	CDraw::line(startX, startY, width, border); // draw top border
	for (unsigned int y = 1; y < height - 1; y++) // draw walls, avoids filling if fill is a space
		if (fill == ' ') {
			CDraw::putChar(border, startX, startY + y);
			CDraw::putChar(border, startX + width - 1, startY + y);
		}
		else
			for (unsigned int x = 0; x < width; x++) 
				CDraw::putChar( (x == 0 || x == width - 1) ? border : fill, startX + x, startY + y ); // draw border at each end, filled with spaces
	CDraw::line(startX, startY + height - 1, width); // draw bottom border
}