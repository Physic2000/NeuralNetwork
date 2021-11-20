goto start

Программа разделяет данные на тренировочные и тестовые и сшивает их по кол-ву входных данных в нейронной сети Элмана.
Сшивание данных для ИНС Элмана означает, что если сеть имеет кол-во входов n, то последние n значений в тренировочном ряду
должны совпадать с первыми n значениями в тестовом ряду.

Этот bat-файл в качестве 1-го параметра exe-файлу передаёт имя файла с данными,
в качестве 2-го параметра - имя файла, в который будут записаны тренировочные данные,
в качестве 3-го параметра - имя файла, в который будут записаны тестовые данные,
в качестве 4-го параметра - размерность входного вектора в ИНС Элмана,
в качестве 5-го параметра - кол-во значений в тестовом ряду (вместе с кол-вом входов ИНС ). 
Если exe-файл вызывается без параметров, то работа программы аварийно останавливается.

:start
::MakeTrainingTestData.exe GeneratedData.txt TrainingData.txt TestData.txt 6 106
::MakeTrainingTestData.exe GeneratedData.txt TrainingData.txt TestData.txt 6 72
::MakeTrainingTestData.exe GeneratedData.txt TrainingData.txt TestData.txt 6 50

::MakeTrainingTestData.exe Data_Ri.txt TrainingData_Ri.txt TestData_Ri.txt 6 138
::MakeTrainingTestData.exe FilteredData_Ri.txt TrainingData_Ri.txt TestData_Ri.txt 6 50

::MakeTrainingTestData.exe PreprocessedData_Ri.txt TrainingData_Ri.txt TestData_Ri.txt 6 138

MakeTrainingTestData FilteredMultipleRepeatedAveragedCycle.txt TempTrainingData_Ri.txt FilteredAveragedCycle.txt 6

COPY FilteredAveragedCycle.txt ..\..\..\..\Chumakov\Neuralnetwork\DataFiles\

pause