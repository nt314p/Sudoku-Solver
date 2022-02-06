#include "Solver.h"

bool VerifyBoard(char(&board)[BOARD_LENGTH])
{
	UInt16 cell;
	for (int i = 0; i < SIDE_LENGTH; i++)
	{
		cell = 0;
		for (int j = 0; j < SIDE_LENGTH; j++)
		{
			cell |= 1 << (board[i * SIDE_LENGTH + j] - 1);
		}

		if (cell != 0b111111111) return false;
	}

	for (int i = 0; i < SIDE_LENGTH; i++)
	{
		cell = 0;
		for (int j = 0; j < SIDE_LENGTH; j++)
		{
			cell |= 1 << (board[j * SIDE_LENGTH + i] - 1);
		}

		if (cell != 0b111111111) return false;
	}

	return true;
}

/*
Attempts to place the number at the given index (but does not modify anything)
Returns false if placing the number resulted in an invalid board state
*/
bool CanPlaceNumber(const UInt16(&cells)[BOARD_LENGTH], int index, int number)
{

	int zeroIndexedNumber = number - 1;
	UInt16 mask = 1 << zeroIndexedNumber;

	UInt16 cell = 0;

	//if (cell !=)
	return true;
}

// TODO
// create method that checks whether there are two single possible cells which conflict with each other
// 


/*
Attempts to place the number at the given index
Returns false if placing the number resulted in an invalid board state
Otherwise returns true
*/
bool PlaceNumber(char(&board)[BOARD_LENGTH], UInt16(&cells)[BOARD_LENGTH], int index, int number)
{
	int row = index / 9;
	int column = index - row * 9;
	int boxRow = (row / 3) * 3;
	int boxColumn = (column / 3) * 3;

	int zeroIndexedNumber = number - 1;

	UInt16 cell;
	UInt16 maskedCell;
	UInt16 mask = 1 << zeroIndexedNumber;

	if ((cells[index] & mask) == 0) std::cerr << "Cannot place number!" << std::endl;

	board[index] = number;
	cells[index] = 0;

	for (int i = 0; i < SIDE_LENGTH; i++) // update all cells in row and column
	{
		cell = cells[row * SIDE_LENGTH + i];
		maskedCell = cell & ~mask;
		if (cell != 0 && maskedCell == 0) return false; // placement caused a cell to have no possibilites
		cells[row * SIDE_LENGTH + i] = maskedCell; // the number can no longer be placed in that cell

		cell = cells[i * SIDE_LENGTH + column];
		maskedCell = cell & ~mask;
		if (cell != 0 && maskedCell == 0) return false;
		cells[i * SIDE_LENGTH + column] = maskedCell;
	}

	for (int i = 0; i < BOX_LENGTH; i++)
	{
		for (int j = 0; j < BOX_LENGTH; j++)
		{
			cell = cells[(boxRow + i) * SIDE_LENGTH + boxColumn + j];
			maskedCell = cell & ~mask;
			if (cell != 0 && maskedCell == 0) return false;
			cells[(boxRow + i) * SIDE_LENGTH + boxColumn + j] = maskedCell;
		}
	}

	return true;
}

