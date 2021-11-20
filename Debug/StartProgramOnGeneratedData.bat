goto start



Этот bat скрипт запускает последовательность программ (exe-файлов) с необходимыми аргументами.
Итогом работы всех программ являются 3 результирующих txt-файла с данными, которые копируются в необходимую вторичную 
дирректорию BlackBox Component Builder для их графической визуализации.



DataGenerator - программа генерирует тестовые данные для ИНС (Искуственной Нейронной Сети).
В качестве 1-го параметра exe-файлу передаётся число периодов функции,
в качестве 2-го параметра - число точек на один период функции, 
Если exe-файл вызывается без параметров, то по умолчанию число периодов функции равно 22, 
число точек на один период равно 50.



ParabolicDataPreprocessing - программа выполняет пораболическую предобработку данных из файла имя которого передаётся программе через командную строку первым аргументом
и записывает обработанные данные в файл, имя которого передаётся программе вторым аргументом.
Третьим аргументом программе надо передать имя файла, в который будут записаны экстремумы числа солнечной активности внутри каждого Солнечного цикла - цикла Швабе-Вольфа (11 лет).
Четвёртым аргументом программе можно передать кол-во точек по которым будет строиться порабола в окрестностях минимума (по умолчанию оно равно 13).
Пятым аргументом программе можно передать кол-во точек по которым будет строиться порабола в окрестностях максимума (по умолчанию оно равно 7).



TransformData - программа считывает txt файл, содержащий весь массив данных (чисел солнечных пятен записанных помесячно), выполняет разбиение данных по циклам и упаковку их в 
специальную структуру данных, которая содержит в себе полную информацию об 11-ти летнем цикле Вольфа. Затем программа формирует последовательность объектов специальной структуры данных
(каждый объект - один Солнечный цикл) и записывает эту последовательность объектов в бинарный файл. Первым аргументом программе надо передать имя txt файла, содержащего весь массив 
данных. Этот файл может содержать или исходные данные, или данные подвергнутые пораболической предобработке. Вторым аргументом программе надо передать имя txt файла в который будут записаны 
преобразованные данные. Суть преобразования: каждый 11-ти летний цикл содержит ровно STANDARD_NUM_OF_DATUM_IN_SOLAR_CYCLE (= 132) отнормированных значения. Третьим аргументом программе надо передать
имя результирующего бинарного файла. Четвёртым аргументом программе надо передать имя txt файла для записи расширенной информации о солнечных циклах. Пятым аргументом программе надо передать кол-во 
полных солнечных циклов, которые будут записаны в txt файл, имя которого программе передаётся вторым аргументом. ВАЖНО: ПРИ АКТИВИРОВАНИИ ЭТОЙ ПРОГРАММЫ НЕОБХОДИМО ДЕАКТИВИРОВАТЬ ПРОГРАММУ 
DataNormalization, ПОСКОЛЬКУ ПРОГРАММА TransformData В СЕБЕ УЖЕ СОДЕРЖИТ ПРОЦЕДУРУ НОРМИРОВАНИЯ ДАННЫХ!!!



DataFilter - программа фильтрует данные из файла, имя которого передаётся программе через командную строку первым аргументом
и записывает отфильтрованные данные в файл, имя которого передаётся программе вторым аргументом.
Третьим аргументом надо передать имя двоичного файла в который будет дописаны для каждого Солнечного цикла 
значения индекса первого отфильтрованного элемента данных соответствующего элементу из массива pointTransformed.
Четвёртым аргументом надо указать число весовых коэффициентов. Число должно быть не чётное и большее или равное 3.
Последующими аргументами программе передаются весовые коэффициенты.



DataNormalization - программа выполняет нормирование данных из файла имя которого передаётся программе через командную строку первым аргументом
и записывает отнормированые данные в файл, имя которого передаётся программе вторым аргументом.
Третьим аргументом программе надо передать имя файла, в который будут записаны максимумы числа солнечной активности внутри каждого Солнечного цикла - цикла Швабе-Вольфа (11 лет).



