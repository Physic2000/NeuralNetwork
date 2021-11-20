goto start

Программа фильтрует данные из файла, имя которого передаётся программе через командную строку первым аргументом
и записывает отфильтрованные данные в файл, имя которого передаётся программе вторым аргументом.
Третьим аргументом надо указать число весовых коэффициентов. Число должно быть не чётное и больше или равное 3.
Последующими аргументами программе передаются весовые коэффициенты.

:start

::DataFilter.exe GeneratedData.txt FilteredData.txt 5 0.05 0.2 0.5 0.2 0.05
::DataFilter.exe GeneratedData.txt FilteredData.txt 5 0.2 0.2 0.2 0.2 0.2
::DataFilter.exe GeneratedData.txt FilteredData.txt 5 0 0 1 0 0

::DataFilter Data_Ri.txt FilteredData_Ri.txt 5 0.05 0.2 0.5 0.2 0.05
::DataFilter Data_Ri.txt FilteredData_Ri.txt 5 0.2 0.2 0.2 0.2 0.2
::DataFilter Data_Ri.txt FilteredData_Ri.txt 5 0 0 1 0 0

::DataFilter Data_Ri.txt FilteredData_Ri.txt 3 0 1 0
::DataFilter Data_Ri.txt FilteredData_Ri.txt 5 0 0 1 0 0
::DataFilter Data_Ri.txt FilteredData_Ri.txt 7 0 0 0 1 0 0 0
::DataFilter Data_Ri.txt FilteredData_Ri.txt 11 0 0 0 0 0 1 0 0 0 0 0
::DataFilter Data_Ri.txt FilteredData_Ri.txt 23 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0

DataFilter MultipleRepeatedAveragedCycle.txt FilteredMultipleRepeatedAveragedCycle.txt 5 0 0 1 0 0

pause