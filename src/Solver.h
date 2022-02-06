#pragma once
#include <string>
#include <iostream>
#include <bitset>


using String = std::string;
using UInt16 = std::uint16_t;

constexpr auto BOX_LENGTH = 3;
constexpr auto SIDE_LENGTH = 9;
constexpr auto BOARD_LENGTH = SIDE_LENGTH * SIDE_LENGTH;
constexpr auto DEFAULT_POSSIBILITY_THRESHOLD = 2;

/*
Returns true if the passed in board was solvable. The solved board can be
accessed through the reference passed in
*/
bool Solve(char(&board)[BOARD_LENGTH]);
void PrintBoard(char(&board)[BOARD_LENGTH]);
void PrintCells(UInt16(&cells)[BOARD_LENGTH]);
bool VerifyBoard(char(&board)[BOARD_LENGTH]);
//void PrintCells() const;