MakeTrainingTestData - программа разделяет данные на тренировочные и тестовые и сшивает их по кол-ву входных данных в нейронной сети Элмана.
Сшивание данных для ИНС Элмана означает, что если сеть имеет кол-во входов n, то последние n значений в тренировочном ряду
должны совпадать с первыми n значениями в тестовом ряду.
MakeTrainingTestData.exe в качестве 1-го параметра принимает имя файла с исходными данными,
в качестве 2-го параметра - имя файла, в который будут записаны тренировочные данные,
в качестве 3-го параметра - имя файла, в который будут записаны тестовые данные,
в качестве 4-го параметра - размерность входного вектора в ИНС Элмана.
Если exe-файл вызывается без параметров, то работа программы аварийно останавливается.



UseNeuralNetwork - программа запускает процесс обучения ИНС и формирует на основе уже обученной ИНС результирующий файл, содержащий аппроксимацию данных, т.е. данные смоделированные ИНС.
UseNeuralNetwork.exe в качестве 1-го параметра принимает имя файла c тренировочными данными,
в качестве 2-го параметра - имя файла c тестовыми данными,
в качестве 3-го параметра - имя результирующего файла в который будут записаны результаты работы ИНС Элмана, т.е. данные смоделированные ИНС,
в качестве 4-го параметра - cкорость обучения нейронной сети, т.е. число типа double ( 0.001 <= a <= 0.5 ),
в качестве 5-го параметра - момент (инерция) обучения нейронной сети, т.е. число типа double ( 0 <= mu <= 1.0 ),
в качестве 6-го параметра - максимально допустимое число циклов обучения, т.е. число типа int ( 100, 200, 500 ),
в качестве 7-го параметра - максимально допустимая сумма квадратов ошибок на тестовых данных, т.е. число типа double,
в качестве 8-го необязательного параметра программа может принять размерность входного вектора, т.е. число нейронов входного слоя,
в качестве 9-го необязательного параметра программа может принять число нейронов скрытого слоя,
в качестве 10-го необязательного параметра программа может принять требуемое от ИНС число смоделированных данных.
Если exe-файл вызывается без необязательных параметров, то по умолчанию число нейронов входного слоя равно 6, число нейронов скрытого слоя равно 10, 
а требуемое от ИНС число смоделированных данных равно числу примеров в тестовых данных, т.е. равно числу тестовых данных уменьшенному на количество нейронов входного слоя.



AutoShifting - программа анализирует данные из двух, переданных ей, через командную строку, файлов, выполняет сдвиг в право данных из 1-го файла относительно данных из 2-го файла и 
записывает результат сдвига в файл имя которого передано программе 3-м аргументом. При этом сдвиг подбирается т.о., что бы сумма квадратов разностей соответствующих значений из
результирующего и 2-го файлов была бы минимальна. AutoShifting.exe в качестве 1-го параметра принимает имя файла данные из которого она будет сдвигать,
в качестве 2-го параметра - имя файла c тестовыми данными, относительно которого сдвигаются данные из 1-го файла,
в качестве 3-го параметра - имя файла, в который будут записаны сдвинутые данные,
в качестве 4-го параметра - кол-во нейронов входного слоя ИНС Элмана,
в качестве 5-го параметра - число сдвигов.



:start



::DataGenerator.exe
::DataGenerator.exe 22 44
::DataGenerator.exe 22 33
::DataGenerator.exe 25 33
::DataGenerator.exe 22 22
::DataGenerator.exe 30 50

::DataGenerator 3 10
::DataGenerator 29 132
::DataGenerator 24 132
::DataGenerator GeneratedData_2.txt 24 132
@IF %ERRORLEVEL% NEQ 0 (
::	Действия, если вызов DataGenerator оказался неудачным:
	@echo.
	@echo ERROR DataGenerator!!!
	@echo.
	@pause
	@pause
	@exit
) ELSE (
	@echo.
	@echo DataGenerator succeded!
	@echo.
	@pause
	@goto next_0
)
:next_0



