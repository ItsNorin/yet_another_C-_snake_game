/*
             ---===   Snakier   ===---
	    A snake game re-write for practice 
		Written by ItsNorin in C++17

		This program is built on Microsoft's Common Language Runtime
		Tested ONLY in Windows 10, using .NET v.4.0
*/
#include "mySnake.h"
#include "CDraw.h"

// waits for a keypress from user, keeps key in buffer if requested
void waitForKey(bool saveKey = false, bool clearBuffer = false)  {
	if (clearBuffer) { // clear any stored presses if needed
		while (System::Console::KeyAvailable)
			System::Console::ReadKey(true);
	}
	while (!System::Console::KeyAvailable) // wait for next key
		System::Threading::Thread::Sleep(1);
	if (!saveKey)
		System::Console::ReadKey(true);
}

// gets a number from user between given min and max
int userNum(int minNum = 0, int maxNum = 100) {
	int userNum = minNum - 1;
	bool prevCursorState = System::Console::CursorVisible;

	System::Console::CursorVisible = true;
	while (userNum < minNum || userNum > maxNum) {
		System::Console::Write("          Enter a number between {0} and {1}: ", minNum, maxNum);
		if (!int::TryParse(System::Console::ReadLine(), userNum)) // attempts to parse user's text
			userNum = minNum - 1;
	}
	System::Console::CursorVisible = prevCursorState;
	return userNum;
}

