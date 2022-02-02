#include <iostream>
#include "Solver.h"

int main()
{
	int board[BOARD_LENGTH]{};

	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		board[i] = (i % 9) + 1;
		if (i == 43) board[i] = 0;
	}
	
	Solver s(board);

	s.PrintBoard();
	std::cout << s.IsCellSolved(7, 4) << std::endl;

	std::cin.get();
}