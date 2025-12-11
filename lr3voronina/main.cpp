#include <iostream>
#include <windows.h>
#include "PipelineManager.h"

int main() {
    SetConsoleOutputCP(65001); // UTF-8
    SetConsoleCP(65001); // UTF-8
    
    std::cout << "Программа управления трубопроводом\n";
    
    PipelineManager manager;
    manager.run();
    return 0;
}