::ParabolicDataPreprocessing GeneratedData.txt PreprocessedData.txt GeneratedExtremums.txt Data_Index.txt
::ParabolicDataPreprocessing GeneratedData.txt PreprocessedData.txt GeneratedExtremums.txt Data_Index.txt 3 3
::ParabolicDataPreprocessing GeneratedData.txt PreprocessedData.txt GeneratedExtremums.txt Data_Index.txt 5 5
::ParabolicDataPreprocessing GeneratedData.txt PreprocessedData.txt GeneratedExtremums.txt Data_Index.txt 7 7
::ParabolicDataPreprocessing GeneratedData.txt PreprocessedData.txt GeneratedExtremums.txt Data_Index.txt 9 9
::ParabolicDataPreprocessing GeneratedData.txt PreprocessedData.txt GeneratedExtremums.txt Data_Index.txt 13 13
ParabolicDataPreprocessing GeneratedData.txt PreprocessedData.txt GeneratedExtremums.txt Data_Index.txt 25 13
::ParabolicDataPreprocessing GeneratedData_2.txt PreprocessedData.txt GeneratedExtremums.txt Data_Index.txt 25 13

::ParabolicDataPreprocessing Data_Ri_NEW.txt PreprocessedData_Ri.txt SolarActivityExtremums.txt Data_Index_Ri.txt
::ParabolicDataPreprocessing Data_Ri.txt PreprocessedData_Ri.txt SolarActivityExtremums.txt Data_Index_Ri.txt
::ParabolicDataPreprocessing Data_Ri.txt PreprocessedData_Ri.txt SolarActivityExtremums.txt Data_Index_Ri.txt 13 13
::ParabolicDataPreprocessing Data_Ri.txt PreprocessedData_Ri.txt SolarActivityExtremums.txt Data_Index_Ri.txt 37 13

::ParabolicDataPreprocessing Data_Ri_2.txt PreprocessedData_Ri.txt SolarActivityExtremums.txt Data_Index_Ri.txt 13 13
::ParabolicDataPreprocessing Data_Ri_2.txt PreprocessedData_Ri.txt SolarActivityExtremums.txt Data_Index_Ri.txt 25 13
::ParabolicDataPreprocessing Data_Ri_2.txt PreprocessedData_Ri.txt SolarActivityExtremums.txt Data_Index_Ri.txt 25 25
@IF %ERRORLEVEL% NEQ 0 (
::	Действия, если вызов ParabolicDataPreprocessing оказался неудачным:
	@echo.
	@echo ERROR ParabolicDataPreprocessing!!!
	@echo.
	@pause
	@pause
	@exit
) ELSE (
	@echo.
	@echo ParabolicDataPreprocessing succeded!
	@echo.
	@pause
	@goto next_1
)
:next_1



TransformData PreprocessedData.txt TransformedData.txt TestCycles.dat Data_FullInformation.txt 24

::TransformData Data_Ri_NEW.txt TransformedData_Ri.txt SolarCycles.dat Data_Ri_FullInformation.txt 23
::TransformData Data_Ri.txt TransformedData_Ri.txt SolarCycles.dat Data_Ri_FullInformation.txt 23
::TransformData PreprocessedData_Ri.txt TransformedData_Ri.txt SolarCycles.dat Data_Ri_FullInformation.txt 20
@IF %ERRORLEVEL% NEQ 0 (
::	Действия, если вызов TransformData оказался неудачным:
	@echo.
	@echo ERROR TransformData!!!
	@echo.
	@pause
	@pause
	@exit
) ELSE (
	@echo.
	@echo TransformData succeded!
	@echo.
	@pause
	@goto next_2
)
:next_2



::TransformData TransformedData_Ri.txt TransformedData_Ri_2.txt SolarCycles_2.dat Data_Ri_FullInformation_2.txt



