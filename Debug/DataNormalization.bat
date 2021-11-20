goto start

Программа выполняет нормирование данных из файла имя которого передаётся программе через командную строку первым аргументом
и записывает отнормированые данные в файл, имя которого передаётся программе вторым аргументом.
Третьим аргументом программе надо передать имя файла, в который будут записаны максимумы числа солнечной активности внутри 
каждого Солнечного цикла - цикла Швабе-Вольфа (11 лет).

:start

DataNormalization.exe Data_Ri.txt NormalizedData_Ri.txt SolarActivityMaxima.txt

::DataNormalization GeneratedData.txt NormalizedData.txt GeneratedMaxima.txt

pause