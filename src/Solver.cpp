#include "Solver.h"

Solver::Solver(int(&board)[BOARD_LENGTH])
{
	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		cells[i] = 0b111111111; // nine ones
	}

	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		this->board[i] = board[i];
		if (board[i] == 0) continue;

		if (!TryPlaceNumber(i / 9, i % 9, board[i]))
		{
			std::cerr << "Invalid board!" << std::endl;
		}

		cells[i] = 0; // nothing can be place on a filled cell
	}
};

void Solver::PrintCells() const
{
	char buffer[11]{};
	buffer[9] = ' ';
	buffer[10] = '\0';

	UInt16 cell;
	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		cell = cells[i];

		for (int n = 0; n < SIDE_LENGTH; n++)
		{
			buffer[SIDE_LENGTH - n - 1] = (cell & (1 << n)) ? n + 1 + '0' : '-';
		}

		std::cout << buffer;
		if (i % 9 == 8) std::cout << "\n\n";
	}
}

void Solver::PrintBoard() const
{
	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		std::cout << board[i] << ' ';
		if (i % 9 == 8) std::cout << "\n";
	}
}

bool Solver::CanPlaceNumber(int row, int column, int number)
{
	return cells[row * SIDE_LENGTH + column] & 1 << (number - 1);
}

bool Solver::TryPlaceNumber(int row, int column, int number)
{
	int computedRow = row * SIDE_LENGTH;
	int boxRow = row / 3;
	int boxColumn = column / 3;

	int zeroIndexedNumber = number - 1;
	UInt16 cell;
	UInt16 mask = 1 << zeroIndexedNumber;

	if (!CanPlaceNumber(row, column, number)) return false; // we cannot place the number here

	board[row * SIDE_LENGTH + column] = number;

	for (int i = 0; i < SIDE_LENGTH; i++) // update all cells in row
	{
		cell = cells[computedRow + i];

		if (cell & mask) // true if bit set
		{

		}
		cells[computedRow + i] &= ~mask; // the number can no longer be placed in that cell
	}

	for (int i = 0; i < SIDE_LENGTH; i++) // update all cells in column
	{
		cells[i * SIDE_LENGTH + column] &= ~mask; // zero the number bit
	}

	boxPossibleNumberCount[boxRow * BOX_LENGTH * SIDE_LENGTH + boxColumn * SIDE_LENGTH + zeroIndexedNumber]--;

	for (int i = 0; i < BOX_LENGTH; i++)
	{
		for (int j = 0; j < BOX_LENGTH; j++)
		{
			cells[(boxRow * 3 + i) * SIDE_LENGTH + boxColumn * 3 + j] &= ~mask;
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
		UInt16 cell;

		for (int row = 0; row < SIDE_LENGTH; row++)
		{
			for (int column = 0; column < SIDE_LENGTH; column++)
			{
				cell = cells[row * SIDE_LENGTH + column];

				if (cell) // does the current cell have possible numbers left?
				{
					isSolved = false;
					int lastNumber = 0;
					int possibleNumbers = 0;

					UInt16 testBit = 1;

					for (int n = 1; n <= SIDE_LENGTH; n++)
					{
						if (cell & testBit) // is the bit set?
						{
							possibleNumbers++;
							lastNumber = n;
						}
						
						testBit <<= 1;
					}

					if (possibleNumbers == 1) // cell only has a single number possibility!
					{
						updated = true;
						if (!TryPlaceNumber(row, column, lastNumber))
						{
							std::cerr << "Cannot place number, this should not be happening!" << std::endl;
						}
					}
				}
			}
		}

		if (!updated) return; // we have gone through the board and changed nothing
	}
}