bool SolveRecursive(char(&board)[BOARD_LENGTH], UInt16(&cells)[BOARD_LENGTH], int firstCellIndex)
{
	//std::cout << "Reached another recursive call: " << firstCellIndex << std::endl;
	bool isSolved = false;

	// has a value been updated?
	bool updated = false;

	// have we gone through the entire board using the possibility algorithm and no changes have been made?
	bool shouldGuess = false;

	bool previousCellSolved = false;
	bool possibilityThresholdSatisfied = false;
	int possibilityCountThreshold = DEFAULT_POSSIBILITY_THRESHOLD;

	while (!isSolved)
	{
		isSolved = true;
		updated = false;
		previousCellSolved = true;
		possibilityThresholdSatisfied = false;

		UInt16 cell;

		for (int index = firstCellIndex; index < BOARD_LENGTH; index++)
		{
			cell = cells[index];

			if (cell == 0) // cell is filled
			{
				continue;
			}

			if (previousCellSolved)
			{
				firstCellIndex = index; // this is the first index with an unsolved cell
				previousCellSolved = false;
			}

			isSolved = false;
			int lastNumber = 0;
			int possibleNumberCount = 0;
			int possibleNumbers[9]{};

			UInt16 testBit = 1;

			for (int n = 1; n <= SIDE_LENGTH; n++)
			{
				if (cell & testBit) // is the bit set?
				{
					possibleNumbers[possibleNumberCount] = n;
					possibleNumberCount++;
					lastNumber = n;
				}

				testBit <<= 1;
			}

			if (possibleNumberCount == 1) // cell only has a single number possibility
			{
				//std::cout << "Placing " << lastNumber << " at index " << index << std::endl;

				//PrintCells(cells);
				//std::cout << "\n\n" << std::endl;

				updated = true;
				shouldGuess = false;
				PlaceNumber(board, cells, index, lastNumber);
			}
			else if (shouldGuess) // guess
			{
				if (possibleNumberCount > possibilityCountThreshold) continue;

				possibilityThresholdSatisfied = true;
				possibilityCountThreshold = DEFAULT_POSSIBILITY_THRESHOLD;

				//std::cout << "Guessing..." << std::endl;
				//PrintBoard(board);

				char tempBoard[BOARD_LENGTH];
				UInt16 tempCells[BOARD_LENGTH];

				for (int i = 0; i < possibleNumberCount; i++)
				{
					memcpy(tempBoard, board, BOARD_LENGTH * sizeof(char));
					memcpy(tempCells, cells, BOARD_LENGTH * sizeof(UInt16));

					//std::cout << "Guessing " << possibleNumbers[i] << " at index " << index << std::endl;
					//PrintCells(tempCells);
					//std::cout << "\n\n" << std::endl;

					if (!PlaceNumber(tempBoard, tempCells, index, possibleNumbers[i]))
					{
						std::cout << "Guess resulted in invalid state" << std::endl;
						continue; // the attempt to place the number resulted in an invalid board state
					}

					//PrintCells(tempCells);
					//std::cout << "\n\n" << std::endl;

					if (SolveRecursive(tempBoard, tempCells, firstCellIndex))
					{
						memcpy(board, tempBoard, BOARD_LENGTH * sizeof(char)); // copy solved data back
						memcpy(cells, tempCells, BOARD_LENGTH * sizeof(UInt16));
						return true;
					}
					//std::cout << "Incorrect guess" << std::endl;
				}

				//std::cout << "No possibilities worked, backtracking..." << std::endl;
				return false;
			}
		}

		// if no cells satisfy the current possibility count threshold, increase it
		if (shouldGuess && !possibilityThresholdSatisfied)
		{
			possibilityCountThreshold++;
			//std::cout << "No cells satisfy possibility threshold, increased to " << possibilityCountThreshold << std::endl;
		}

		if (!updated) shouldGuess = true; // we have gone through the board and changed nothing
	}

	//for (int index = 0; index < BOARD_LENGTH; index++)
	//{
	//	if (board[index] == 0)
	//	{
	//		std::cout << "Board has unsolved cells, backtracking..." << std::endl;
	//		return false;
	//	}
	//}

	//PrintCells(cells);

	if (!VerifyBoard(board)) return false;

	return true;
}

bool Solve(char(&board)[BOARD_LENGTH])
{
	/* A 1d array containing elements representing the 2d board cells row by row.
	   Each element is a bit field which represents what possible numbers can go in that cell.
	   If the nth bit is set (zero indexed), it is valid for the number n+1 to go there.
	*/
	UInt16 cells[BOARD_LENGTH]{};

	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		cells[i] = 0b111111111; // nine ones
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

		cells[i] = 0; // nothing can be place on a filled cell
	}

	return SolveRecursive(board, cells, 0);
}

bool CanPlaceNumber(UInt16(&cells)[BOARD_LENGTH], int index, int number)
{
	return cells[index] & 1 << (number - 1);
}

void PrintCells(UInt16(&cells)[BOARD_LENGTH])
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

void PrintBoard(char(&board)[BOARD_LENGTH])
{
	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		std::cout << (int)board[i] << ' ';
		if (i % 9 == 8) std::cout << "\n";
	}
}