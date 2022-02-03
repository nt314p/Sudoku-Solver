#include "Solver.h"

Solver::Solver(int(&board)[BOARD_LENGTH])
{
	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		cells[i].set();
	}

	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		this->board[i] = board[i];
		if (board[i] == 0) continue;

		if (!TryPlaceNumber(i / 9, i % 9, board[i]))
		{
			std::cerr << "Invalid board!" << std::endl;
		}

		cells[i].reset(); // nothing can be place on a filled cell
	}
};

void Solver::PrintCells() const
{
	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		std::cout << cells[i]<< ' ';
		if (i % 9 == 8) std::cout << "\n\n";
	}
}

void Solver::PrintBoard() const
{
	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		std::cout << board[i] << ' ';
		if (i % 9 == 8) std::cout << "\n\n";
	}
}

bool Solver::IsCellSolved(int row, int column) const
{
	return !cells[row * SIDE_LENGTH + column].any();
}

bool Solver::CanPlaceNumber(int row, int column, int number)
{
	return cells[row * SIDE_LENGTH + column].test(number - 1);
}

bool Solver::TryPlaceNumber(int row, int column, int number)
{
	int computedRow = row * SIDE_LENGTH;

	if (!CanPlaceNumber(row, column, number)) return false; // we cannot place the number here

	board[row * SIDE_LENGTH + column] = number;

	for (int i = 0; i < SIDE_LENGTH; i++) // update all cells in row
	{
		cells[computedRow + i].reset(number - 1); // the number can no longer be placed in that cell
	}

	for (int i = 0; i < SIDE_LENGTH; i++) // update all cells in column
	{
		cells[i * SIDE_LENGTH + column].reset(number - 1);
	}

	int boxRow = 3 * (row / 3);
	int boxColumn = 3 * (column / 3);

	for (int i = 0; i < BOX_LENGTH; i++)
	{
		for (int j = 0; j < BOX_LENGTH; j++)
		{
			cells[(boxRow + i) * SIDE_LENGTH + boxColumn + j].reset(number - 1);
		}
	}

	return true;
}

void Solver::Solve()
{
	bool isSolved = false;
	bool updated = false;
	while (!isSolved)
	{
		isSolved = true;
		updated = false;
		for (int row = 0; row < SIDE_LENGTH; row++)
		{
			for (int column = 0; column < SIDE_LENGTH; column++)
			{
				std::bitset<SIDE_LENGTH> cell = cells[row * SIDE_LENGTH + column];

				if (cell.any())
				{
					isSolved = false;
					if (cell.count() == 1)
					{
						for (int n = 1; n <= SIDE_LENGTH; n++)
						{
							if (cell.test(n - 1))
							{
								updated = true;
								if (!TryPlaceNumber(row, column, n))
								{
									std::cerr << "Cannot place number, this should not be happening!" << std::endl;
								}
								break;
							}
						}
					}
				}
			}
		}

		if (!updated) return;
	}
}