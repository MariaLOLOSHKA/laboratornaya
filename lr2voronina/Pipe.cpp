#include "Pipe.h"
#include "utils.h"

Pipe::Pipe() : name(""), length(0.0), diameter(0), underRepair(false) {}

void Pipe::readFromConsole() {
    std::cout << "Ввод данных трубы:\n";
    
    std::cout << "Километровая отметка (название): ";
    std::cin.ignore();
    std::getline(std::cin, name);
    
    length = getPositiveDouble("Длина (в км): ");
    diameter = getPositiveInt("Диаметр (в мм): ");
    
    underRepair = false; // по умолчанию не в ремонте
}

void Pipe::printToConsole() const {
    std::cout << "Труба '" << name << "':\n";
    std::cout << "  Длина: " << length << " км\n";
    std::cout << "  Диаметр: " << diameter << " мм\n";
    std::cout << "  Статус: " << (underRepair ? "В ремонте" : "Работает") << "\n";
}

void Pipe::toggleRepair() {
    underRepair = !underRepair;
    std::cout << "Труба '" << name << "' теперь " 
              << (underRepair ? "в ремонте" : "в работе") << "\n";
}

void Pipe::saveToFile(std::ofstream& out) const {
    out << name << "\n"
        << length << "\n"
        << diameter << "\n"
        << underRepair << "\n";
}

void Pipe::loadFromFile(std::ifstream& in) {
    in >> std::ws;
    std::getline(in, name);
    in >> length >> diameter >> underRepair;
}

std::string Pipe::getName() const {
    return name;
}

bool Pipe::isUnderRepair() const {
    return underRepair;
}