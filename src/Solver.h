#pragma once
#include <string>
#include <iostream>
#include <bitset>

using String = std::string;
using UInt16 = std::uint16_t;

constexpr auto BOX_LENGTH = 3;
constexpr auto SIDE_LENGTH = 9;
constexpr auto BOARD_LENGTH = SIDE_LENGTH * SIDE_LENGTH;

class Solver
{
private:
	/* A 1d array containing elements representing the 2d board cells row by row.
	   Each element is a bit field which represents what possible numbers can go in that cell.
	   If the nth bit is set (zero indexed), it is valid for the number n+1 to go there.
	*/
	UInt16 cells[BOARD_LENGTH];

	/* A 1d array representing a 3x3x9 array. It is indexed as x = [boxRow][boxCol][n], where
	   x is the number of times the number n + 1 can be placed within cells in the box at
	   (boxRow, boxCol) ([0..2], [0..2]). 
	   Accessed as arr[boxRow * BOX_LENGTH * SIDE_LENGTH + boxCol * SIDE_LENGTH + n]
	*/
	UInt16 boxPossibleNumberCount[BOX_LENGTH * BOX_LENGTH * SIDE_LENGTH];

	int board[BOARD_LENGTH];

public:
	Solver(int(&board)[BOARD_LENGTH]);
	bool CanPlaceNumber(int row, int column, int number);
	bool TryPlaceNumber(int row, int column, int number);
	void PrintBoard() const;
	void PrintCells() const;
	bool IsCellSolved(int row, int column) const;
	void Solve();
};