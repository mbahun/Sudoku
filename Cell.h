#pragma once
#include <memory.h>

/*
This is a single cell defintion. Cell has a value, where max value is equal to cell size.
Also, cell has candidates - values that could form a solution. Each cell belongs to a block. 
In 9x9 game there are 9 blocks, each containing 9 cells. 
*/

class Cell {
    public:
        Cell(int size);
        Cell(const Cell &cell);
        ~Cell();

        int GetValue();
        void SetValue(int value);
        void AddCandidate(int value);
        void ClearCandidates();
        int UseNextCandidate();
        int GetCanidatesCount();
        void SetBlockIndex(int index);
        int GetBlockIndex();

    private:
        int _blockIndex;
        int _value;
        int *_candidates;
        int _size;
        int _currentCandidateIndex;
        int _candidatesCount;
};

