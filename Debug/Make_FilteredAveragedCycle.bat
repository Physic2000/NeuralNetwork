goto start

Этот bat файл создаёт файл "FilteredAveragedCycle.txt".

DataFilter - программа фильтрует данные из файла, имя которого передаётся программе через командную строку первым аргументом
и записывает отфильтрованные данные в файл, имя которого передаётся программе вторым аргументом.
Третьим аргументом надо указать число весовых коэффициентов. Число должно быть не чётное и больше или равное 3.
Последующими аргументами программе передаются весовые коэффициенты.

MakeTrainingTestData - программа разделяет данные на тренировочные и тестовые и сшивает их по кол-ву входных данных в нейронной сети Элмана.
Сшивание данных для ИНС Элмана означает, что если сеть имеет кол-во входов n, то последние n значений в тренировочном ряду
должны совпадать с первыми n значениями в тестовом ряду.
MakeTrainingTestData.exe в качестве 1-го параметра принимает имя файла с исходными данными,
в качестве 2-го параметра - имя файла, в который будут записаны тренировочные данные,
в качестве 3-го параметра - имя файла, в который будут записаны тестовые данные,
в качестве 4-го параметра - размерность входного вектора в ИНС Элмана.
Если exe-файл вызывается без параметров, то работа программы аварийно останавливается.

:start
DataFilter MultipleRepeatedAveragedCycle.txt FilteredMultipleRepeatedAveragedCycle.txt 5 0 0 1 0 0
@if %ERRORLEVEL% NEQ 0 (
::	Действия, если вызов DataFilter оказался неудачным:
	@echo.
	@echo ERROR DataFilter!!!
	@echo.
	@pause
	@exit
) else (
	@echo.
	@echo DataFilter succeded!
	@echo.
::	@pause
::	goto A
)

A:
MakeTrainingTestData FilteredMultipleRepeatedAveragedCycle.txt TempTrainingData_Ri.txt FilteredAveragedCycle.txt 6
@if %ERRORLEVEL% NEQ 0 (
::	Действия, если вызов MakeTrainingTestData оказался неудачным:
	@echo.
	@echo ERROR MakeTrainingTestData!!!
	@echo.
	@pause
	@exit
) else (
	@echo.
	@echo MakeTrainingTestData succeded!
	@echo.
::	@pause
::	goto B
)

B:
cls
@COPY FilteredAveragedCycle.txt ..\..\..\..\Chumakov\Neuralnetwork\DataFiles\
@echo.
@echo The "FilteredAveragedCycle.txt" file has been successfully created and copied to the destination directory.
@echo The destination directory is "..\..\..\..\Chumakov\Neuralnetwork\DataFiles".
@echo.

pause