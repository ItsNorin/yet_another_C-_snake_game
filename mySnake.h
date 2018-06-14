/******************************************************
*   MySnake - handles snake movement and collisions   *
*           Written by ItsNorin                       *
******************************************************/
#ifndef __MY_SNAKE_H__
#define __MY_SNAKE_H__

#include <deque>

#define FIELD_X_MIN 5
#define FIELD_Y_MIN 5

#define START_SIZE 3
#define FOOD_WORTH 2

class MySnake {
	public:

		typedef __int16 coord;
		struct point { // for working with cartesian points
			coord x, y; 
			// easy checking whether two points are the same or not
			bool operator==(const point &comparison) { return (x == comparison.x && y == comparison.y); }
			bool operator!=(const point &comparison) { return !operator==(comparison); }
		}; 

		enum moveDirections { STOP, UP, DOWN, LEFT, RIGHT } direction; // valid movement directions

		point head, tailLead, oldTail, food; // snake's head and old tail positions

		bool gameOver() { return gameLost; }; // true if game is over
		bool eraseTail() { return toRemoveTail; }; // true if tail needs to be removed
		unsigned int score() { return gameScore; }; // gives score

		void begin(const coord sizeX = 0, const coord sizeY = 0); // initialize snake game with given field size
		void move(); // moves snake in current direction

	private:

		bool gameLost, toRemoveTail;
		unsigned int gameScore;
		uint8_t skipTailDeletion;

		point fieldSize; // field size in x and y

		std::deque<point> body; // set of all positions of snake's body
		
		bool isBody(const point &checkPos); // attempts to find given point in MySnake's body
		void checkCollisions(); // checks for snake's head for collisions
		void makeFood(); // gives food a random coordinate not on snake's body
};

#endif