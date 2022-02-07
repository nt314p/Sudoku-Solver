#include "Solver.h"

inline bool TryUpdateCell(UInt16(&cells)[BOARD_LENGTH], int index, UInt16 mask)
{
	UInt16 cell = cells[index];
	UInt16 maskedCell = cell & ~mask;

	if (cell != 0 && maskedCell == 0) return false;

	cells[index] = maskedCell;
	return true;
}

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
	UInt16 mask = 1 << zeroIndexedNumber;

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
		cell = cells[i*SIDE_LENGTH + column];

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

bool SolveRecursive(char(&board)[BOARD_LENGTH], UInt16(&cells)[BOARD_LENGTH], int firstCellIndex)
{
	//std::cout << "Reached another recursive call: " << firstCellIndex << std::endl;
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

		UInt16 cell;

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

			UInt16 testBit = 1;

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
				//std::cout << "Placing " << lastNumber << " at index " << index << std::endl;
				//PrintCells(cells);
				//std::cout << "\n\n" << std::endl;

				updated = true;
				shouldGuess = false;
				if (!PlaceNumber(board, cells, index, possibleNumbers[0]))
				{
					//std::cout << "Placing number resulted in invalid state" << std::endl;
					return false;
				}

				//PrintCells(cells);
				//std::cout << "\n\n" << std::endl;
				//PrintBoard(board);
				//std::cout << "\n\n" << std::endl;
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
					memcpy(tempBoard, board, sizeof(board));
					memcpy(tempCells, cells, sizeof(cells));

					//std::cout << "Guessing " << possibleNumbers[i] << " at index " << index << std::endl;
					//PrintCells(tempCells);
					//std::cout << "\n\n" << std::endl;

					if (!PlaceNumber(tempBoard, tempCells, index, possibleNumbers[i]))
					{
						//std::cout << "Guess resulted in invalid state" << std::endl;
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

	//if (!VerifyBoard(board)) return false;

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

bool CanPlaceNumber(UInt16(&cells)[BOARD_LENGTH], int index, int number)
{
	return cells[index] & 1 << (number - 1);
}

bool VerifyBoard(char(&board)[BOARD_LENGTH])
{
	UInt16 cellRow;
	UInt16 cellColumn;
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