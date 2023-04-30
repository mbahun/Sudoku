#include "Cli.h"


void Cli::MainMenu() {
    Grid::Size sudokuSize;

    while (1) {
        CLEAR_SCREEN;
        std::cout << "Please, select Sudoku size:" << std::endl;
        std::cout << " <1> - Small (4x4)" << std::endl;
        std::cout << " <2> - Standard (9x9)" << std::endl;
        std::cout << " <3> - Big (16x16)" << std::endl;

        switch (ReadUserInput()) {
        case 1:
            sudokuSize = Grid::Small;
            break;
        case 2:
            sudokuSize = Grid::Normal;
            break;
        case 3:
            sudokuSize = Grid::Big;
            break;
        default:
            continue;
            break;
        }
        
        while (MenuSelectAction(sudokuSize)){}
    }
}

int Cli::MenuSelectAction(Grid::Size size) {
    CLEAR_SCREEN;
    std::cout << "What do you want to do?" << std::endl;
    std::cout << " <1> - Solve a Sudoku problem" << std::endl;
    std::cout << " <2> - Create a Sudoku problem" << std::endl;
    std::cout << " <3> - Change Sudoku size" << std::endl;
    std::cout << " <4> - Exit" << std::endl;

    switch (ReadUserInput()) {
    case 1:
        MenuSolve(size);
        break;
    case 2:
        MenuCreate(size);
        break;
    case 3:
        MainMenu();
        return 0;
    case 4:
        exit(1);
    default:
        return 1;
    }

    std::cout << "Press enter to continue...";
    ReadUserInput(true);
    return 1;
}


void Cli::MenuSolve(Grid::Size size) {
    CLEAR_SCREEN;
    std::cout << "Select:" << std::endl;
    std::cout << " <1> - Input Sudoku data" << std::endl;
    std::cout << " <2> - Use Sudoku test data" << std::endl;

    _sudoku = new Sudoku((Grid::Size)size);

    switch (ReadUserInput()) {
    case 1:
        InputValues(size);
        break;
    case 2:
        InputTestValues(size);
        break;
    default:
        delete _sudoku;
        std::cout << "Wrong input..." << std::endl;
        return;
    }

    std::cout << "This is the solution:" << std::endl;
    SudokuSolve(size);
    delete _sudoku;
}


int Cli::ReadUserInput(bool justEnter) {
    int menuItem = 0;

    if (justEnter) {
        std::cin.get();
        return 0;
    }
    else {
        std::cin >> menuItem;
    }

    std::cin.clear();
    std::cin.ignore(1000, '\n');
    return menuItem;
}


void Cli::MenuCreate(Grid::Size size) {
    CLEAR_SCREEN;
    if (size>Grid::Normal){
        std::cout << "This will take some time..." << std::endl; 
    }
    SudokuCreate(size);
}


void Cli::SudokuSolve(Grid::Size size) {
    SudokuResult result;
    Sudoku::ResultStatus ret = SudokuSolveAndMeasureTime(&result);
    
    switch (ret) {
    case Sudoku::ResultStatus::ProblemIncorrect:
        std::cout << " ->Problem not set up correctly!" << std::endl;
        return;
    case Sudoku::ResultStatus::NoCandidates:
        std::cout << " Couldn't find any candidates to start solving!" << std::endl;
        return;
    case Sudoku::ResultStatus::NoSolution:
        std::cout << " Couldn't find the solution!" << std::endl;
        return;
    case Sudoku::ResultStatus::LimitReached:
        std::cout << " Couldn't find the solution within the limit..." << std::endl;
        return;
    case Sudoku::ResultStatus::Solved:
        Print(result.array, result.arrayLen, size);
        std::cout << std::endl << "Paths tried: " << result.combinationsTried << std::endl;
        std::cout << "Elapsed time: " << result.seconds << " seconds." << std::endl;
        if (result.arrayLen) {
            delete[] result.array;
        }
        return;
    }
}



#ifdef _WIN32

Sudoku::ResultStatus Cli::SudokuSolveAndMeasureTime(SudokuResult* result){
    LARGE_INTEGER frequency, begin, end;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&begin);
    Sudoku::ResultStatus ret = _sudoku->Solve(&result->array, result->arrayLen, result->combinationsTried);
    QueryPerformanceCounter(&end);

    result->seconds = (float)(end.QuadPart - begin.QuadPart) / frequency.QuadPart;

    return ret;
}

#elif __linux__

Sudoku::ResultStatus Cli::SudokuSolveAndMeasureTime(SudokuResult* result){
    long secs = 0;
    struct timespec begin, end;

    clock_gettime(CLOCK_REALTIME, &begin);
    Sudoku::ResultStatus ret = _sudoku->Solve(&result->array, result->arrayLen, result->combinationsTried);
    clock_gettime(CLOCK_REALTIME, &end);

    secs = end.tv_sec - begin.tv_sec;
    result->seconds = secs + ((end.tv_nsec - begin.tv_nsec) * 1e-9);

    return ret;
}

#endif


void Cli::SudokuCreate(Grid::Size size) {
    int* array, arrayLen = 0;
    _sudoku = new Sudoku((Grid::Size)size);

    _sudoku->Create(&array, arrayLen);
    CLEAR_SCREEN;
    Print(array, arrayLen, size);
    delete[] array;
    delete _sudoku;
}


void Cli::Print(int* array, int arrayLen, Grid::Size size) {
    int maxDigit = size * size;
    int spaceEvery = size;

    for (int i = 1; i <= arrayLen; i++) {
        if (maxDigit > 9)
            std::cout << std::setfill('0') << std::setw(2) << array[i - 1] << " ";
        else
            std::cout << array[i - 1] << " ";

        if (i % spaceEvery == 0) {
            std::cout << "  ";
        }

        if (i % maxDigit == 0) {
            std::cout << std::endl;
        }

        if (i % (maxDigit * spaceEvery) == 0) {
            std::cout << std::endl;
        }
    }
}


