#include "CompressorStation.h"
#include "utils.h"

CompressorStation::CompressorStation() 
    : name(""), totalWorkshops(0), workingWorkshops(0), stationClass(0) {}

void CompressorStation::readFromConsole() {
    std::cout << "Ввод данных КС:\n";
    
    std::cout << "Название: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    
    totalWorkshops = getPositiveInt("Количество цехов: ");
    
    workingWorkshops = getPositiveInt("Количество цехов в работе: ", totalWorkshops);
    
    stationClass = getPositiveInt("Класс станции: ");
}

void CompressorStation::printToConsole() const {
    std::cout << "КС '" << name << "':\n";
    std::cout << "  Всего цехов: " << totalWorkshops << "\n";
    std::cout << "  Работающих цехов: " << workingWorkshops << "\n";
    std::cout << "  Класс станции: " << stationClass << "\n";
}

void CompressorStation::startWorkshop() {
    if (workingWorkshops < totalWorkshops) {
        workingWorkshops++;
        std::cout << "Запущен цех. Работающих цехов: " << workingWorkshops << "/" << totalWorkshops << "\n";
    } else {
        std::cout << "Все цехи уже работают!\n";
    }
}

void CompressorStation::stopWorkshop() {
    if (workingWorkshops > 0) {
        workingWorkshops--;
        std::cout << "Остановлен цех. Работающих цехов: " << workingWorkshops << "/" << totalWorkshops << "\n";
    } else {
        std::cout << "Нет работающих цехов!\n";
    }
}

void CompressorStation::saveToFile(std::ofstream& out) const {
    out << name << "\n"
        << totalWorkshops << "\n"
        << workingWorkshops << "\n"
        << stationClass << "\n";
}

void CompressorStation::loadFromFile(std::ifstream& in) {
    in >> std::ws;
    std::getline(in, name);
    in >> totalWorkshops >> workingWorkshops >> stationClass;
}

std::string CompressorStation::getName() const {
    return name;
}

int CompressorStation::getWorkingWorkshops() const {
    return workingWorkshops;
}

int CompressorStation::getTotalWorkshops() const {
    return totalWorkshops;
}