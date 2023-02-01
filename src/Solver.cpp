#include "Solver.h"

inline bool TryUpdateCell(Cell(&cells)[BOARD_LENGTH], int index, Cell mask)
{
	Cell cell = cells[index];
	Cell maskedCell = cell & ~mask;

	if (cell != 0 && maskedCell == 0) return false;

	cells[index] = maskedCell;
	return true;
}

/*
Attempts to place the number at the given index
Returns false if placing the number resulted in an invalid board state
Otherwise returns true
*/
bool PlaceNumber(char(&board)[BOARD_LENGTH], Cell(&cells)[BOARD_LENGTH], int index, int number)
{
	int row = index / 9;
	int column = index - row * 9;
	int boxRow = (row / 3) * 3;
	int boxColumn = (column / 3) * 3;

	int zeroIndexedNumber = number - 1;

	Cell cell;
	Cell mask = 1 << zeroIndexedNumber;

	if ((cells[index] & mask) == 0) std::cerr << "Cannot place number!" << std::endl;

	board[index] = number;
	cells[index] = 0;

	// TODO:
	// create a queue to add all single cell possibilities to

	int computedRow = row * SIDE_LENGTH;

	for (int i = 0; i < SIDE_LENGTH; i++) // update all cells in row and column
	{
		cell = cells[computedRow + i];

		if (cell != 0 && cell == mask) return false;

		cells[computedRow + i] = cell & ~mask;
	}

	for (int i = 0; i < SIDE_LENGTH; i++)
	{
		cell = cells[i * SIDE_LENGTH + column];

		if (cell != 0 && cell == mask) return false;

		cells[i * SIDE_LENGTH + column] = cell & ~mask;
	}

	for (int i = 0; i < BOX_LENGTH; i++)
	{
		computedRow = (boxRow + i) * SIDE_LENGTH + boxColumn;
		if (!TryUpdateCell(cells, computedRow, mask)) return false;
		if (!TryUpdateCell(cells, computedRow + 1, mask)) return false;
		if (!TryUpdateCell(cells, computedRow + 2, mask)) return false;
	}

	return true;
}

bool SolveRecursive(char(&board)[BOARD_LENGTH], Cell(&cells)[BOARD_LENGTH], int firstCellIndex)
{
	bool isSolved = false;

	bool updated = false; // has a value been updated?

	// have we gone through the entire board using the possibility algorithm and no changes have been made?
	bool shouldGuess = false;

	bool previousCellSolved = false;
	bool possibilityThresholdSatisfied = false;
	int possibilityCountThreshold = DEFAULT_POSSIBILITY_THRESHOLD;
	int possibleNumbers[9]{};

	while (!isSolved)
	{
		isSolved = true;
		updated = false;
		previousCellSolved = true;
		possibilityThresholdSatisfied = false;

		Cell cell;

		// modify for loop to pop off queue
		for (int index = firstCellIndex; index < BOARD_LENGTH; index++)
		{
			cell = cells[index];

			if (cell == 0) continue; // cell is filled

			if (previousCellSolved)
			{
				firstCellIndex = index; // this is the first index with an unsolved cell
				previousCellSolved = false;
			}

			isSolved = false;
			int possibleNumberCount = 0;

			Cell testBit = 1;

			// create an array of the possible numbers to try in the current cell
			for (int n = 1; n <= SIDE_LENGTH; n++)
			{
				if (cell & testBit) // is the bit set?
				{
					possibleNumbers[possibleNumberCount] = n;
					possibleNumberCount++;
				}

				testBit <<= 1;
			}

			if (possibleNumberCount == 1) // cell only has a single number possibility
			{
				updated = true;
				shouldGuess = false;
				if (!PlaceNumber(board, cells, index, possibleNumbers[0]))
				{
					return false; // Placing number resulted in an invalid state
				}
			}
			else if (shouldGuess) // guess
			{
				if (possibleNumberCount > possibilityCountThreshold) continue;

				possibilityThresholdSatisfied = true;
				possibilityCountThreshold = DEFAULT_POSSIBILITY_THRESHOLD;

				char tempBoard[BOARD_LENGTH];
				Cell tempCells[BOARD_LENGTH];

				// begin guessing the possible numbers
				for (int i = 0; i < possibleNumberCount; i++)
				{
					memcpy(tempBoard, board, sizeof(board));
					memcpy(tempCells, cells, sizeof(cells));

					if (!PlaceNumber(tempBoard, tempCells, index, possibleNumbers[i]))
					{
						continue; // the attempt to place the number resulted in an invalid board state
					}

					if (SolveRecursive(tempBoard, tempCells, firstCellIndex))
					{
						memcpy(board, tempBoard, BOARD_LENGTH * sizeof(char)); // copy solved data back
						memcpy(cells, tempCells, BOARD_LENGTH * sizeof(Cell));
						return true; // the number placement was valid
					}
				}

				return false; // No numbers worked, backtrack
			}
		}

		// if no cells satisfy the current possibility count threshold, increase it
		if (shouldGuess && !possibilityThresholdSatisfied)
		{
			possibilityCountThreshold++;
		}

		if (!updated) shouldGuess = true; // we have gone through the board and changed nothing
	}

	return true;
}

bool Solve(char(&board)[BOARD_LENGTH])
{
	/* A 1d array containing elements representing the 2d board cells row by row.
	   Each element is a bit field which represents what possible numbers can go in that cell.
	   If the nth bit is set (zero indexed), it is valid for the number n+1 to go there.
	*/
	Cell cells[BOARD_LENGTH]{};

	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		cells[i] = FULL_CELL;
	}

	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		if (board[i] == 0) continue;

		if (!(cells[i] & 1 << (board[i] - 1)))
		{
			std::cerr << "Invalid board!" << std::endl;
			return false;
		}

		PlaceNumber(board, cells, i, board[i]);
	}

	return SolveRecursive(board, cells, 0);
}

bool CanPlaceNumber(Cell(&cells)[BOARD_LENGTH], int index, int number)
{
	return cells[index] & 1 << (number - 1);
}

bool VerifyBoard(char(&board)[BOARD_LENGTH])
{
	Cell cellRow;
	Cell cellColumn;
	for (int i = 0; i < SIDE_LENGTH; i++)
	{
		cellRow = 0;
		cellColumn = 0;
		for (int j = 0; j < SIDE_LENGTH; j++)
		{
			cellRow |= 1 << (board[i * SIDE_LENGTH + j] - 1);
			cellColumn |= 1 << (board[j * SIDE_LENGTH + i] - 1);
		}

		if (cellRow != FULL_CELL || cellColumn != FULL_CELL) return false;
	}

	return true;
}

void PrintCells(Cell(&cells)[BOARD_LENGTH])
{
	char buffer[11]{};
	buffer[9] = ' ';
	buffer[10] = '\0';

	Cell cell;
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

void PrintBoard(char(&board)[BOARD_LENGTH])
{
	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		std::cout << (int)board[i] << ' ';
		if (i % 9 == 8) std::cout << "\n";
	}
}