int main()  {
	using namespace System;

	bool runSnake = true;
	enum gameStates { START_SCR, MAIN_MENU, OPTIONS, VIEW_CONTROLS, INIT_GAME, PLAY_GAME, GAME_OVER } gameState = START_SCR;
	enum subOptions { OPTIONS_LIST, SET_FIELD_SIZE, SET_DIFFICULTY } optionState = OPTIONS_LIST;
	enum difficulties { EASY = 0, NORMAL, HARD, EXTREME } difficulty = NORMAL; 
	array<String^>^ diffNames = gcnew array<String^>{ "Easy", "Normal", "Hard", "Extreme" };
	const int diffDelay[] = { 350, 250, 150, 80 };
	const double scoreMultplier[] = { 0.8, 1.0, 1.2, 1.5 };

	MySnake snake; // used for snake's movement logic
	MySnake::coord fieldSizeX = 40, fieldSizeY = 20;
	
	ConsoleKeyInfo cki; // accepting keypresses from user
	Console::CursorVisible = false;

	do {
		if (Console::KeyAvailable) { 	// reads a key if avaliable and acts accordingly for current game state
			cki = Console::ReadKey(true);

			if (gameState == PLAY_GAME)  // set snake direction based on WASD and arrow keys if user is playing game
				switch (cki.Key) {
					case ConsoleKey::W: 
					case ConsoleKey::UpArrow:
						if (snake.direction != MySnake::DOWN) snake.direction = MySnake::UP;
						break;
					case ConsoleKey::S:
					case ConsoleKey::DownArrow:
						if (snake.direction != MySnake::UP) snake.direction = MySnake::DOWN;
						break;
					case ConsoleKey::D:
					case ConsoleKey::RightArrow:
						if (snake.direction != MySnake::LEFT) snake.direction = MySnake::RIGHT;
						break;
					case ConsoleKey::A:
					case ConsoleKey::LeftArrow:
						if (snake.direction != MySnake::RIGHT) snake.direction = MySnake::LEFT;
						break;
					case ConsoleKey::Spacebar:
					case ConsoleKey::Escape:
					case ConsoleKey::Enter:
						if (snake.direction == MySnake::STOP) gameState = MAIN_MENU;
						else snake.direction = MySnake::STOP;
					default: break;
				}
			else if (gameState == MAIN_MENU)
				switch (cki.Key) {
					case ConsoleKey::O:		gameState = OPTIONS;		break;
					case ConsoleKey::P:		gameState = INIT_GAME;		break;
					case ConsoleKey::C:		gameState = VIEW_CONTROLS;	break;
					case ConsoleKey::Escape: runSnake = false;
					default: break;
				}
			else if (gameState == OPTIONS) { // options menu actions
				if ((cki.Key == ConsoleKey::Spacebar || cki.Key == ConsoleKey::Escape || cki.Key == ConsoleKey::Enter) && optionState != OPTIONS_LIST) // return to options list on escape press in options submenus 
					optionState = OPTIONS_LIST;
				else {
					switch (optionState) { // do apropriate actions for keypresses for each submenu
						case OPTIONS_LIST:
							switch (cki.Key) {
								case ConsoleKey::Enter:
								case ConsoleKey::Spacebar:
								case ConsoleKey::Escape: gameState = MAIN_MENU;			break;
								case ConsoleKey::F:		optionState = SET_FIELD_SIZE;	break;
								case ConsoleKey::D:		optionState = SET_DIFFICULTY;
								default: break;
							}
							break;
						case SET_FIELD_SIZE:
							switch (cki.Key) {
								case ConsoleKey::X:		fieldSizeX = userNum(5, 100);	break;
								case ConsoleKey::Y:		fieldSizeY = userNum(5, 50);
								default: break;
							}
							break;
						case SET_DIFFICULTY:
							switch (cki.Key) {
								case ConsoleKey::E:		difficulty = EASY;		break;
								case ConsoleKey::N:		difficulty = NORMAL;	break;
								case ConsoleKey::H:		difficulty = HARD;		break;
								case ConsoleKey::X:		difficulty = EXTREME;
								default: break;
							}
					}
				}
			}	
		} // end input reading

		// does appropriate actions for current game state
		switch (gameState) {
			case START_SCR: // TODO: display better welcome message, title, description of game, etc.
				Console::Write("\n\n           Welcome to Snake\n\n        Press any key to start!\n");
				waitForKey(); // proceed to main menu on keypress
				gameState = MAIN_MENU; 

			case MAIN_MENU:
				Console::Clear();
				Console::Write("\n\n         --==  MAIN MENU  ==--\n\n          o    Options\n          c    Controls\n          p    Play the game at {0} difficulty\n", diffNames[difficulty]);
				if (runSnake) waitForKey(true); // if snake game needs to run, wait for and store keypress
				break;

			case OPTIONS:
				Console::Clear();
				switch (optionState) {
					case SET_FIELD_SIZE:
						Console::Write("\n\n         --==  FIELD SIZE  ==--\n\n          x    Change field width\n          y    Change field height\n");
						Console::Write("\n          Current field size: {0} x {1}\n\n", fieldSizeX, fieldSizeY);
						break;
					case SET_DIFFICULTY:
						Console::Write("\n\n         --== DIFFICULTY ==--\n\n          e    Easy\n          n    Normal\n          h    hard\n          x    Extreme\n\n");
						Console::Write("          Current difficulty  {0}\n          Score multiplier    x {1}\n", diffNames[difficulty], scoreMultplier[difficulty]);
						break;
					default:
						optionState = OPTIONS_LIST;
						Console::Write("\n\n         --== OPTIONS ==--\n\n          f    Field Size\n          d    Difficulty\n");
				}
				waitForKey(true);
				break;

			case VIEW_CONTROLS:
				Console::Clear();
				Console::Write("\n\n         --==  CONTROLS  ==--\n\n       Your objective is to collect as much\n       food as possible (x) without running\n       into the border or your own body\n\n       Use WASD or arrow keys to\n       change the snake's direction\n\n       Press Space or Escape to pause the game,\n       and press again to return to Main Menu\n\n       Press Enter, Escape, or Space to\n       go back in any menu.\n");
				waitForKey();
				gameState = MAIN_MENU;
				break;

			case INIT_GAME: // initialize the game then proceed to play
				Console::Clear();
				snake.begin(fieldSizeX, fieldSizeY);
				CDraw::box(0, 0, fieldSizeX + 2, fieldSizeY + 2, '#', ' '); // creates a border around game field
				CDraw::putChar('x', snake.food.x + 1, snake.food.y + 1); // start game with snake head and tail on field
				CDraw::putChar('@', snake.head.x + 1, snake.head.y + 1);
				gameState = PLAY_GAME; 

			case PLAY_GAME: // visually update snake, let MySnake handle the movement and checks
				if (snake.direction != MySnake::STOP) { // don't attempt to update anything when paused
					snake.move(); // advance the snakes!
					if (snake.eraseTail()) CDraw::putChar(' ', snake.oldTail.x + 1, snake.oldTail.y + 1); // remove tail back if asked for by MySnake
					CDraw::putChar('O', snake.tailLead.x + 1, snake.tailLead.y + 1); // draw tail where head was
					CDraw::putChar('x', snake.food.x + 1, snake.food.y + 1); // draw in food and head
					CDraw::putChar('@', snake.head.x + 1, snake.head.y + 1);
				}
				if (snake.gameOver()) gameState = GAME_OVER; // end game
				Threading::Thread::Sleep(diffDelay[difficulty]); // pause for given difficulty delay between frames
				break;

			case GAME_OVER:
				Console::Clear();
				Console::Write("\n\n         --==  GAME OVER  ==--\n\n         You had a score of {0}\n         in the {1} difficulty\n\n", 
								   static_cast<int>(snake.score()*scoreMultplier[difficulty]), diffNames[difficulty]);
				Threading::Thread::Sleep(1000);
				waitForKey(false, true);
				gameState = MAIN_MENU;
		}
	} while (runSnake);

	return 0;
}