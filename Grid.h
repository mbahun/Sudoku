#pragma once
#include "Cell.h"
#include <math.h>
#include <queue>

/*
Grid is collection of cells. Size of grid (2, 3, 4) determines the number of cells.
Standard Sudoku is size of 3 (81 Cells, each cell having maximum number of 9). 
*/

class Grid {
    public:
        enum Size {Small=2, Normal=3, Big=4};
        Grid(Size size=Normal);
        Grid(const Grid &grid);
        ~Grid();
    
    public:
        bool Check();
        bool Check(int cellIndex);
        bool PrepareCandidates();
        bool PrepareCandidates(bool allInOne);
        void SetOnlyCandidateToValue();
        void ClearCandidates();
        int PickCandidate();
        bool HasEmptyCells();
        int GetCellsCount();
        void Reset();
        Cell *GetCell(int index);

    private:
        int _cellsCount;
        Cell **_cells; 
        int _maxNumberInCell;
        int *_checks; //Used to check count of equal number in one line, column or block.

        bool CheckHorizontal(int firstCellIndex); //Index of first cell in a row
        bool CheckVertical(int firstCellIndex); //Index of a first cell in a column
        bool CheckBlock(int blockIndex); //Index of any cell in a block
       
};