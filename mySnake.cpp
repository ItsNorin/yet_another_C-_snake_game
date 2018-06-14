/******************************************************
*   MySnake - handles snake movement and collisions   *
*           Written by ItsNorin                       *
******************************************************/
#include "mySnake.h"

// initialize new snake game with given field size
void MySnake::begin(const coord sizeX, const coord sizeY) {
	// sets gamefield size
	if (sizeX < FIELD_X_MIN) fieldSize.x = FIELD_X_MIN; 
	else fieldSize.x = sizeX - 1;

	if (sizeY < FIELD_Y_MIN) fieldSize.y = FIELD_Y_MIN;
	else fieldSize.y = sizeY - 1; 

	head = { fieldSize.x / 2,  fieldSize.y / 2 }; // starts MySnake's head in middle of field

	body.clear(); body.shrink_to_fit(); // clear previous body and give it some length
	for (uint8_t i = 0; i < START_SIZE; i++) body.push_back(head); 
	skipTailDeletion = START_SIZE;
	toRemoveTail = gameLost = false;
	
	gameScore = 0;
	direction = STOP;

	makeFood();
}

// true if given point is in snake's body
bool MySnake::isBody(const point &checkPos) {
	for (uint32_t i = 0; i < body.size(); i++)  // searches through all coordinates for a match
		if (body[i] == checkPos) return true; // true if current point matches given
	return false; 
}

// "randomly" generates food in empty space
void MySnake::makeFood() {
	std::deque<point> emptyPoints; // potential food locations
	point toCheck;

	// store every point that is not a head or body part in emptyPoints
	for (toCheck.x = 0; toCheck.x < fieldSize.x; toCheck.x++) 
		for (toCheck.y = 0; toCheck.y < fieldSize.y; toCheck.y++)
			if ( toCheck != head && !isBody(toCheck) )
				emptyPoints.push_back(toCheck);

	// places food on a "random" empty point 
	srand(fieldSize.x + fieldSize.y - head.x + head.y - oldTail.y + gameScore);  // not random at all but good enough for now
	food = emptyPoints[rand() % emptyPoints.size()]; 
}

// checks for snake's head for collisions with anything in the field
void MySnake::checkCollisions() {
	// check if snake's head has left the game field, or if snake's head is overlapping its own body
	if (head.x > fieldSize.x || head.y > fieldSize.y	||		head.x < 0 || head.y < 0	||		isBody(head))
		gameLost = true;

	// add to score and snake's body when eating food, make more food
	else if (head == food) {
		gameScore++;
		for (uint8_t i = 0; i < FOOD_WORTH; i++)  body.push_back(body.back());
		skipTailDeletion += FOOD_WORTH;
		toRemoveTail = false;
		makeFood();
	}
}

// moves snake if possible, stores coordinate of old tail piece to be removed
void MySnake::move() {
	if (direction != STOP && !gameLost) {
		tailLead = head; // puts tail where head was before moving 
		body.push_front(head); 

		switch (direction) { // moves head according to direction
			case UP:	head.y--;	break;
			case DOWN:	head.y++;	break;
			case RIGHT:	head.x++;	break;
			case LEFT:	head.x--;	break;
		}

		checkCollisions();

		// let caller know to remove tail unless snake just ate
		if (skipTailDeletion == 0) toRemoveTail = true;
		else skipTailDeletion--;
		
		oldTail = body.back();
		body.pop_back(); // throw out old tail
	}
}