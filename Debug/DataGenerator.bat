goto start

Этот bat-файл в качестве 1-го параметра exe-файлу передаёт число периодов функции,
в качестве 2-го параметра - число точек на один период функции, 
Если exe-файл вызывается без параметров, то по умолчанию число периодов функции равно 22, 
число точек на один период равно 50.

:start
::DataGenerator.exe
::DataGenerator.exe 22 33
DataGenerator.exe 22 22
::DataGenerator.exe 30 50
::DataGenerator.exe 24 132

::DataGenerator 3 10

pause