#include <iostream>
#include <chrono>
#include "Solver.h"

// 003000009000070000200580300080150004000007500100009000040000060000001000800230900

/*
070200003
400800700
013000000
500900400
700020060
000000000
200050900
105060004
000010300
*/
// 133 us

int main()
{
	const int trials = 100000;
	long* timingsUs = new long[trials] {};

	char board[BOARD_LENGTH]{
		2, 0, 6, 3, 7, 0, 0, 0, 0,
		5, 1, 0, 4, 0, 0, 7, 0, 0,
		4, 3, 0, 0, 6, 0, 0, 0, 8,
		9, 6, 0, 0, 5, 0, 0, 0, 0,
		0, 5, 3, 0, 8, 0, 0, 0, 0,
		7, 8, 0, 0, 1, 0, 5, 4, 9,
		0, 4, 0, 0, 0, 0, 0, 5, 7,
		0, 7, 5, 0, 9, 6, 2, 0, 0,
		0, 0, 1, 7, 4, 0, 3, 9, 6,
	}; // 1.2 us

	char tempBoard[BOARD_LENGTH]{};

	char c;

	for (int i = 0; i < BOARD_LENGTH; i++)
	{
		std::cin >> c;
		board[i] = c - '0';
	}

	memcpy(tempBoard, board, sizeof(board));

	PrintBoard(tempBoard);
	Solve(tempBoard);

	if (true)
	{
		for (int i = 0; i < trials; i++)
		{
			memcpy(tempBoard, board, BOARD_LENGTH * sizeof(char));
			auto start = std::chrono::steady_clock::now();
			Solve(tempBoard);
			auto end = std::chrono::steady_clock::now();
			timingsUs[i] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		}
	}

	std::cout << std::endl;

	PrintBoard(tempBoard);

	if (VerifyBoard(tempBoard))
	{
		std::cout << "Board solved!" << std::endl;
	}
	else
	{
		std::cout << "Board NOT solved!" << std::endl;
	}

	long totalTimeUs = 0;
	for (int i = 0; i < trials; i++)
	{
		totalTimeUs += timingsUs[i];
	}

	double averageTimeUs = totalTimeUs / (double)trials;

	std::cout << "Average solve time: " << averageTimeUs << " us" << std::endl;


	delete[] timingsUs;
	std::cin.get();
}