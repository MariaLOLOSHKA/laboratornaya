#include "CompressorStation.h"
#include "utils.h"
#include <iostream>

int CompressorStation::nextId = 1;

CompressorStation::CompressorStation() 
    : id(nextId++), name(""), totalWorkshops(0), workingWorkshops(0), stationClass(0) {}

CompressorStation::CompressorStation(int id, const std::string& name, int total, int working, int stationClass)
    : id(id), name(name), totalWorkshops(total), workingWorkshops(working), stationClass(stationClass) {
}

void CompressorStation::readFromConsole() {
    std::cout << "\n=== Ввод данных КС ===\n";
    
    name = InputUtils::getString("Название: ");
    totalWorkshops = InputUtils::getPositiveInt("Количество цехов: ");
    workingWorkshops = InputUtils::getPositiveInt("Количество цехов в работе: ", totalWorkshops);
    stationClass = InputUtils::getPositiveInt("Класс станции: ");
}

void CompressorStation::printToConsole() const {
    std::cout << "ID: " << id << " | КС '" << name << "' | ";
    std::cout << "Цехи: " << workingWorkshops << "/" << totalWorkshops << " | ";
    std::cout << "Класс: " << stationClass << " | ";
    std::cout << "Незадействовано: " << getUnusedPercentage() << "%" << std::endl;
}

void CompressorStation::startWorkshop() {
    if (workingWorkshops < totalWorkshops) {
        workingWorkshops++;
        std::cout << "Запущен цех. Работающих цехов: " << workingWorkshops << "/" << totalWorkshops << std::endl;
    } else {
        std::cout << "Все цехи уже работают!" << std::endl;
    }
}

void CompressorStation::stopWorkshop() {
    if (workingWorkshops > 0) {
        workingWorkshops--;
        std::cout << "Остановлен цех. Работающих цехов: " << workingWorkshops << "/" << totalWorkshops << std::endl;
    } else {
        std::cout << "Нет работающих цехов!" << std::endl;
    }
}

void CompressorStation::saveToFile(std::ofstream& out) const {
    out << "[STATION]\n";
    out << "ID:" << id << "\n";
    out << "NAME:" << name << "\n";
    out << "TOTAL_WORKSHOPS:" << totalWorkshops << "\n";
    out << "WORKING_WORKSHOPS:" << workingWorkshops << "\n";
    out << "CLASS:" << stationClass << "\n";
    out << "[END_STATION]\n";
}

void CompressorStation::loadFromFile(std::ifstream& in) {
    std::string line;
    
    while (std::getline(in, line)) {
        if (line == "[END_STATION]") break;
        
        if (line.find("ID:") == 0) {
            id = std::stoi(line.substr(3));
        } else if (line.find("NAME:") == 0) {
            name = line.substr(5);
        } else if (line.find("TOTAL_WORKSHOPS:") == 0) {
            totalWorkshops = std::stoi(line.substr(16));
        } else if (line.find("WORKING_WORKSHOPS:") == 0) {
            workingWorkshops = std::stoi(line.substr(18));
        } else if (line.find("CLASS:") == 0) {
            stationClass = std::stoi(line.substr(6));
        }
    }
    
}