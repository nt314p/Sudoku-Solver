#pragma once
#include <string>
#include <iostream>
#include <bitset>

using String = std::string;
using UInt16 = std::uint16_t;

constexpr auto SIDE_LENGTH = 9;
constexpr auto BOARD_LENGTH = SIDE_LENGTH * SIDE_LENGTH;

class Solver 
{
private:
	/* A one dimensional array containing elements representing the board cells row by row.
	   Each UInt16 is a bit field which represents what possible numbers can go in that cell.
	   If the nth bit is set (zero indexed), it is valid for the number n+1 to go there.
	*/
	std::bitset<SIDE_LENGTH> board[BOARD_LENGTH];

public:
	Solver(int(&board)[BOARD_LENGTH]);
	void PrintBoard() const;
	bool IsCellSolved(int row, int column) const;
	void Solve();
};