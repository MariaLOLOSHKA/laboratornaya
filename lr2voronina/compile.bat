@echo off
chcp 65001 > nul
echo ===============================
echo Компиляция проекта трубопровода
echo ===============================

echo Проверяем файлы...
if not exist main.cpp (
    echo ОШИБКА: main.cpp не найден!
    pause
    exit
)
if not exist Pipe.cpp (
    echo ОШИБКА: Pipe.cpp не найден!
    pause
    exit
)
if not exist CompressorStation.cpp (
    echo ОШИБКА: CompressorStation.cpp не найден!
    pause
    exit
)

echo.
echo Все файлы найдены. Компилируем...
g++ -g main.cpp Pipe.cpp CompressorStation.cpp -o pipeline_app.exe

if %errorlevel% == 0 (
    echo.
    echo ✅ КОМПИЛЯЦИЯ УСПЕШНА!
    echo.
    echo Запуск программы...
    echo ===============================
    timeout /t 2 > nul
    pipeline_app.exe
) else (
    echo.
    echo ❌ Ошибка компиляции!
    echo.
    echo Возможные причины:
    echo 1. Нет компилятора g++
    echo 2. Ошибки в коде C++
    echo 3. Файлы повреждены
)

pause