goto start

Этот bat-файл в качестве 1-го параметра exe-файлу передаёт имя файла c тренировочными данными,
в качестве 2-го параметра - имя файла c тестовыми данными,
в качестве 3-го параметра - имя результирующего файла в который будут записаны результаты работы ИНС Элмана,
в качестве 4-го параметра - размерность входного вектора, т.е. число нейронов входного слоя,
в качестве 5-го параметра - число нейронов скрытого слоя.
Если exe-файл вызывается только с именами файлов, то по умолчанию число нейронов входного слоя равно 6,
число нейронов скрытого слоя равно 10.

:start
::UseNeuralNetwork TrainingData.txt TestData.txt ResultData.txt 0.01 0.125 100 0.1
::UseNeuralNetwork TrainingData.txt TestData.txt ResultData.txt 0.01 0.125 100 0.1 25
::UseNeuralNetwork TrainingData.txt TestData.txt ResultData.txt 0.01 0.125 100 0.1 4 10
::UseNeuralNetwork TrainingData.txt TestData.txt ResultData.txt 0.01 0.125 100 0.1 6 10 25

UseNeuralNetwork TrainingData_Ri.txt TestData_Ri.txt ResultData_Ri.txt 0.01 0.125 100 0.1
::UseNeuralNetwork TrainingData_Ri.txt TestData_Ri.txt ResultData_Ri.txt 0.01 0.125 100 0.1 27
::UseNeuralNetwork TrainingData_Ri.txt TestData_Ri.txt ResultData_Ri.txt 0.01 0.125 100 0.1 50
::UseNeuralNetwork TrainingData_Ri.txt TestData_Ri.txt ResultData_Ri.txt 0.01 0.125 100 0.1 33
::UseNeuralNetwork TrainingData_Ri.txt TestData_Ri.txt ResultData_Ri.txt 0.01 0.125 100 0.1 6 10
::UseNeuralNetwork TrainingData_Ri.txt TestData_Ri.txt ResultData_Ri.txt 0.01 0.125 100 0.1 6 10 25
@if %ERRORLEVEL% NEQ 0 (
::	Действия, если вызов UseNeuralNetwork оказался неудачным:
	@echo.
	@echo ERROR UseNeuralNetwork!!!
	@echo.
	@pause
	@pause
	@exit
) else (
	@echo.
	@echo UseNeuralNetwork succeded!
	@echo.
	@pause
	@goto next_5
)
:next_5