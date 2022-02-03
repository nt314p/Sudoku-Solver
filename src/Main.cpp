#include <iostream>
#include "Solver.h"

int main()
{
	int board[BOARD_LENGTH]{
		2, 0, 6, 3, 7, 0, 0, 0, 0,
		5, 1, 0, 4, 0, 0, 7, 0, 0,
		4, 3, 0, 0, 6, 0, 0, 0, 8,
		9, 6, 0, 0, 5, 0, 0, 0, 0,
		0, 5, 3, 0, 8, 0, 0, 0, 0,
		7, 8, 0, 0, 1, 0, 5, 4, 9,
		0, 4, 0, 0, 0, 0, 0, 5, 7,
		0, 7, 5, 0, 9, 6, 2, 0, 0,
		0, 0, 1, 7, 4, 0, 3, 9, 6,
	};

	char c;

	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		std::cin >> c;
		board[i] = c - '0';
	}

	Solver s(board);

	s.PrintCells();
	s.PrintBoard();
	
	s.Solve();

	s.PrintCells();
	s.PrintBoard();
	
	std::cin.get();
}