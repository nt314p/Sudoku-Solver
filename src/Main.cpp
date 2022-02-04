#include <iostream>
#include <chrono>
#include "Solver.h"

int main()
{
	const int trials = 100000;
	long* timingsUs = new long[trials];

	int board[BOARD_LENGTH]{
		2, 0, 6, 3, 7, 0, 0, 0, 0,
		5, 1, 0, 4, 0, 0, 7, 0, 0,
		4, 3, 0, 0, 6, 0, 0, 0, 8,
		9, 6, 0, 0, 5, 0, 0, 0, 0,
		0, 5, 3, 0, 8, 0, 0, 0, 0,
		7, 8, 0, 0, 1, 0, 5, 4, 9,
		0, 4, 0, 0, 0, 0, 0, 5, 7,
		0, 7, 5, 0, 9, 6, 2, 0, 0,
		0, 0, 1, 7, 4, 0, 3, 9, 6,
	}; // 1.4 us

	//char c;

	//for (int i = 0; i < BOARD_LENGTH; i++)
	//{
	//	std::cin >> c;
	//	board[i] = c - '0';
	//}

	Solver s(board);

	s.PrintCells();
	s.PrintBoard();
	
	for (int i = 0; i < trials; i++) 
	{
		s = Solver(board);
		auto start = std::chrono::steady_clock::now();
		s.Solve();
		auto end = std::chrono::steady_clock::now();
		timingsUs[i] = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	}
	
	s.PrintCells();
	s.PrintBoard();

	long sum = 0;
	for (int i = 0; i < trials; i++)
	{
		sum += timingsUs[i];
	}

	double avg = sum / (double)trials;
	
	std::cout << "Average solve time: " << avg << " us" << std::endl;

	delete[] timingsUs;
	std::cin.get();
}