#include "PipelineManager.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>

void PipelineManager::resetCounters() {
    Pipe::setNextId(1);
    CompressorStation::setNextId(1);
}

void PipelineManager::printMenu() const {
    std::cout << "\n=== Меню управления трубопроводом ===\n";
    std::cout << "1. Добавить трубу\n";
    std::cout << "2. Добавить КС\n";
    std::cout << "3. Просмотр всех объектов\n";
    std::cout << "4. Редактировать трубу\n";
    std::cout << "5. Редактировать КС\n";
    std::cout << "6. Удалить объект\n";
    std::cout << "7. Поиск объектов\n";
    std::cout << "8. Пакетное редактирование труб\n";
    std::cout << "9. Соединить КС\n";
    std::cout << "10. Разорвать соединение\n";
    std::cout << "11. Просмотр сети\n";
    std::cout << "12. Топологическая сортировка\n";
    std::cout << "13. Максимальный поток\n";
    std::cout << "14. Кратчайший путь\n";
    std::cout << "15. Сохранить\n";
    std::cout << "16. Загрузить\n";
    std::cout << "17. Выход\n";
    std::cout << "Выберите действие: ";
}
void PipelineManager::addPipe() {
    Pipe pipe;
    pipe.readFromConsole();
    pipes[pipe.getId()] = pipe;
    
    logger.log("Добавлена труба", "ID: " + std::to_string(pipe.getId()) + ", Название: " + pipe.getName());
    std::cout << "Труба успешно добавлена! ID: " << pipe.getId() << std::endl;
}

void PipelineManager::addStation() {
    CompressorStation station;
    station.readFromConsole();
    stations[station.getId()] = station;
    
    logger.log("Добавлена КС", "ID: " + std::to_string(station.getId()) + ", Название: " + station.getName());
    std::cout << "КС успешно добавлена! ID: " << station.getId() << std::endl;
}

void PipelineManager::printAllPipes() const {
    if (pipes.empty()) {
        std::cout << "Трубы не добавлены.\n";
        return;
    }
    
    std::cout << "\n=== Список труб ===\n";
    for (const auto& [id, pipe] : pipes) {
        pipe.printToConsole();
    }
}

void PipelineManager::printAllStations() const {
    if (stations.empty()) {
        std::cout << "КС не добавлены.\n";
        return;
    }
    
    std::cout << "\n=== Список КС ===\n";
    for (const auto& [id, station] : stations) {
        station.printToConsole();
    }
}

void PipelineManager::viewAll() const {
    printAllPipes();
    printAllStations();
}

void PipelineManager::editPipe() {
    printAllPipes();
    if (pipes.empty()) return;
    
    int id = InputUtils::getPositiveInt("Введите ID трубы для редактирования: ");
    auto it = pipes.find(id);
    if (it != pipes.end()) {
        it->second.toggleRepair();
        logger.log("Изменен статус трубы", "ID: " + std::to_string(id) + 
                    ", Новый статус: " + std::string(it->second.isUnderRepair() ? "В ремонте" : "Работает"));
    } else {
        std::cout << "Труба с ID " << id << " не найдена!\n";
    }
}

void PipelineManager::editStation() {
    printAllStations();
    if (stations.empty()) return;
    
    int id = InputUtils::getPositiveInt("Введите ID КС для редактирования: ");
    auto it = stations.find(id);
    if (it != stations.end()) {
        std::cout << "\nРедактирование КС:\n";
        std::cout << "1. Запустить цех\n";
        std::cout << "2. Остановить цех\n";
        std::cout << "Выберите действие: ";
        
        int choice = InputUtils::getPositiveInt("", 2);
        
        int oldWorking = it->second.getWorkingWorkshops();
        if (choice == 1) {
            it->second.startWorkshop();
        } else {
            it->second.stopWorkshop();
        }
        
        logger.log("Изменено количество цехов КС", 
                  "ID: " + std::to_string(id) + ", Было: " + std::to_string(oldWorking) + 
                  ", Стало: " + std::to_string(it->second.getWorkingWorkshops()));
    } else {
        std::cout << "КС с ID " << id << " не найдена!\n";
    }
}