::DataFilter GeneratedData.txt FilteredData.txt TestCycles.dat 5 0.05 0.2 0.5 0.2 0.05
::DataFilter GeneratedData.txt FilteredData.txt TestCycles.dat 5 0.2 0.2 0.2 0.2 0.2
::DataFilter GeneratedData.txt FilteredData.txt TestCycles.dat 3 0 1 0
::DataFilter GeneratedData.txt FilteredData.txt TestCycles.dat 5 0 0 1 0 0
::DataFilter GeneratedData.txt FilteredData.txt TestCycles.dat 7 0 0 0 1 0 0 0

::DataFilter PreprocessedData.txt FilteredData.txt TestCycles.dat 5 0 0 1 0 0
DataFilter TransformedData.txt FilteredData.txt TestCycles.dat 5 0 0 1 0 0

::DataFilter Data_Ri.txt FilteredData_Ri.txt SolarCycles.dat 5 0.05 0.2 0.5 0.2 0.05
::DataFilter Data_Ri.txt FilteredData_Ri.txt SolarCycles.dat 5 0.2 0.2 0.2 0.2 0.2
::DataFilter Data_Ri.txt FilteredData_Ri.txt SolarCycles.dat 3 0 1 0
::DataFilter Data_Ri.txt FilteredData_Ri.txt SolarCycles.dat 5 0 0 1 0 0
::DataFilter Data_Ri.txt FilteredData_Ri.txt SolarCycles.dat 7 0 0 0 1 0 0 0
::DataFilter Data_Ri.txt FilteredData_Ri.txt SolarCycles.dat 11 0 0 0 0 0 1 0 0 0 0 0
::DataFilter Data_Ri.txt FilteredData_Ri.txt SolarCycles.dat 23 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0
::DataFilter Data_Ri.txt FilteredData_Ri.txt SolarCycles.dat 7 1 1 1 1 1 1 1

::DataFilter PreprocessedData_Ri.txt FilteredData_Ri.txt SolarCycles.dat 5 0 0 1 0 0
::DataFilter PreprocessedData_Ri.txt FilteredData_Ri.txt SolarCycles.dat 7 0 0 0 1 0 0 0

::DataFilter TransformedData_Ri.txt FilteredData_Ri.txt SolarCycles.dat 5 0 0 1 0 0
::DataFilter TransformedData_Ri_2.txt FilteredData_Ri.txt SolarCycles.dat 5 0 0 1 0 0
@IF %ERRORLEVEL% NEQ 0 (
::	Действия, если вызов DataFilter оказался неудачным:
	@echo.
	@echo ERROR DataFilter!!!
	@echo.
	@pause
	@pause
	@exit
) ELSE (
	@echo.
	@echo DataFilter succeded!
	@echo.
	@pause
	@goto next_3
)
:next_3



::DataNormalization GeneratedData.txt NormalizedData.txt GeneratedMaxima.txt
::DataNormalization FilteredData.txt NormalizedData.txt GeneratedMaxima.txt
::DataNormalization PreprocessedData.txt NormalizedData.txt GeneratedMaxima.txt

::DataNormalization Data_Ri.txt NormalizedData_Ri.txt SolarActivityMaxima.txt
::DataNormalization FilteredData_Ri.txt NormalizedData_Ri.txt SolarActivityMaxima.txt
::DataNormalization PreprocessedData_Ri.txt NormalizedData_Ri.txt SolarActivityMaxima.txt



::MakeTrainingTestData GeneratedData.txt TrainingData.txt TestData.txt 6
MakeTrainingTestData FilteredData.txt TrainingData.txt TestData.txt 6
::MakeTrainingTestData NormalizedData.txt TrainingData.txt TestData.txt 6

