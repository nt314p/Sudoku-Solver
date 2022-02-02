#include "Solver.h"

Solver::Solver(int(&board)[BOARD_LENGTH])
{
	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		if (board[i] == 0) continue;
		this->board[i].set(board[i] - 1);
	}
};

void Solver::PrintBoard() const
{
	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		std::cout << board[i]<< ' ';
		if (i % 9 == 8) std::cout << "\n\n";
	}
}

bool Solver::IsCellSolved(int row, int column) const
{
	return board[row + column * SIDE_LENGTH].count() == 1;
}