void PipelineManager::deletePipe(int id) {
    auto it = pipes.find(id);
    if (it != pipes.end()) {
        logger.log("Удалена труба", "ID: " + std::to_string(id) + ", Название: " + it->second.getName());
        pipes.erase(it);
        std::cout << "Труба с ID " << id << " удалена.\n";
    }
}

void PipelineManager::deleteStation(int id) {
    auto it = stations.find(id);
    if (it != stations.end()) {
        logger.log("Удалена КС", "ID: " + std::to_string(id) + ", Название: " + it->second.getName());
        stations.erase(it);
        std::cout << "КС с ID " << id << " удалена.\n";
    }
}

void PipelineManager::deleteObject() {
    std::cout << "\n1. Удалить трубу\n";
    std::cout << "2. Удалить КС\n";
    int choice = InputUtils::getPositiveInt("Выберите тип объекта: ", 2);
    
    if (choice == 1) {
        printAllPipes();
        if (!pipes.empty()) {
            int id = InputUtils::getPositiveInt("Введите ID трубы для удаления: ");
            deletePipe(id);
        }
    } else {
        printAllStations();
        if (!stations.empty()) {
            int id = InputUtils::getPositiveInt("Введите ID КС для удаления: ");
            deleteStation(id);
        }
    }
}

std::vector<int> PipelineManager::findPipesByName(const std::string& name) const {
    std::vector<int> result;
    for (const auto& [id, pipe] : pipes) {
        if (pipe.getName().find(name) != std::string::npos) {
            result.push_back(id);
        }
    }
    return result;
}

std::vector<int> PipelineManager::findPipesByRepairStatus(bool underRepair) const {
    std::vector<int> result;
    for (const auto& [id, pipe] : pipes) {
        if (pipe.isUnderRepair() == underRepair) {
            result.push_back(id);
        }
    }
    return result;
}

std::vector<int> PipelineManager::findStationsByName(const std::string& name) const {
    std::vector<int> result;
    for (const auto& [id, station] : stations) {
        if (station.getName().find(name) != std::string::npos) {
            result.push_back(id);
        }
    }
    return result;
}

std::vector<int> PipelineManager::findStationsByUnusedPercentage(double minPercent, double maxPercent) const {
    std::vector<int> result;
    for (const auto& [id, station] : stations) {
        double unused = station.getUnusedPercentage();
        if (unused >= minPercent && unused <= maxPercent) {
            result.push_back(id);
        }
    }
    return result;
}

std::vector<int> PipelineManager::findFreePipesByDiameter(int diameter) const {
    std::vector<int> result;
    
    // Собираем все трубы, которые уже используются в сети
    std::set<int> usedPipes;
    for (const auto& conn : network.getAllConnections()) {
        usedPipes.insert(conn.pipeId);
    }
    
    // Ищем свободные трубы нужного диаметра
    for (const auto& [id, pipe] : pipes) {
        if (usedPipes.find(id) == usedPipes.end() && 
            pipe.getDiameter() == diameter && 
            !pipe.isUnderRepair()) {
            result.push_back(id);
        }
    }
    
    return result;
}

