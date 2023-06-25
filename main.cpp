#include "Cli.h"
#include <windows.h>

#ifdef _WIN32
    #include "windows.h"
#elif __linux__
    #include "time.h"
#else 
    #error "OS not supported"
#endif 

int get_byte_array(byte **problemAndSolution){
    Sudoku* s = new Sudoku();
    int *problem, *solution, len = 0;

    s->Create(&problem, &solution, len);

    (*problemAndSolution) = new byte[len * 2];

    for(int i=0; i<len; i++){
        (*problemAndSolution)[i] = (byte)problem[i];
        (*problemAndSolution)[i + (len)] = (byte)solution[i];
    }

    delete[] problem;
    delete[] solution;
    delete s;

    return len*2;
}

#ifdef _WIN32

int interop(const std::string name, const byte *data, const int dataSize) {
    HANDLE hMapFile;
    byte* pData;
    std::string memName = "sudoku_mm_" + name;
    //std::string semName = "sudoku_sem_" + name;

    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, memName.c_str());
    if (hMapFile == NULL) {
        return 2;
    }

    pData = (byte*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, dataSize);

    if (pData == NULL) {
        CloseHandle(hMapFile);
        return 3;
    }

    /*
    hSem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, semName.c_str());
    if (hSem == NULL) {
        CloseHandle(hMapFile);
        return -3;
    }
    */

    ZeroMemory(pData, dataSize);
    CopyMemory(pData, data, dataSize);

    //ReleaseSemaphore(hSem, 1, NULL);
    UnmapViewOfFile(pData);
    CloseHandle(hMapFile);
    //CloseHandle(hSem);

    return 0;
}

#elif __linux__

int interop(const std::string name, const byte* data, const int dataSize) {
    return 1;
}

#endif


//Main
int main(int argc, char* argv[]) {
    if (argc != 2 || strlen(argv[1]) <= 0) {
        Cli cli;
        cli.MainMenu();
        return 0;
    }
    else {
        byte* interProcData;
        int interProcDataSize, retValue;
        std::string name(argv[1]);

        interProcDataSize = get_byte_array(&interProcData);
        retValue = interop(name, interProcData, interProcDataSize);
        delete[] interProcData;
        return retValue;
    }
}