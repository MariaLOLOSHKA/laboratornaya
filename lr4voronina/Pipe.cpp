#include "Pipe.h"
#include "utils.h"
#include <iostream>
#include <cmath>
#include <map> // Добавляем include
#include <limits>

int Pipe::nextId = 1;

// Производительность труб в зависимости от диаметра (м³/час)
const std::map<int, double> Pipe::PIPE_CAPACITIES = { // Добавляем Pipe::
    {500, 1000.0},
    {700, 2500.0},
    {1000, 6000.0},
    {1400, 15000.0}
};

// Поправочный коэффициент для формулы
const double Pipe::CAPACITY_COEFFICIENT = 0.8; // Добавляем Pipe::

Pipe::Pipe() : id(nextId++), name(""), length(0.0), diameter(0), underRepair(false) {}

Pipe::Pipe(int id, const std::string& name, double length, int diameter, bool repair)
    : id(id), name(name), length(length), diameter(diameter), underRepair(repair) {}

double Pipe::getCapacity() const {
    if (underRepair) {
        return 0.0; // Труба в ремонте - производительность 0
    }
    
    auto it = PIPE_CAPACITIES.find(diameter);
    if (it != PIPE_CAPACITIES.end()) {
        // Упрощенная формула: производительность зависит от диаметра и длины
        // Q = Q_base * (1 / sqrt(length)) * коэффициент
        return it->second * (1.0 / std::sqrt(length)) * CAPACITY_COEFFICIENT;
    }
    return 0.0;
}

double Pipe::getWeight() const {
    if (underRepair) {
        return std::numeric_limits<double>::infinity(); // Труба в ремонте - бесконечный вес
    }
    return length; // Вес = длина трубы (для кратчайшего пути)
}

void Pipe::readFromConsole() {
    std::cout << "\n=== Ввод данных трубы ===\n";
    
    name = InputUtils::getString("Километровая отметка (название): ");
    length = InputUtils::getPositiveDouble("Длина (в км): ");
    
    // Ограничиваем диаметры допустимыми значениями
    std::cout << "Допустимые диаметры: 500, 700, 1000, 1400 мм\n";
    while (true) {
        diameter = InputUtils::getPositiveInt("Диаметр (в мм): ");
        if (PIPE_CAPACITIES.find(diameter) != PIPE_CAPACITIES.end()) {
            break;
        }
        std::cout << "Недопустимый диаметр! Используйте один из допустимых значений.\n";
    }
    
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
    
    // Обновляем nextId при загрузке
    if (id >= nextId) {
        nextId = id + 1;
    }
}