void Cli::InputValues(Grid::Size size) {
    int numbersPerLine = size * size;
    int totalNumbers = numbersPerLine * numbersPerLine;

    int input;
    int *array = new int[totalNumbers];

    CLEAR_SCREEN;

    std::cout << "Please enter numbers to fill " << size << "x" << size << " Sudoku, line by line." << std::endl;

    for (int i = 0; i < numbersPerLine; i++) {
        if (i > 0) CLEAR_SCREEN;
        std::cout << "Input " << numbersPerLine << " numbers for line " << i + 1 << "!" << std::endl;

        for (int j = 0; j < numbersPerLine; j++) {
            std::cout << std::setfill('0') << std::setw(2) << " Line " << i + 1 << ", number " << j + 1 << ": ";
            do {
                input = ReadUserInput();
                if (input > numbersPerLine) {
                    std::cout << "Wrong Choice. Enter again: ";
                }
                else {
                    array[i * numbersPerLine + j] = input;
                    break;
                }
            } while (1);
        }
    }

    _sudoku->InsertProblemValuesFromArray(array);

    CLEAR_SCREEN;
    std::cout << "This is the problem:" << std::endl;
    Print(array, totalNumbers, size);
    delete[] array;
}


void Cli::InputTestValues(Grid::Size size) {
    CLEAR_SCREEN;
    std::cout << "This is the problem:" << std::endl;

    switch (size) {
    case Grid::Size::Small:
        UseTestValues2x2();
        break;
    case Grid::Size::Normal:
        UseTestValues3x3();
        break;
    case Grid::Size::Big:
        UseTestValues4x4();
    }
}


void Cli::UseTestValues2x2() {
    //int values[] = {
    //    0,0,  0,0,
    //    0,0,  0,0,

    //    0,0,  0,0,
    //    0,0,  0,0
    //};

    int values[] = {
        0,3,  0,0,
        1,0,  0,0,

        0,0,  2,0,
        2,1,  0,0
    };

    if (!_sudoku->InsertProblemValuesFromArray(values)) {
        std::cout << "Wrong test data! " << std::endl;
    }
    Print(values, 16, Grid::Size::Small);
}


void Cli::UseTestValues3x3() {
    //int values[] = {
    //    0,0,0,  0,0,0,  0,0,0,
    //    0,0,0,  0,0,0,  0,0,0,
    //    0,0,0,  0,0,0,  0,0,0,

    //    0,0,0,  0,0,0,  0,0,0,
    //    0,0,0,  0,0,0,  0,0,0,
    //    0,0,0,  0,0,0,  0,0,0,

    //    0,0,0,  0,0,0,  0,0,0,
    //    0,0,0,  0,0,0,  0,0,0,
    //    0,0,0,  0,0,0,  0,0,0
    //};


    int values[] = {
        0,0,0,  2,0,0,  0,0,0,
        9,6,0,  0,0,5,  0,2,0,
        7,0,0,  0,0,0,  8,0,9,

        0,0,0,  0,6,0,  9,4,0,
        0,5,0,  1,0,8,  0,3,0,
        0,4,6,  0,9,0,  0,0,0,

        8,0,3,  0,0,0,  0,0,1,
        0,7,0,  8,0,0,  0,9,3,
        0,0,0,  0,0,1,  0,0,0
    };

    if (!_sudoku->InsertProblemValuesFromArray(values)) {
        std::cout << "Wrong test data: " << std::endl;
    }
    Print(values, 81, Grid::Size::Normal);
}


void Cli::UseTestValues4x4() {
    //int values[] = {
    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,

    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,

    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,

    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
    //     0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0
    //};

    int values[] = {
         1,  2,  3,  4,   5,  6,  7,  8,   9, 10, 11, 12,  13, 14, 15, 16,
        13, 14, 15, 16,   1,  2,  3,  4,   5,  6,  7,  8,   9, 10, 11, 12,
         9, 10, 11, 12,  13, 14, 15, 16,   1,  2,  3,  4,   5,  6,  7,  8,
         5,  6,  7,  8,   9, 10, 11, 12,  13, 14, 15, 16,   1,  2,  3,  4,

        16,  1,  2,  3,   4,  5,  6,  7,   8,  9, 10, 11,  12, 13, 14, 15,
        12, 13, 14, 15,  16,  1,  2,  3,   4,  5,  6,  7,   8,  9, 10, 11,
         8,  9, 10, 11,  12, 13, 14, 15,  16,  1,  2,  3,   4,  5,  6,  7,
         4,  5,  6,  7,   8,  9, 10, 11,  12, 13, 14, 15,  16,  1,  2,  3,

        15, 16,  1,  2,   3,  4,  5,  6,   7,  8,  9, 10,  11, 12, 13, 14,
        11, 12, 13, 14,  15, 16,  1,  2,   3,  4,  5,  6,   7,  8,  9, 10,
         7,  8,  9, 10,  11, 12, 13, 14,  15, 16,  1,  2,   3,  4,  5,  6,
         3,  4,  5,  6,   7,  8,  9, 10,  11, 12, 13, 14,  15, 16,  1,  2,

         0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
         0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
         0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,
         6,  7,  8,  9,  10, 11, 12, 13,  14, 15, 16,  1,   2,  3,  4,  5
    };

    if (!_sudoku->InsertProblemValuesFromArray(values)) {
        std::cout << "Wrong test data! " << std::endl;
    }
    Print(values, 256, Grid::Size::Big);
}