void PipelineManager::findObjects() {
    std::cout << "\n=== Поиск объектов ===\n";
    std::cout << "1. Найти трубы по названию\n";
    std::cout << "2. Найти трубы по статусу ремонта\n";
    std::cout << "3. Найти КС по названию\n";
    std::cout << "4. Найти КС по проценту незадействованных цехов\n";
    
    int choice = InputUtils::getPositiveInt("Выберите тип поиска: ", 4);
    
    std::vector<int> foundIds;
    
    switch (choice) {
        case 1: {
            std::string name = InputUtils::getString("Введите часть названия: ");
            foundIds = findPipesByName(name);
            std::cout << "\nНайденные трубы:\n";
            for (int id : foundIds) {
                pipes.at(id).printToConsole();
            }
            break;
        }
        case 2: {
            bool status = InputUtils::getYesNo("Искать трубы в ремонте?");
            foundIds = findPipesByRepairStatus(status);
            std::cout << "\nНайденные трубы:\n";
            for (int id : foundIds) {
                pipes.at(id).printToConsole();
            }
            break;
        }
        case 3: {
            std::string name = InputUtils::getString("Введите часть названия: ");
            foundIds = findStationsByName(name);
            std::cout << "\nНайденные КС:\n";
            for (int id : foundIds) {
                stations.at(id).printToConsole();
            }
            break;
        }
        case 4: {
            double minPercent = InputUtils::getPositiveDouble("Введите минимальный процент незадействованных цехов: ");
            double maxPercent = 100;
            if (InputUtils::getYesNo("Указать максимальный процент?")) {
                maxPercent = InputUtils::getPositiveDouble("Введите максимальный процент: ");
            }
            foundIds = findStationsByUnusedPercentage(minPercent, maxPercent);
            std::cout << "\nНайденные КС:\n";
            for (int id : foundIds) {
                stations.at(id).printToConsole();
            }
            break;
        }
    }
    
    logger.log("Выполнен поиск объектов", "Тип поиска: " + std::to_string(choice) + 
               ", Найдено объектов: " + std::to_string(foundIds.size()));
}

void PipelineManager::batchEditPipes(const std::vector<int>& pipeIds) {
    for (int id : pipeIds) {
        pipes.at(id).toggleRepair();
    }
    logger.log("Пакетное редактирование труб", "Обработано труб: " + std::to_string(pipeIds.size()));
}

void PipelineManager::selectiveEditPipes(const std::vector<int>& pipeIds) {
    std::vector<int> selectedIds;
    
    std::cout << "\nВыберите трубы для редактирования:\n";
    for (int id : pipeIds) {
        std::cout << "ID " << id << ": ";
        pipes.at(id).printToConsole();
        if (InputUtils::getYesNo("Редактировать эту трубу?")) {
            selectedIds.push_back(id);
        }
    }
    
    if (!selectedIds.empty()) {
        batchEditPipes(selectedIds);
    }
}

void PipelineManager::batchEdit() {
    std::cout << "\n=== Пакетное редактирование труб ===\n";
    std::cout << "1. Найти трубы для редактирования\n";
    std::cout << "2. Редактировать все трубы\n";
    
    int choice = InputUtils::getPositiveInt("Выберите действие: ", 2);
    
    std::vector<int> pipeIds;
    
    if (choice == 1) {
        std::cout << "\nКритерии поиска:\n";
        std::cout << "1. По названию\n";
        std::cout << "2. По статусу ремонта\n";
        
        int searchType = InputUtils::getPositiveInt("Выберите критерий: ", 2);
        
        if (searchType == 1) {
            std::string name = InputUtils::getString("Введите часть названия: ");
            pipeIds = findPipesByName(name);
        } else {
            bool status = InputUtils::getYesNo("Искать трубы в ремонте?");
            pipeIds = findPipesByRepairStatus(status);
        }
    } else {
        for (const auto& [id, pipe] : pipes) {
            pipeIds.push_back(id);
        }
    }
    
    if (pipeIds.empty()) {
        std::cout << "Трубы не найдены.\n";
        return;
    }
    
    std::cout << "\nНайдено труб: " << pipeIds.size() << std::endl;
    
    if (InputUtils::getYesNo("Редактировать все найденные трубы?")) {
        batchEditPipes(pipeIds);
    } else {
        selectiveEditPipes(pipeIds);
    }
}

