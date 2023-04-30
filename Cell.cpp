#include "Cell.h"


Cell::Cell(int size){
     if (size <= 0) {
        size = 9;
     }

    _candidates = new int[size];
    memset(_candidates, 0, sizeof(int)*size);
    _value = 0;
    _blockIndex = -1;
    _size = size;
    _currentCandidateIndex = -1;
    _candidatesCount = 0;
}


Cell::Cell(const Cell &cell){
    _candidates = new int[cell._size];
    memset(_candidates, 0, sizeof(int) * cell._size);

    _currentCandidateIndex = -1;
    _candidatesCount = 0;

    _blockIndex = cell._blockIndex;
    _size = cell._size;
    _value = cell._value;
}


Cell::~Cell(){
    delete[] _candidates;
}


int Cell::GetValue(){
    return _value;
}


void Cell::SetValue(int value){
    if (value > _size || value < 0) {
        return;
    }

    _value = value;
}


void Cell::AddCandidate(int value){
    _candidates[_candidatesCount++] = value;
}


void Cell::ClearCandidates(){
    _currentCandidateIndex = -1;
    _candidatesCount = 0;
    memset(_candidates, 0, sizeof(int)*_size);
}


int Cell::UseNextCandidate(){
    if (!(_currentCandidateIndex + 1 > _candidatesCount)){
        _candidatesCount--;
        _value = _candidates[++_currentCandidateIndex]; 
    }
    
    return _value;
}


int Cell::GetCanidatesCount(){
    return _candidatesCount;
}


void Cell::SetBlockIndex(int index){
    _blockIndex = index;
}


int Cell::GetBlockIndex(){
    return _blockIndex;
}