::MakeTrainingTestData Data_Ri.txt TrainingData_Ri.txt TestData_Ri.txt 6
::MakeTrainingTestData PreprocessedData_Ri.txt TrainingData_Ri.txt TestData_Ri.txt 6
::MakeTrainingTestData FilteredData_Ri.txt TrainingData_Ri.txt TestData_Ri.txt 6
::MakeTrainingTestData NormalizedData_Ri.txt TrainingData_Ri.txt TestData_Ri.txt 6
@IF %ERRORLEVEL% NEQ 0 (
::	Действия, если вызов MakeTrainingTestData оказался неудачным:
	@echo.
	@echo ERROR MakeTrainingTestData!!!
	@echo.
	@pause
	@pause
	@exit
) ELSE (
	@echo.
	@echo MakeTrainingTestData succeded!
	@echo.
	@pause
	@goto next_4
)
:next_4



UseNeuralNetwork TrainingData.txt TestData.txt ResultData.txt 0.01 0.125 100 0.1
::UseNeuralNetwork TrainingData.txt TestData.txt ResultData.txt 0.01 0.125 100 0.1 25
::UseNeuralNetwork TrainingData.txt TestData.txt ResultData.txt 0.01 0.125 100 0.1 4 10
::UseNeuralNetwork TrainingData.txt TestData.txt ResultData.txt 0.01 0.125 100 0.1 6 10 25

::UseNeuralNetwork TrainingData_Ri.txt TestData_Ri.txt ResultData_Ri.txt 0.01 0.125 100 0.1
::UseNeuralNetwork TrainingData_Ri.txt TestData_Ri.txt ResultData_Ri.txt 0.01 0.125 100 0.1 27
::UseNeuralNetwork TrainingData_Ri.txt TestData_Ri.txt ResultData_Ri.txt 0.01 0.125 100 0.1 50
::UseNeuralNetwork TrainingData_Ri.txt TestData_Ri.txt ResultData_Ri.txt 0.01 0.125 100 0.1 33
::UseNeuralNetwork TrainingData_Ri.txt TestData_Ri.txt ResultData_Ri.txt 0.01 0.125 100 0.1 6 10
::UseNeuralNetwork TrainingData_Ri.txt TestData_Ri.txt ResultData_Ri.txt 0.01 0.125 100 0.1 6 10 25
@IF %ERRORLEVEL% NEQ 0 (
::	Действия, если вызов UseNeuralNetwork оказался неудачным:
	@echo.
	@echo ERROR UseNeuralNetwork!!!
	@echo.
	@pause
	@pause
	@exit
) ELSE (
	@echo.
	@echo UseNeuralNetwork succeded!
	@echo.
	@pause
	@goto next_5
)
:next_5



::AutoShifting ResultData.txt TestData.txt ShiftedData.txt 6 5
::AutoShifting ResultData_Ri.txt TestData_Ri.txt ShiftedData_Ri.txt 6 5



COPY TrainingData.txt ..\..\..\..\Chumakov
COPY TrainingData.txt ..\..\..\..\Chumakov\Neuralnetwork\DataFiles\
COPY TestData.txt ..\..\..\..\Chumakov
COPY TestData.txt ..\..\..\..\Chumakov\Neuralnetwork\DataFiles\
COPY ResultData.txt ..\..\..\..\Chumakov\Neuralnetwork\DataFiles\
::COPY ShiftedData.txt ..\..\..\..\Chumakov\Neuralnetwork\DataFiles\

::COPY TrainingData_Ri.txt ..\..\..\..\Chumakov
::COPY TrainingData_Ri.txt ..\..\..\..\Chumakov\Neuralnetwork\DataFiles\
::COPY TestData_Ri.txt ..\..\..\..\Chumakov
::COPY TestData_Ri.txt ..\..\..\..\Chumakov\Neuralnetwork\DataFiles\
::COPY ResultData_Ri.txt ..\..\..\..\Chumakov\Neuralnetwork\DataFiles\
::COPY ShiftedData_Ri.txt ..\..\..\..\Chumakov\Neuralnetwork\DataFiles\



pause