void PipelineManager::connectStations() {
    if (stations.size() < 2) {
        std::cout << "Для соединения нужно как минимум 2 КС!\n";
        return;
    }
    
    printAllStations();
    
    int startId = InputUtils::getPositiveInt("Введите ID КС входа: ");
    int endId = InputUtils::getPositiveInt("Введите ID КС выхода: ");
    
    if (startId == endId) {
        std::cout << "КС входа и выхода не могут быть одинаковыми!\n";
        return;
    }
    
    if (stations.find(startId) == stations.end()) {
        std::cout << "КС с ID " << startId << " не найдена!\n";
        return;
    }
    
    if (stations.find(endId) == stations.end()) {
        std::cout << "КС с ID " << endId << " не найдена!\n";
        return;
    }
    
    // Проверяем допустимые диаметры
    const std::vector<int> allowedDiameters = {500, 700, 1000, 1400};
    
    // ПОКАЗЫВАЕМ ВСЕ СВОБОДНЫЕ ТРУБЫ С ПОДХОДЯЩИМ ДИАМЕТРОМ
    std::cout << "\n=== Свободные трубы с допустимыми диаметрами (500, 700, 1000, 1400 мм) ===\n";
    std::set<int> usedPipes;
    for (const auto& conn : network.getAllConnections()) {
        usedPipes.insert(conn.pipeId);
    }
    
    std::vector<int> availablePipes;
    for (const auto& [id, pipe] : pipes) {
        if (usedPipes.find(id) == usedPipes.end() && 
            !pipe.isUnderRepair() &&
            std::find(allowedDiameters.begin(), allowedDiameters.end(), pipe.getDiameter()) != allowedDiameters.end()) {
            availablePipes.push_back(id);
            pipe.printToConsole();
        }
    }
    
    if (availablePipes.empty()) {
        std::cout << "Свободных труб с допустимыми диаметрами не найдено.\n";
        std::cout << "1. Создать новую трубу\n";
        std::cout << "2. Отмена\n";
        int choice = InputUtils::getPositiveInt("Выберите действие: ", 2);
        
        if (choice == 1) {
            // Создаем новую трубу с выбором диаметра
            int pipeId = createPipeWithAllowedDiameter();
            if (pipeId == -1) {
                return; // Пользователь отменил создание
            }
            
            try {
                int diameter = pipes[pipeId].getDiameter();
                network.addConnection(pipeId, startId, endId, diameter);
                
                logger.log("Создано соединение", 
                          "КС " + std::to_string(startId) + " -> КС " + std::to_string(endId) + 
                          ", Труба ID: " + std::to_string(pipeId) + ", Диаметр: " + std::to_string(diameter) + "мм");
                std::cout << "Соединение успешно создано!\n";
            } catch (const std::exception& e) {
                std::cout << "Ошибка: " << e.what() << std::endl;
                logger.log("Ошибка создания соединения", e.what());
            }
        }
        return;
    }
    
    // ПРОСИМ ВВЕСТИ ID КОНКРЕТНОЙ ТРУБЫ
    int pipeId = InputUtils::getPositiveInt("\nВведите ID трубы для соединения: ");
    
    // ПРОВЕРЯЕМ ВАЛИДНОСТЬ ВЫБРАННОЙ ТРУБЫ
    if (pipes.find(pipeId) == pipes.end()) {
        std::cout << "Труба с ID " << pipeId << " не найдена!\n";
        return;
    }
    
    if (usedPipes.find(pipeId) != usedPipes.end()) {
        std::cout << "Труба с ID " << pipeId << " уже используется в другом соединении!\n";
        return;
    }
    
    if (pipes.at(pipeId).isUnderRepair()) {
        std::cout << "Труба с ID " << pipeId << " в ремонте!\n";
        return;
    }
    
    // ПРОВЕРЯЕМ ДИАМЕТР ТРУБЫ
    int pipeDiameter = pipes.at(pipeId).getDiameter();
    if (std::find(allowedDiameters.begin(), allowedDiameters.end(), pipeDiameter) == allowedDiameters.end()) {
        std::cout << "ОШИБКА: Диаметр трубы " << pipeDiameter << " мм недопустим!\n";
        std::cout << "Допустимые диаметры: 500, 700, 1000, 1400 мм\n";
        std::cout << "Пожалуйста, выберите трубу с допустимым диаметром.\n";
        return;
    }
    
    try {
        network.addConnection(pipeId, startId, endId, pipeDiameter);
        
        logger.log("Создано соединение", 
                  "КС " + std::to_string(startId) + " -> КС " + std::to_string(endId) + 
                  ", Труба ID: " + std::to_string(pipeId) + ", Диаметр: " + std::to_string(pipeDiameter) + "мм");
        std::cout << "Соединение успешно создано!\n";
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
        logger.log("Ошибка создания соединения", e.what());
    }
}

