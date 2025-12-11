#include "Pipe.h"
#include "utils.h"
#include <iostream>

// Инициализация статической переменной
int Pipe::nextId = 1;

Pipe::Pipe() : id(nextId++), name(""), length(0.0), diameter(0), underRepair(false) {}

Pipe::Pipe(int id, const std::string& name, double length, int diameter, bool repair)
    : id(id), name(name), length(length), diameter(diameter), underRepair(repair) {
}

void Pipe::readFromConsole() {
    std::cout << "\n=== Ввод данных трубы ===\n";
    
    name = InputUtils::getString("Километровая отметка (название): ");
    length = InputUtils::getPositiveDouble("Длина (в км): ");
    diameter = InputUtils::getPositiveInt("Диаметр (в мм): ");
    
    underRepair = false;
}

void Pipe::printToConsole() const {
    std::cout << "ID: " << id << " | Труба '" << name << "' | ";
    std::cout << length << " км | " << diameter << " мм | ";
    std::cout << "Статус: " << (underRepair ? "В ремонте" : "Работает") << std::endl;
}

void Pipe::toggleRepair() {
    underRepair = !underRepair;
    std::cout << "Труба '" << name << "' теперь " 
              << (underRepair ? "в ремонте" : "в работе") << std::endl;
}

void Pipe::saveToFile(std::ofstream& out) const {
    out << "[PIPE]\n";
    out << "ID:" << id << "\n";
    out << "NAME:" << name << "\n";
    out << "LENGTH:" << length << "\n";
    out << "DIAMETER:" << diameter << "\n";
    out << "REPAIR:" << underRepair << "\n";
    out << "[END_PIPE]\n";
}

void Pipe::loadFromFile(std::ifstream& in) {
    std::string line;
    
    while (std::getline(in, line)) {
        if (line == "[END_PIPE]") break;
        
        if (line.find("ID:") == 0) {
            id = std::stoi(line.substr(3));
        } else if (line.find("NAME:") == 0) {
            name = line.substr(5);
        } else if (line.find("LENGTH:") == 0) {
            length = std::stod(line.substr(7));
        } else if (line.find("DIAMETER:") == 0) {
            diameter = std::stoi(line.substr(9));
        } else if (line.find("REPAIR:") == 0) {
            underRepair = std::stoi(line.substr(7));
        }
    }
    
}