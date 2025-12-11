@echo off
chcp 65001 > nul
echo ===============================
echo Компиляция проекта трубопровода
echo ===============================

echo Удаляем старый исполняемый файл...
if exist pipeline_app.exe del pipeline_app.exe

echo.
echo Компилируем файлы...
g++ -g main.cpp Pipe.cpp CompressorStation.cpp PipelineManager.cpp GasNetwork.cpp -o pipeline_app.exe

if %errorlevel% == 0 (
    echo.
    echo КОМПИЛЯЦИЯ УСПЕШНО ЗАВЕРШЕНА!
    echo.
    echo Запуск программы...
    echo ===============================
    timeout /t 2 > nul
    .\pipeline_app.exe
) else (
    echo.
    echo ОШИБКА КОМПИЛЯЦИИ!
    echo Проверьте код на ошибки
    pause
)