int PipelineManager::createPipeWithAllowedDiameter() {
    std::cout << "\n=== Создание новой трубы ===\n";
    
    // Проверяем допустимые диаметры
    const std::vector<int> allowedDiameters = {500, 700, 1000, 1400};
    
    std::cout << "Допустимые диаметры: ";
    for (size_t i = 0; i < allowedDiameters.size(); ++i) {
        std::cout << allowedDiameters[i];
        if (i < allowedDiameters.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    int diameter = InputUtils::getPositiveInt("Введите диаметр трубы: ");
    
    // Проверяем корректность диаметра
    if (std::find(allowedDiameters.begin(), allowedDiameters.end(), diameter) == allowedDiameters.end()) {
        std::cout << "Недопустимый диаметр! Используйте один из допустимых значений.\n";
        return -1; // Возвращаем -1 при ошибке
    }
    
    Pipe newPipe;
    
    // Запрашиваем только название и длину
    std::string name = InputUtils::getString("Километровая отметка (название): ");
    double length = InputUtils::getPositiveDouble("Длина (в км): ");
    
    // Создаем трубу с заданными параметрами
    newPipe.setName(name);
    newPipe.setLength(length);
    newPipe.setDiameter(diameter);
    newPipe.setRepair(false);
    
    int pipeId = newPipe.getId();
    pipes[pipeId] = newPipe;
    
    std::cout << "Создана новая труба ID: " << pipeId << " диаметром " << diameter << " мм\n";
    logger.log("Создана труба", "ID: " + std::to_string(pipeId) + 
               ", Название: " + name + ", Диаметр: " + std::to_string(diameter) + "мм");
    
    return pipeId;
}

int PipelineManager::createPipeWithDiameter(int diameter) {
    std::cout << "\n=== Создание новой трубы ===\n";
    
    Pipe newPipe;
    
    // Запрашиваем только название и длину, диаметр уже известен
    std::string name = InputUtils::getString("Километровая отметка (название): ");
    double length = InputUtils::getPositiveDouble("Длина (в км): ");
    
    // Создаем трубу с заданными параметрами
    newPipe.setName(name);
    newPipe.setLength(length);
    newPipe.setDiameter(diameter); // Устанавливаем выбранный диаметр
    newPipe.setRepair(false);
    
    int pipeId = newPipe.getId();
    pipes[pipeId] = newPipe;
    
    std::cout << "Создана новая труба ID: " << pipeId << " диаметром " << diameter << " мм\n";
    logger.log("Создана труба", "ID: " + std::to_string(pipeId) + 
               ", Название: " + name + ", Диаметр: " + std::to_string(diameter) + "мм");
    
    return pipeId;
}

void PipelineManager::disconnectStations() {
    if (network.isEmpty()) {
        std::cout << "Сеть пуста!\n";
        return;
    }
    
    printNetwork();
    
    int pipeId = InputUtils::getPositiveInt("Введите ID трубы для разрыва соединения: ");
    
    network.removeConnection(pipeId);
    logger.log("Разорвано соединение", "Труба ID: " + std::to_string(pipeId));
    std::cout << "Соединение разорвано!\n";
}

void PipelineManager::viewNetwork() const {
    if (network.isEmpty()) {
        std::cout << "Сеть пуста!\n";
        return;
    }
    
    std::cout << "\n=== Газотранспортная сеть ===\n";
    auto connections = network.getAllConnections();
    
    for (const auto& conn : connections) {
        std::cout << "КС " << conn.startStationId << " -> КС " << conn.endStationId;
        std::cout << " | Труба ID: " << conn.pipeId << " | Диаметр: " << conn.diameter << "мм\n";
    }
}

void PipelineManager::topologicalSort() {
    if (network.isEmpty()) {
        std::cout << "Сеть пуста!\n";
        return;
    }
    
    try {
        std::vector<int> sortedStations = network.topologicalSort();
        
        std::cout << "\n=== Топологическая сортировка ===\n";
        std::cout << "Порядок обработки КС: ";
        for (size_t i = 0; i < sortedStations.size(); ++i) {
            std::cout << sortedStations[i];
            if (i < sortedStations.size() - 1) std::cout << " -> ";
        }
        std::cout << std::endl;
        
        logger.log("Выполнена топологическая сортировка", 
                  "Количество КС в сети: " + std::to_string(sortedStations.size()));
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
        logger.log("Ошибка топологической сортировки", e.what());
    }
}

void PipelineManager::printNetwork() const {
    viewNetwork();
}

void PipelineManager::saveData() {
    std::string filename = InputUtils::getString("Введите имя файла для сохранения: ");
    
    std::ofstream out(filename);
    if (!out) {
        std::cout << "Ошибка открытия файла для записи!\n";
        return;
    }
    
    out << "[PIPELINE_DATA]\n";
    out << "PIPE_NEXT_ID:" << Pipe::getNextId() << "\n";
    out << "STATION_NEXT_ID:" << CompressorStation::getNextId() << "\n";
    
    // Сохраняем трубы
    for (const auto& [id, pipe] : pipes) {
        pipe.saveToFile(out);
    }
    
    // Сохраняем КС
    for (const auto& [id, station] : stations) {
        station.saveToFile(out);
    }
    
    // Сохраняем сеть
    out << "[NETWORK]\n";
    auto connections = network.getAllConnections();
    for (const auto& conn : connections) {
        out << "CONNECTION:" << conn.pipeId << ":" << conn.startStationId << ":" 
            << conn.endStationId << ":" << conn.diameter << "\n";
    }
    out << "[END_NETWORK]\n";
    
    out << "[END_DATA]\n";
    out.close();
    
    logger.log("Сохранение данных", "Файл: " + filename + 
               ", Труб: " + std::to_string(pipes.size()) + 
               ", КС: " + std::to_string(stations.size()) +
               ", Соединений: " + std::to_string(connections.size()));
    std::cout << "Данные сохранены в файл '" << filename << "'\n";
}

void PipelineManager::loadData() {
    std::string filename = InputUtils::getString("Введите имя файла для загрузки: ");
    
    std::ifstream in(filename);
    if (!in) {
        std::cout << "Ошибка открытия файла для чтения!\n";
        return;
    }
    
    std::string line;
    bool validFormat = false;
    
    while (std::getline(in, line)) {
        if (line == "[PIPELINE_DATA]") {
            validFormat = true;
            break;
        }
    }
    
    if (!validFormat) {
        std::cout << "Неверный формат файла!\n";
        in.close();
        return;
    }
    
    // ВРЕМЕННО сохраняем максимальные ID
    int maxPipeId = 0;
    int maxStationId = 0;
    
    pipes.clear();
    stations.clear();
    network.clear();
    
    bool inNetworkSection = false;
    
    while (std::getline(in, line)) {
        if (line == "[END_DATA]") break;
        
        if (line == "[PIPE]") {
            Pipe pipe;
            pipe.loadFromFile(in);
            pipes[pipe.getId()] = pipe;
            if (pipe.getId() > maxPipeId) maxPipeId = pipe.getId();
        } else if (line == "[STATION]") {
            CompressorStation station;
            station.loadFromFile(in);
            stations[station.getId()] = station;
            if (station.getId() > maxStationId) maxStationId = station.getId();
        } else if (line == "[NETWORK]") {
            inNetworkSection = true;
        } else if (line == "[END_NETWORK]") {
            inNetworkSection = false;
        } else if (line.find("PIPE_NEXT_ID:") == 0) {
            // Игнорируем сохраненное значение, будем вычислять сами
        } else if (line.find("STATION_NEXT_ID:") == 0) {
            // Игнорируем сохраненное значение, будем вычислять сами
        } else if (inNetworkSection && line.find("CONNECTION:") == 0) {
            // Формат: CONNECTION:pipeId:startId:endId:diameter
            size_t pos = 11; // после "CONNECTION:"
            size_t nextPos = line.find(':', pos);
            int pipeId = std::stoi(line.substr(pos, nextPos - pos));
            
            pos = nextPos + 1;
            nextPos = line.find(':', pos);
            int startId = std::stoi(line.substr(pos, nextPos - pos));
            
            pos = nextPos + 1;
            nextPos = line.find(':', pos);
            int endId = std::stoi(line.substr(pos, nextPos - pos));
            
            pos = nextPos + 1;
            int diameter = std::stoi(line.substr(pos));
            
            try {
                network.addConnection(pipeId, startId, endId, diameter);
            } catch (const std::exception& e) {
                std::cout << "Предупреждение: не удалось загрузить соединение: " << e.what() << std::endl;
            }
        }
    }
    
    in.close();
    
    // УСТАНАВЛИВАЕМ правильные nextId после загрузки всех объектов
    Pipe::setNextId(maxPipeId + 1);
    CompressorStation::setNextId(maxStationId + 1);
    
    logger.log("Загрузка данных", "Файл: " + filename + 
               ", Загружено труб: " + std::to_string(pipes.size()) + 
               ", КС: " + std::to_string(stations.size()) +
               ", Соединений: " + std::to_string(network.getAllConnections().size()));
    std::cout << "Данные загружены из файла '" << filename << "'\n";
}

void PipelineManager::calculateMaxFlow() {
    if (network.isEmpty()) {
        std::cout << "Сеть пуста! Сначала создайте соединения между КС.\n";
        return;
    }
    
    // ОБЯЗАТЕЛЬНО устанавливаем ссылки на трубы
    network.setPipePointers(pipes);
    
    printAllStations();
    
    // ПОКАЗЫВАЕМ ВСЕ СОЕДИНЕНИЯ С ПРОИЗВОДИТЕЛЬНОСТЬЮ
    auto connections = network.getAllConnections();
    std::cout << "\n=== Текущие соединения и их производительность ===\n";
    if (connections.empty()) {
        std::cout << "Нет соединений!\n";
        return;
    }
    
    double totalCapacity = 0.0;
    for (const auto& conn : connections) {
        double capacity = 0.0;
        if (pipes.find(conn.pipeId) != pipes.end()) {
            capacity = pipes.at(conn.pipeId).getCapacity();
        }
        totalCapacity += capacity;
        
        std::cout << "КС " << conn.startStationId << " -> КС " << conn.endStationId;
        std::cout << " | Труба " << conn.pipeId << " (" << conn.diameter << "мм)";
        std::cout << " | Производительность: " << capacity << " м³/час";
        std::cout << std::endl;
    }
    std::cout << "Общая производительность всех труб: " << totalCapacity << " м³/час\n";
    
    int source = InputUtils::getPositiveInt("\nВведите ID КС источника: ");
    int sink = InputUtils::getPositiveInt("Введите ID КС стока: ");
    
    if (source == sink) {
        std::cout << "Источник и сток не могут быть одинаковыми!\n";
        return;
    }
    
    if (stations.find(source) == stations.end()) {
        std::cout << "КС источника с ID " << source << " не найдена!\n";
        return;
    }
    
    if (stations.find(sink) == stations.end()) {
        std::cout << "КС стока с ID " << sink << " не найдена!\n";
        return;
    }
    
    try {
        std::cout << "\n=== Расчет максимального потока ===\n";
        std::cout << "Рассчитываем поток от КС " << source << " к КС " << sink << "...\n";
        
        double maxFlow = network.calculateMaxFlow(source, sink);
        
        std::cout << "\nРЕЗУЛЬТАТ РАСЧЕТА:\n";
        std::cout << "Максимальный поток: " << maxFlow << " м³/час\n";
        std::cout << "Это означает, что можно передать " << maxFlow 
                  << " м³ газа в час от КС " << source << " к КС " << sink << "\n";
        
        // АНАЛИЗ РЕЗУЛЬТАТА
        if (maxFlow == 0) {
            std::cout << "\nПОТОК РАВЕН 0! Возможные причины:\n";
            std::cout << "1. Нет пути от КС " << source << " к КС " << sink << "\n";
            std::cout << "2. Все трубы на пути в ремонте\n";
            std::cout << "3. Пропускная способность труб слишком мала\n";
            
            // Проверяем есть ли путь
            std::vector<int> path = network.findShortestPath(source, sink);
            if (path.empty()) {
                std::cout << "   - Путь между КС действительно отсутствует\n";
            } else {
                std::cout << "   - Путь существует, но производительность труб = 0\n";
            }
        } else if (maxFlow < 50) {
            std::cout << "\nПоток очень мал. Рекомендации:\n";
            std::cout << "- Используйте трубы большего диаметра (1000-1400 мм)\n";
            std::cout << "- Уменьшите длину труб\n";
            std::cout << "- Проверьте трубы в ремонте\n";
        } else {
            std::cout << "\nОтличный результат! Сеть работает эффективно.\n";
        }
        
        logger.log("Расчет максимального потока", 
                  "Источник: " + std::to_string(source) + 
                  ", Сток: " + std::to_string(sink) + 
                  ", Поток: " + std::to_string(maxFlow) + " м³/час");
                  
    } catch (const std::exception& e) {
        std::cout << "ОШИБКА: " << e.what() << std::endl;
        logger.log("Ошибка расчета потока", e.what());
    }
}

void PipelineManager::findShortestPath() {
    if (network.isEmpty()) {
        std::cout << "Сеть пуста!\n";
        return;
    }
    
    // Устанавливаем ссылки на трубы
    network.setPipePointers(pipes);
    
    printAllStations();
    
    int start = InputUtils::getPositiveInt("Введите ID начальной КС: ");
    int end = InputUtils::getPositiveInt("Введите ID конечной КС: ");
    
    if (stations.find(start) == stations.end()) {
        std::cout << "Начальная КС с ID " << start << " не найдена!\n";
        return;
    }
    
    if (stations.find(end) == stations.end()) {
        std::cout << "Конечная КС с ID " << end << " не найдена!\n";
        return;
    }
    
    try {
        std::vector<int> path = network.findShortestPath(start, end);
        
        std::cout << "\n=== Кратчайший путь ===\n";
        if (path.empty()) {
            std::cout << "Путь между КС " << start << " и КС " << end << " не существует!\n";
        } else {
            std::cout << "Кратчайший путь: ";
            for (size_t i = 0; i < path.size(); ++i) {
                std::cout << path[i];
                if (i < path.size() - 1) std::cout << " -> ";
            }
            std::cout << "\nДлина пути: " << path.size() - 1 << " перегонов\n";
        }
        
        logger.log("Поиск кратчайшего пути", 
                  "От: " + std::to_string(start) + 
                  ", До: " + std::to_string(end) + 
                  ", Длина: " + std::to_string(path.size() - 1) + " перегонов");
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
        logger.log("Ошибка поиска пути", e.what());
    }
}

void PipelineManager::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void PipelineManager::pause() const {
    std::cout << "Нажмите Enter для продолжения...";
    InputUtils::clearInputBuffer();
}

void PipelineManager::run() {
    int choice;
    
    resetCounters();
    
    std::cout << "Программа управления трубопроводом\n";
    logger.log("Запуск программы");
    
    while (true) {
        printMenu();
        
        if (!(std::cin >> choice)) {
            std::cout << "Ошибка! Введите число от 1 до 17.\n";
            InputUtils::clearInputBuffer();
            continue;
        }
        InputUtils::clearInputBuffer();
        
        switch (choice) {
            case 1: addPipe(); break;
            case 2: addStation(); break;
            case 3: viewAll(); break;
            case 4: editPipe(); break;
            case 5: editStation(); break;
            case 6: deleteObject(); break;
            case 7: findObjects(); break;
            case 8: batchEdit(); break;
            case 9: connectStations(); break;
            case 10: disconnectStations(); break;
            case 11: viewNetwork(); break;
            case 12: topologicalSort(); break;
            case 13: calculateMaxFlow(); break;
            case 14: findShortestPath(); break;
            case 15: saveData(); break;
            case 16: loadData(); break;
            case 17:
                std::cout << "Выход из программы.\n";
                logger.log("Завершение программы");
                return;
            default:
                std::cout << "Неверный выбор! Введите число от 1 до 17.\n";
        }
        
        pause();
        clearScreen();
    }
}