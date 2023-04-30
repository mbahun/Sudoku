#pragma once
#include "Sudoku.h"
#include <iostream>
#include <iomanip>

#ifdef _WIN32
	#include "windows.h"
	#define CLEAR_SCREEN system("cls")
#elif __linux__
	#include "time.h"
	#define CLEAR_SCREEN system("clear")
#else 
	#error "OS not supported"
#endif 

/*
Menu based CLI class. It also contais some predefined test input data.
*/

class Cli {
	public:
		void MainMenu();
		int MenuSelectAction(Grid::Size size);
		void MenuSolve(Grid::Size size);
		void MenuCreate(Grid::Size size);

	private:
		Sudoku* _sudoku = nullptr;
		struct SudokuResult {
			int* array;
			int arrayLen;
			int combinationsTried;
			double seconds;
		};

		int ReadUserInput(bool justEnter=false);
		void SudokuSolve(Grid::Size size);
		Sudoku::ResultStatus SudokuSolveAndMeasureTime(SudokuResult* result);
		void SudokuCreate(Grid::Size size);
		void Print(int* array, int arrayLen, Grid::Size size);
		void InputValues(Grid::Size size);
		void InputTestValues(Grid::Size size);
		void UseTestValues2x2();
		void UseTestValues3x3();
		void UseTestValues4x4();
};