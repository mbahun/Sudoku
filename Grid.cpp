#include "Grid.h"


Grid::Grid(Size size){
    int blockIndex = -1;
    int cnt = -1;

    _cellsCount = (int)pow((int)size, 4);
    _maxNumberInCell = (int)pow((int)size, 2);
    _cells = new Cell*[_cellsCount];
    _checks = new int[(int)(_maxNumberInCell+1)];

    for (int i=0; i<_cellsCount; i++) {
        _cells[i] = new Cell(_maxNumberInCell);

        if (i % (_maxNumberInCell * size) == 0) {
            cnt = i / int(_maxNumberInCell);
        }

        if (i % (_maxNumberInCell) == 0) {
            blockIndex = cnt -1;
        }

        if (i % size == 0) {
            blockIndex += 1;
        }

        _cells[i]->SetBlockIndex(blockIndex);
    }
}


Grid::Grid(const Grid &grid){
    _cellsCount = grid._cellsCount;
    _maxNumberInCell = grid._maxNumberInCell;
    _checks = new int[ _maxNumberInCell+1];
    
    _cells = new Cell*[grid._cellsCount];
    for (int i=0; i<_cellsCount; i++){
        _cells[i] = new Cell(*grid._cells[i]);
    }
}


Grid::~Grid(){
    for (int i=0; i<_cellsCount; i++){
        delete _cells[i];
    }
    delete[] _cells;
    delete[] _checks;
}


Cell *Grid::GetCell(int index){
    if (index < 0 || index >= _cellsCount ){
        return nullptr;
    }

    return _cells[index];
}


bool Grid::CheckHorizontal(int firstCellIndex){
    memset(_checks, 0, sizeof(int)*(_maxNumberInCell+1));

    for (int i = firstCellIndex; i < (firstCellIndex + _maxNumberInCell); i++){
        if (++_checks[_cells[i]->GetValue()] > 1 && _cells[i]->GetValue() != 0) { 
            return false;
        }
    }

    return true;
}


bool Grid::CheckVertical(int firstCellIndex){
    memset(_checks, 0, sizeof(int)*(_maxNumberInCell+1));

    for (int i = firstCellIndex; i <= (_maxNumberInCell * (_maxNumberInCell-1)) + firstCellIndex; i+=_maxNumberInCell){
        if (++_checks[_cells[i]->GetValue()] > 1 && _cells[i]->GetValue() != 0) { 
            return false;
        }
    }

    return true;
}


bool Grid::CheckBlock(int blockIndex){
    int size = (int)sqrt(_maxNumberInCell);
    int m = blockIndex % size;
    //get first cell index in the block from block index
    int cellIndex = ((blockIndex - m) * _maxNumberInCell) + (size * m);

    memset(_checks, 0, sizeof(int) * (_maxNumberInCell + 1));
    
    for (int i = 0; i < size; i++) {
        for (int j = cellIndex; j < cellIndex + (_maxNumberInCell * size); j += _maxNumberInCell) {
            if (++_checks[_cells[i+j]->GetValue()] > 1 && _cells[i+j]->GetValue() != 0) {
                return false;
            }
        }
    }
    return true;
}


bool Grid::Check(){
    int block = 0;

    for (int i=0; i<_maxNumberInCell; i++){
        if (!CheckVertical(i)){
            return false;
        }

        if (!CheckHorizontal(i*_maxNumberInCell)){
            return false;
        }

        if (!CheckBlock(i)) {
            return false;
        }
    }
    
    return true;
}


bool Grid::Check(int cellIndex) {
    if (cellIndex < 0)
        return false;

    int vIndex = cellIndex % _maxNumberInCell; 
    int hIndex = (cellIndex / _maxNumberInCell) * _maxNumberInCell;
    int bIndex = _cells[cellIndex]->GetBlockIndex();

    if (!CheckVertical(vIndex)) {
        return false;
    }

    if (!CheckHorizontal(hIndex)) {
        return false;
    }

    if (!CheckBlock(bIndex)) {
        return false;
    }

    return true;
}


bool Grid::PrepareCandidates() {
    bool ret = false;

    for (int i = 0; i < _cellsCount; i++) {
        if (_cells[i]->GetValue() == 0) {
            _cells[i]->ClearCandidates();

            for (int j = 0; j < _maxNumberInCell; j++) {
                _cells[i]->SetValue(j + 1);

                if (Check(i)) {
                    _cells[i]->AddCandidate(j + 1);
                    ret = true;
                }

                _cells[i]->SetValue(0);
            }           
        }
    }
    return ret;
}


void Grid::SetOnlyCandidateToValue(){
    for (int i=0; i<_cellsCount; i++){
        if (_cells[i]->GetCanidatesCount() == 1 && _cells[i]->GetValue() == 0){
            _cells[i]->UseNextCandidate();
        }
    }
}


int Grid::PickCandidate(){
    int count, index = -1;
    int countMin = GetCellsCount();

    for (int i=0; i<_cellsCount; i++){ 
        count = _cells[i]->GetCanidatesCount();

        if (count == 2) {
            return i;
        }

        if (count > 1 && count < countMin){
            countMin = count;
            index = i;
        }
    }

    return index;
}


void Grid::ClearCandidates(){
    for (int i=0; i<_cellsCount; i++){ 
        _cells[i]->ClearCandidates();
    }
}


bool Grid::HasEmptyCells(){
    for (int i=0; i<_cellsCount; i++){
        if (_cells[i]->GetValue() == 0){
            return true;
        }
    }

    return false;
}


int Grid::GetCellsCount() {
    return _cellsCount;
}


void Grid::Reset() {
    ClearCandidates();
    for (int i = 0; i < _cellsCount; i++) {
        _cells[i]->SetValue(0);
    }
}

