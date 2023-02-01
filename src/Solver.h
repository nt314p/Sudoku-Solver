#pragma once
#include <string>
#include <iostream>

using String = std::string;
using Cell = std::uint_fast16_t;

constexpr auto BOX_LENGTH = 3;
constexpr auto SIDE_LENGTH = 9;
constexpr auto BOARD_LENGTH = SIDE_LENGTH * SIDE_LENGTH;
constexpr auto DEFAULT_POSSIBILITY_THRESHOLD = 2;
constexpr auto FULL_CELL = 0b111111111;

/*
Returns true if the passed in board was solvable. The solved board can be
accessed through the reference passed in
*/
bool Solve(char(&board)[BOARD_LENGTH]);
void PrintBoard(char(&board)[BOARD_LENGTH]);
void PrintCells(Cell(&cells)[BOARD_LENGTH]);
bool VerifyBoard(char(&board)[BOARD_LENGTH]);