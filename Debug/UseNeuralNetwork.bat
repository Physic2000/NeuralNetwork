goto start

���� bat-���� � �������� 1-�� ��������� exe-����� ������� ��� ����� c �������������� �������,
� �������� 2-�� ��������� - ��� ����� c ��������� �������,
� �������� 3-�� ��������� - ��� ��������������� ����� � ������� ����� �������� ���������� ������ ��� ������,
� �������� 4-�� ��������� - ����������� �������� �������, �.�. ����� �������� �������� ����,
� �������� 5-�� ��������� - ����� �������� �������� ����.
���� exe-���� ���������� ������ � ������� ������, �� �� ��������� ����� �������� �������� ���� ����� 6,
����� �������� �������� ���� ����� 10.

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
::	��������, ���� ����� UseNeuralNetwork �������� ���������:
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