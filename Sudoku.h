#pragma once
#include "Grid.h"
#include <random>

/*
Sudoku - uses grid to solve a Sudoku problem or to create one.
*/

class Sudoku {
	public:
        enum ResultStatus {
            Solved = 0,
            ProblemIncorrect = 1,
            NoCandidates = 2,
            NoSolution = 3,
            LimitReached = 4
        };
        Sudoku();
        Sudoku(Grid::Size size);
        ~Sudoku();

        bool InsertProblemValue(int cellIndex, int value);
        bool InsertProblemValuesFromArray(int* array);
        ResultStatus Solve(int** array, int& arrayLen, int& pathsTried);
        //Create returns problem and a solution (if solutionArray != nullptr). 
        void Create(int** problemArray, int** solutionArray, int& arrayLen);

    private:
        Grid* _grid = nullptr;
        void DeleteAll(Grid* game, std::queue<Grid*>* games);
        void CreateaAndCopyToArray(Grid* grid, int** array);
        int GetRadnomValue(int min, int max);
        void Scramble(int* a, int size);
        void Generate();
};

