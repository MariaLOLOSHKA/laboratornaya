#include <iostream>
#include <fstream>
#include <string>
#include "Pipe.h"
#include "CompressorStation.h"
#include "utils.h"

// Глобальные переменные для хранения данных
Pipe pipe;
CompressorStation station;
bool pipeExists = false;
bool stationExists = false;

void printMenu() {
    std::cout << "\n=== Меню управления ===\n";
    std::cout << "1. Добавить трубу\n";
    std::cout << "2. Добавить КС\n";
    std::cout << "3. Просмотр всех объектов\n";
    std::cout << "4. Редактировать трубу\n";
    std::cout << "5. Редактировать КС\n";
    std::cout << "6. Сохранить\n";
    std::cout << "7. Загрузить\n";
    std::cout << "8. Выход\n";
    std::cout << "Выберите действие: ";
}

void addPipe() {
    pipe.readFromConsole();
    pipeExists = true;
    std::cout << "Труба успешно добавлена!\n";
}

void addStation() {
    station.readFromConsole();
    stationExists = true;
    std::cout << "КС успешно добавлена!\n";
}

void viewAll() {
    if (pipeExists) {
        std::cout << "\n";
        pipe.printToConsole();
    } else {
        std::cout << "Труба не добавлена.\n";
    }
    
    if (stationExists) {
        std::cout << "\n";
        station.printToConsole();
    } else {
        std::cout << "КС не добавлена.\n";
    }
}

void editPipe() {
    if (!pipeExists) {
        std::cout << "Сначала добавьте трубу!\n";
        return;
    }
    
    pipe.toggleRepair();
}

void editStation() {
    if (!stationExists) {
        std::cout << "Сначала добавьте КС!\n";
        return;
    }
    
    std::cout << "\nРедактирование КС:\n";
    std::cout << "1. Запустить цех\n";
    std::cout << "2. Остановить цех\n";
    std::cout << "Выберите действие: ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            station.startWorkshop();
            break;
        case 2:
            station.stopWorkshop();
            break;
        default:
            std::cout << "Неверный выбор!\n";
    }
}

void saveData() {
    std::string filename;
    std::cout << "Введите имя файла для сохранения: ";
    std::cin >> filename;
    
    std::ofstream out(filename);
    if (!out) {
        std::cout << "Ошибка открытия файла!\n";
        return;
    }
    
    // Сохраняем флаги существования объектов
    out << pipeExists << "\n";
    out << stationExists << "\n";
    
    // Сохраняем данные объектов, если они существуют
    if (pipeExists) {
        pipe.saveToFile(out);
    }
    if (stationExists) {
        station.saveToFile(out);
    }
    
    out.close();
    std::cout << "Данные сохранены в файл '" << filename << "'\n";
}

void loadData() {
    std::string filename;
    std::cout << "Введите имя файла для загрузки: ";
    std::cin >> filename;
    
    std::ifstream in(filename);
    if (!in) {
        std::cout << "Ошибка открытия файла!\n";
        return;
    }
    
    // Загружаем флаги существования объектов
    in >> pipeExists;
    in >> stationExists;
    
    // Загружаем данные объектов, если они существуют
    if (pipeExists) {
        pipe.loadFromFile(in);
    }
    if (stationExists) {
        station.loadFromFile(in);
    }
    
    in.close();
    std::cout << "Данные загружены из файла '" << filename << "'\n";
}

int main() {
    int choice;
    
    std::cout << "Программа управления трубой и КС\n";
    
    while (true) {
        printMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                addPipe();
                break;
            case 2:
                addStation();
                break;
            case 3:
                viewAll();
                break;
            case 4:
                editPipe();
                break;
            case 5:
                editStation();
                break;
            case 6:
                saveData();
                break;
            case 7:
                loadData();
                break;
            case 8:
                std::cout << "Выход из программы.\n";
                return 0;
            default:
                std::cout << "Неверный выбор! Попробуйте снова.\n";
        }
        
        pause();
        clearScreen();
    }
    
    return 0;
}