#include "Sudoku.h"

Sudoku::Sudoku() {
    _grid = new Grid(Grid::Normal);
}


Sudoku::Sudoku(Grid::Size size) {
    _grid = new Grid(size);
}


Sudoku::~Sudoku() {
    delete _grid;
}


bool Sudoku::InsertProblemValue(int cellIndex, int value) {
    _grid->GetCell(cellIndex)->SetValue(value);
    return _grid->Check();
}


bool Sudoku::InsertProblemValuesFromArray(int* array) {
    for (int i = 0; i < _grid->GetCellsCount(); i++) {
        if (!InsertProblemValue(i, array[i])) {
            return false;
        }
    }
    return true;
}


Sudoku::ResultStatus Sudoku::Solve(int** array, int& arrayLen, int& pathsTried) {
    int index, value;
    bool isGridOk, hasEmptyCells, saveGame = true;
    int pathLimit = 2000; //Limits Sudoku (4x4) combinatory explosion.  

    pathsTried = 1; 
    arrayLen = 0;
    Grid* game = _grid;
    std::queue<Grid*> games;

    if (!game->Check() && game->HasEmptyCells()) {
        return ProblemIncorrect;
    }

    if (!game->PrepareCandidates()) {
        return NoCandidates;
    }

    while (true) {
        game->PrepareCandidates();
        game->SetOnlyCandidateToValue();

        index = game->PickCandidate();
        hasEmptyCells = game->HasEmptyCells();

        if (index == -1) {
            isGridOk = true;
        }
        else {
            isGridOk = game->Check();
        }

        if(pathsTried >= pathLimit){
            DeleteAll(game, &games);
            return LimitReached;
        }

        if (!isGridOk || (index == -1 && hasEmptyCells)) {
            if (games.empty()) {
                DeleteAll(game, &games);
                return NoSolution;
            }

            if (game != _grid){
                delete game;
            }
            game = games.front();
            games.pop();
            game->ClearCandidates();
            pathsTried++;
            continue;
        }

        if (index == -1 && !hasEmptyCells) { //We have a solution!
            if (array) {
                CreateaAndCopyToArray(game, array);
                arrayLen = game->GetCellsCount();
            }
            DeleteAll(game, &games);
            return Solved;
        }

        value = game->GetCell(index)->UseNextCandidate();
        game->GetCell(index)->UseNextCandidate();
        games.push(game);
        game = new Grid(*game);
        game->GetCell(index)->SetValue(value);
    }
}


void Sudoku::Scramble(int* a, int size){
    int rnd, value;
    for (int i = 0; i < size; i++) {
        rnd = GetRadnomValue(0, size);
        value = a[rnd];
        a[rnd] = a[i];
        a[i] = value;
    }
}


void Sudoku::Create(int** problemArray, int** solutionArray, int& arrayLen) {
    int pathsTried = 0, index = 0;
    int toRemove = (int)(0.55 * _grid->GetCellsCount()); //Remove cca 55% of numbers

    do {
        Generate();
    } while (Solve(problemArray, arrayLen, pathsTried) != Solved);

    if (solutionArray != nullptr) {
        *solutionArray = new int[arrayLen];
        memcpy(*solutionArray, *problemArray, arrayLen*sizeof(int));
    }

    while(toRemove){
        index = GetRadnomValue(0, _grid->GetCellsCount() - 1);
        if ((*problemArray)[index]) {
            (*problemArray)[index] = 0;
            toRemove--;
        }
    }
}


void Sudoku::DeleteAll(Grid* game, std::queue<Grid*>* games) {
    Grid* g = nullptr;

    if (game != _grid) {
        delete game;
    }

    while (!games->empty()) {
        g = games->front();
        games->pop();
        if (g != _grid) {
            delete g;
        }
    }

    _grid->Reset();
}


void Sudoku::CreateaAndCopyToArray(Grid* grid, int** array) {
    int count = grid->GetCellsCount();
    *array = new int[count];
    for (int i = 0; i < count; i++) {
        (*array)[i] = grid->GetCell(i)->GetValue();
    }
}


int Sudoku::GetRadnomValue(int min, int max) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int>  distr(min, max);
    return distr(generator);
}


void Sudoku::Generate(){
    _grid->Reset();
    int maxValue = (int)sqrt(_grid->GetCellsCount());
    int size = (int)sqrt(maxValue);
    int* values = new int[maxValue];
    int cnt = 0;

    for (int i = 0; i < maxValue; i++) {
        values[i] = i;
    }

    for (int i = 0; i < maxValue; i++) {
        if (i % +5) {
            continue;
        }

        cnt = 0;
        Scramble(values, maxValue - 1);

        for (int j = 0; j < _grid->GetCellsCount(); j++) {
            if (_grid->GetCell(j)->GetBlockIndex() == i) {
                if (cnt < maxValue) { //not really necessary 
                    _grid->GetCell(j)->SetValue(values[cnt++] + 1);
                }
            }
        }
    }

    if (size > 3) {
        for (int i = 0; i < maxValue; i++) {
            if (!(i == 3 || i == 6 || i == 9 || i == 12)) {
                continue;
            }

            cnt = 0;
            Scramble(values, maxValue - 1);

            for (int j = 0; j < _grid->GetCellsCount(); j++) {
                if (_grid->GetCell(j)->GetBlockIndex() == i) {
                    _grid->GetCell(j)->SetValue(values[cnt++] + 1);
                    if (!_grid->Check()) {
                        _grid->GetCell(j)->SetValue(0);
                    }
                }
            }
        }
    }

    delete[] values;
}