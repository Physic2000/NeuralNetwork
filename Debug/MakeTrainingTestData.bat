goto start

��������� ��������� ������ �� ������������� � �������� � ������� �� �� ���-�� ������� ������ � ��������� ���� ������.
�������� ������ ��� ��� ������ ��������, ��� ���� ���� ����� ���-�� ������ n, �� ��������� n �������� � ������������� ����
������ ��������� � ������� n ���������� � �������� ����.

���� bat-���� � �������� 1-�� ��������� exe-����� ������� ��� ����� � �������,
� �������� 2-�� ��������� - ��� �����, � ������� ����� �������� ������������� ������,
� �������� 3-�� ��������� - ��� �����, � ������� ����� �������� �������� ������,
� �������� 4-�� ��������� - ����������� �������� ������� � ��� ������,
� �������� 5-�� ��������� - ���-�� �������� � �������� ���� (������ � ���-��� ������ ��� ). 
���� exe-���� ���������� ��� ����������, �� ������ ��������� �������� ���������������.

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