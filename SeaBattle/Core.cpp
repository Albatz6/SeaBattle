#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <Windows.h>
#include <vector>
#include <random>
#include "AuxiliaryFuncs.h"

using namespace std;


/*   ГРАФИКА   */

//Выводит двумерный массив с расстановкой в ASCII-графике
//displayFullComputerGrid - определяет, показывается ли пользователю полное поле компьютера со всеми кораблями. Если true, то показывается
void DisplayGrid(int userGrid[10][10], int generatedGrid[10][10], bool displayFullComputerGrid)
{
    /* Цвета для аргумента функции SetConsoleTextAttribute */
    //7 - цвет консоли по умолчанию
    //11 - бирюзовый
    //191 - белый на бирюзовом фоне
    //12 - красный
    //79 - белый на красном фоне
    //64 - черный на красном фоне
    //14 - желтый
    //6 - оранжевый
    //111 - белый на оранжевом фоне
    //96 - черный на оранжевом фоне
    //10 - зеленый
    //9 - синий
    //31 - белый на синем фоне
    //16 - черный на синем фоне
    //13 - фиолетовый
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Для управления цветом текста в консоли

    //Выводим оформляющие строки с буквами
    for (int i = 0; i < 2; i++)
    {
        cout << "    ";
        for (int j = 0; j < 10; j++)
        {
            cout << " ";
            cout << (char)(j + 65); //65-й символ ASCII - 'A'
            cout << "  ";
        }

        cout << "     ";
    }
    cout << "\n";

    //Выводим оформляющие строки-разделители перед выведением строк со значениями
    for (int i = 0; i < 2; i++)
    {
        cout << "   |";
        for (int j = 0; j < 10; j++)
        {
            cout << "---|";
        }

        cout << "     ";
    }
    cout << "\n";

    //Выводим значения матрицы
    for (int i = 0; i < 10; i++)
    {
        cout << i + 1; //Выводим маркировку строки пользовательской матрицы
        (i == 9) ? (cout << " ") : (cout << "  "); //Если выводим десятую строку, вправо откладываем только один пробел
        cout << "|";

        //Выводим строку со значениями пользовательской матрицы
        for (int j = 0; j < 10; j++)
        {
            cout << " ";

            switch (userGrid[i][j])
            {
                case 4:
                    SetConsoleTextAttribute(hConsole, 16);
                    break;

                case 1:
                    SetConsoleTextAttribute(hConsole, 11);
                    break;

                case 2:
                    SetConsoleTextAttribute(hConsole, 111);
                    break;

                case 3:
                    SetConsoleTextAttribute(hConsole, 79);
                    break;

                default: break;
            }
            (userGrid[i][j] == 0) ? (cout << " ") : (cout << "#");

            SetConsoleTextAttribute(hConsole, 7);
            cout << " |"; //Выводит " 0 |" или " 1 |"
        }


        cout << "     "; //Выводим маркировку строки сгенерированной матрицы
        cout << i + 1;
        (i == 9) ? (cout << " ") : (cout << "  ");
        cout << "|";

        //Выведение значений сгенерированной матрицы
        for (int j = 0; j < 10; j++)
        {
            cout << " ";

            switch (generatedGrid[i][j])
            {
                case 4:
                    SetConsoleTextAttribute(hConsole, 16);
                    break;

                case 1:
                    SetConsoleTextAttribute(hConsole, 11);
                    break;

                case 2:
                    SetConsoleTextAttribute(hConsole, 111);
                    break;

                case 3:
                    SetConsoleTextAttribute(hConsole, 79);
                    break;

                default: break;
            }
            //(generatedGrid[i][j] == 0) ? (cout << " ") : (cout << "#"); //Для тестовых запусков раскомментировать эту строку или следующую
            //(generatedGrid[i][j] == 1 || generatedGrid[i][j] == 2 || generatedGrid[i][j] == 3 || generatedGrid[i][j] == 4) ? (cout << "#") : (cout << " ");
            if (!displayFullComputerGrid) //Если isFinished верно, то нужно вывести пользователю полное поле компьютера со всеми кораблями
            {
                (generatedGrid[i][j] == 2 || generatedGrid[i][j] == 3 || generatedGrid[i][j] == 4) ? (cout << "#") : (cout << " ");
            }
            else
            {
                (generatedGrid[i][j] == 1 || generatedGrid[i][j] == 2 || generatedGrid[i][j] == 3 || generatedGrid[i][j] == 4) ?
                    (cout << "#") : (cout << " ");
            }

            SetConsoleTextAttribute(hConsole, 7);
            cout << " |"; //Выводит " 0 |" или " 1 |"
        }
        cout << "\n";

        //Выводим разделительные строки
        for (int j = 0; j < 2; j++)
        {
            cout << "   |";
            for (int k = 0; k < 10; k++)
            {
                cout << "---|";
            }

            cout << "     ";
        }
        cout << "\n";
    }
}



/*   ОСНОВНЫЕ ФУНКЦИИ   */

//Считывает пользовательскую расстановку кораблей матрицей из 0 и 1 в двумерный массив
void ReadGridFromFile(ifstream& inputFile, int(*input)[10])
{
    string line;
    int k; //Указывает местоположение в каждой отдельно взятой строке

    for (int i = 0; i < 10; i++) //Поэлементное чтение пользовательской матрицы в массив
    {
        getline(inputFile, line);
        k = 0;

        for (int j = 0; j < 10; j++)
        {
            string numValue;
            int value;

            while (line[k] != ' ' && k != line.length())
            {
                numValue += line[k];
                k++;
            }
            k++; //Переводим указатель в строке на следующий символ для следующей итерации

            value = atoi(numValue.c_str());

            switch (value)
            {
            case 0:
                input[i][j] = false;
                break;

            case 1:
                input[i][j] = true;
                break;

            default:
                cout << "Неверный формат входных данных\n\n";
                exit(2);
                break;
            }
        }
    }
}

//Генерирует матрицу, которую компьютер использует для игры
void GenerateGrid(int (*input)[10])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Для управления цветом текста
    vector<vector<int>> coords; //Вектор, каждым элементом которого является вектор с координатой ячейки поля
    
    for (int i = 0; i < 10; i++) //Заполняем вектор векторами с координатами каждой отдельной клетки
    {
        for (int j = 0; j < 10; j++)
        {
            vector<int> element;

            element.push_back(i);
            element.push_back(j);

            coords.push_back(element);
        }
    }

    for (int i = 0; i < 10; i++) //Заполнение матрицы компьютера нулевыми значениями перед расстановкой кораблей
    {
        for (int j = 0; j < 10; j++)
        {
            input[i][j] = 0;
        }
    }
    
    PlaceShip(4, 1, coords, input); //Размещение четырехпалубного корабля
    //Sleep(100); //Ждём, чтобы получать более случайные значения по сиду от времени
    PlaceShip(3, 2, coords, input); //Размещение трехпалубных кораблей
    //Sleep(100);
    PlaceShip(2, 3, coords, input); //Размещение двухпалубных кораблей
    //Sleep(100);
    PlaceShip(1, 4, coords, input); //Размещение однопалубников
    //Sleep(100);

    /*for (int i = 0; i < coords.size(); i++)
    {
        for (int j = 0; j < coords[i].size(); j++)
        {
            cout << coords[i][j] << "  ";
        }
        cout << endl;
    }
    cout << endl;*/
    
    //cout << (x + 1) << " " << (y + 1) << " " << angleParameter << "\n"; //Для трейсинга координат четырехпалубника
    SetConsoleTextAttribute(hConsole, 10); //Зеленый цвет текста
    cout << "==Расстановка сгенерирована==\n\n";
    SetConsoleTextAttribute(hConsole, 7); //Зеленый цвет текста
}

//Фиксирует ход игрока и изменяет матрицу в соответствии с ним
//row, column - координаты
//isHuman - определяет, кто делает ход. True, если пользователь, false для компьютера
//*moveCondition - указатель на флаг состояния хода (чтобы продлевать ход при удачном выстреле)
//*inputGrid - указатель на матрицу, в которой делается ход
//Возвращает значение isFinished - закончена ли игра. True, если да, false, если нет
bool MakeMove(int row, int column, bool isHuman, bool* moveCondition, int (*inputGrid)[10])
{
    if (inputGrid[column][row] == 1) //Если попали в нераненную клетку корабля
    {
        int killedCells = 0; //Содержит количество клеток убитых кораблей. Переменная нужна, чтобы зафиксировать окончание игры
        int up, down, left, right;

        //Получаем значения ближайших клеток, чтобы узнать, с какой стороны располагается остальная часть корабля
        (column - 1 >= 0) ? (up = inputGrid[column - 1][row]) : (up = -1);
        (column + 1 <= 9) ? (down = inputGrid[column + 1][row]) : (down = -1);
        (row - 1 >= 0) ? (left = inputGrid[column][row - 1]) : (left = -1);
        (row + 1 <= 9) ? (right = inputGrid[column][row + 1]) : (right = -1);


        if (up != 1 && down != 1 && left != 1 && right != 1 && //Потоплен однопалубный корабль
            up != 2 && down != 2 && left != 2 && right != 2) 
        {
            inputGrid[column][row] = 3; //Красный цвет корабля

            //Раскраска окрестностей в синий цвет
            if (column - 1 >= 0)
            {
                inputGrid[column - 1][row] = 4; 

                if (row - 1 >= 0)
                {
                    inputGrid[column - 1][row - 1] = 4;
                }

                if (row + 1 <= 9)
                {
                    inputGrid[column - 1][row + 1] = 4;
                }
            }

            if (column + 1 <= 9)
            {
                inputGrid[column + 1][row] = 4;

                if (row - 1 >= 0)
                {
                    inputGrid[column + 1][row - 1] = 4;
                }

                if (row + 1 <= 9)
                {
                    inputGrid[column + 1][row + 1] = 4;
                }
            }

            if (row - 1 >= 0)
            {
                inputGrid[column][row - 1] = 4;
            }

            if (row + 1 <= 9)
            {
                inputGrid[column][row + 1] = 4;
            }
        }

        if (up == 1 || down == 1 || left == 1 || right == 1) //Задет корабль длиной больше 1
        {
            inputGrid[column][row] = 2;
        }

        if (up == 2 || down == 2 || left == 2 || right == 2) //Задет ранее подбитый корабль
        {
            bool isFillingNeeded = false; //Флаг, показывающий, нужно ли заполнять окрестности вокруг потопленного корабля

            if (up == 2) //Если сверху есть ранненая клетка корабля
            {
                int validCellsCount = 0; //Хранит количество целых клеток корабля, которые располагаются выше/ниже
                int c = 1;
                bool isKilled = true; //Флаг состояния корабля. Станет false, если найдётся хотя бы одна целая клетка корабля (validCellsCount != 0)

                //Определеяем, есть ли сверху нетронутые клетки корабля
                while (inputGrid[column - c][row] != 0 && inputGrid[column - c][row] != 4 && column - c >= 0)
                {
                    if (inputGrid[column - c][row] == 1)
                    {
                        validCellsCount++;
                    }

                    c++;
                } 

                c = 1;

                //Определеяем, есть ли снизу нетронутые клетки корабля
                while (inputGrid[column + c][row] != 0 && inputGrid[column + c][row] != 4 && column + c <= 9)
                {
                    if (inputGrid[column + c][row] == 1)
                    {
                        validCellsCount++;
                    } 

                    c++;
                }

                if (validCellsCount != 0) //Если нашли целые клетки корабля, значит, он не потоплен
                {
                    isKilled = false;
                }

                if (isKilled) //Если корабль убит
                {
                    int cnt = 0;
                    int v = -1;

                    while (v != 0 && v != 4 && (column - cnt >= 0))
                    {
                        if (inputGrid[column - cnt][row] == 1 || inputGrid[column - cnt][row] == 2)
                        {
                            inputGrid[column - cnt][row] = 3; //Красный цвет клетки
                        }

                        v = inputGrid[column - cnt][row];

                        cnt++;
                    }

                    isFillingNeeded = true;
                }
                else
                {
                    inputGrid[column][row] = 2; //Если корабль не убит, обозначаем данную клетку как раненную
                }
            }

            if (down == 2)
            {
                int validCellsCount = 0;
                int c = 1;
                bool isKilled = true;

                //Определеяем, есть ли сверху нетронутые клетки корабля
                while (inputGrid[column - c][row] != 0 && inputGrid[column - c][row] != 4 && column - c >= 0)
                {
                    if (inputGrid[column - c][row] == 1)
                    {
                        validCellsCount++;
                    }

                    c++;
                }

                c = 1;

                //Определеяем, есть ли снизу нетронутые клетки корабля
                while (inputGrid[column + c][row] != 0 && inputGrid[column + c][row] != 4 && column + c <= 9) 
                {
                    if (inputGrid[column + c][row] == 1)
                    {
                        validCellsCount++;
                    }

                    c++;
                }

                if (validCellsCount != 0)
                {
                    isKilled = false;
                }

                if (isKilled)
                {
                    int cnt = 0;
                    int v = -1;

                    while (v != 0 && v != 4 && (column + cnt <= 9))
                    {
                        if (inputGrid[column + cnt][row] == 1 || inputGrid[column + cnt][row] == 2)
                        {
                            inputGrid[column + cnt][row] = 3;
                        }

                        v = inputGrid[column + cnt][row];

                        cnt++;
                    }

                    isFillingNeeded = true;
                }
                else
                {
                    inputGrid[column][row] = 2;
                }
            }

            if (left == 2)
            {
                int validCellsCount = 0;
                int c = 1;
                bool isKilled = true;

                //Определеяем, есть ли слева нетронутые клетки корабля
                while (inputGrid[column][row - c] != 0 && inputGrid[column][row - c] != 4 && row - c >= 0)
                {
                    if (inputGrid[column][row - c] == 1)
                    {
                        validCellsCount++;
                    }

                    c++;
                }

                c = 1;

                //Определеяем, есть ли справа нетронутые клетки корабля
                while (inputGrid[column][row + c] != 0 && inputGrid[column][row + c] != 4 && row + c <= 9)
                {
                    if (inputGrid[column][row + c] == 1)
                    {
                        validCellsCount++;
                    }

                    c++;
                }

                if (validCellsCount != 0)
                {
                    isKilled = false;
                }

                if (isKilled) //Если корабль убит
                {
                    int cnt = 0;
                    int v = -1;

                    while (v != 0 && v != 4 && (row - cnt >= 0))
                    {
                        if (inputGrid[column][row - cnt] == 1 || inputGrid[column][row - cnt] == 2)
                        {
                            inputGrid[column][row - cnt] = 3; //Красный цвет клетки
                        }

                        v = inputGrid[column][row - cnt];

                        cnt++;
                    }

                    isFillingNeeded = true;
                }
                else
                {
                    inputGrid[column][row] = 2;
                }
            }

            if (right == 2)
            {
                int validCellsCount = 0;
                int c = 1;
                bool isKilled = true;

                //Определеяем, есть ли слева нетронутые клетки корабля
                while (inputGrid[column][row - c] != 0 && inputGrid[column][row - c] != 4 && row - c >= 0)
                {
                    if (inputGrid[column][row - c] == 1)
                    {
                        validCellsCount++;
                    }

                    c++;
                }

                c = 1;

                //Определеяем, есть ли справа нетронутые клетки корабля
                while (inputGrid[column][row + c] != 0 && inputGrid[column][row + c] != 4 && row + c <= 9)
                {
                    if (inputGrid[column][row + c] == 1)
                    {
                        validCellsCount++;
                    }

                    c++;
                }

                if (validCellsCount != 0)
                {
                    isKilled = false;
                }

                if (isKilled) //Если корабль убит
                {
                    int cnt = 0;
                    int v = -1;

                    while (v != 0 && v != 4 && (row + cnt <= 9))
                    {
                        if (inputGrid[column][row + cnt] == 1 || inputGrid[column][row + cnt] == 2)
                        {
                            inputGrid[column][row + cnt] = 3; //Красный цвет клетки
                        }

                        v = inputGrid[column][row + cnt];

                        cnt++;
                    }

                    isFillingNeeded = true;
                }
                else
                {
                    inputGrid[column][row] = 2;
                }
            }


            if (isFillingNeeded) //Если корабль потоплен, то клетки в окрестностях синим цветом
            {
                FillNearbyArea(inputGrid);
            }
        }


        if (isHuman)
        {
            *moveCondition = true; //Передаем следующий ход человеку
        }
        else
        {
            *moveCondition = false; //Передаем следующий ход компьютеру
        }

        for (int i = 0; i < 10; i++) //Считаем количество "убитых" клеток (клеток с состоянием 3)
        {
            for (int j = 0; j < 10; j++)
            {
                if (inputGrid[i][j] == 3)
                {
                    killedCells++;
                }
            }
        }

        if (killedCells == 20) //Если найдется 20 клеток в состоянии 3, значит, все корабли потоплены
        {
            return true;
        }
    }
    else
    {
        if (inputGrid[column][row] != 3 && inputGrid[column][row] != 2) //Если встретили значение 3, то это потопленный корабль
        {
            inputGrid[column][row] = 4; //Синий цвет клетки, если попали мимо
        }

        *moveCondition = !*moveCondition; //Меняем флаг хода на противоположный, чтобы ход сделал другой игрок
    }

    return false;
}

//Реализует ход компьютера
//*inputGrid - указатель на матрицу пользователя, в которой ходит компьютер
//*computerGrid - указатель на матрицу компьютера
//&inputVector - указатель на вектор с координатами каждой клетки
//*moveCondition - указатель на флаг состояния хода
//nearbyArea - массив из четырех элементов, каждый из которых хранит состояние ближайшей клетки на каждой стороне
bool ComputerMove(int (*inputGrid)[10], int (*computerGrid)[10], vector<vector<int>>& inputVector, bool* moveCondition, int (*nearbyArea))
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Для управления цветом текста
    bool isFinished = false; //Хранит состояние игры. Если станет true, игра завершится
    int x, y, numberOfCoord;
    x = -1; y = -1;
    srand(time(NULL)); //Сид по времени для получения случайных чисел

    //Ищем, есть ли на поле раненные клетки
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (inputGrid[i][j] == 2) //Запоминаем координаты клетки, если она ранена
            {
                x = j;
                y = i;
            }
        }
    }

    //Если не нашли раненную клетку, стреляем в случайную
    if (x == -1 && y == -1)
    {
        numberOfCoord = rand() % inputVector.size(); //Берём случайный номер от нулевого до последнего элемента inputVector
        vector<int> coordElement = inputVector[numberOfCoord]; //Берём вектор с координатой из общего вектора
        x = coordElement[0];
        y = coordElement[1];

        isFinished = MakeMove(x, y, false, moveCondition, inputGrid);

        SetConsoleTextAttribute(hConsole, 14);
        cout << "Ход компьютера: ";
        SetConsoleTextAttribute(hConsole, 7);
        cout << (char)(x + 65) << y + 1 << "\n";

        DisplayGrid(inputGrid, computerGrid, false);
        cout << "\n\n";
    }
    else
    {
        //Записываем состояния клеток по сторонам. Состояние 2 - клетка ранена, состояние 5 - в этом месте поле не существует
        (y - 1 >= 0) ? (nearbyArea[0] = inputGrid[y - 1][x]) : (nearbyArea[0] = 5); //Сверху
        (x + 1 <= 9) ? (nearbyArea[1] = inputGrid[y][x + 1]) : (nearbyArea[1] = 5); //Справа
        (y + 1 <= 9) ? (nearbyArea[2] = inputGrid[y + 1][x]) : (nearbyArea[2] = 5); //Снизу
        (x - 1 >= 0) ? (nearbyArea[3] = inputGrid[y][x - 1]) : (nearbyArea[3] = 5); //Слева

        /*for (int i = 0; i < 4; i++)
        {
            cout << nearbyArea[i] << " ";
        }
        cout << "\n";*/

        //Если вокруг нет раненных клеток, выбираем любую ближайшую
        if (nearbyArea[0] != 2 && nearbyArea[1] != 2 && nearbyArea[2] != 2 && nearbyArea[3] != 2)
        {
            bool isSuccessful = false; //Показывает, получилось ли выбрать клетку для хода и совершить ход

            while (!isSuccessful)
            {
                int angleParameter = rand() % 4; //Генерируем случайный угол выстрела. 0 - вверх, 3 - влево
                    
                switch (angleParameter)
                {
                    case 0:
                        if (nearbyArea[0] == 0 || nearbyArea[0] == 1) //Сверху
                        {
                            isFinished = MakeMove(x, y - 1, false, moveCondition, inputGrid);

                            SetConsoleTextAttribute(hConsole, 14);
                            cout << "Ход компьютера: ";
                            SetConsoleTextAttribute(hConsole, 7);
                            cout << (char)(x + 65) << y << "\n";

                            isSuccessful = true;
                        }
                        break;

                    case 1:
                        if (nearbyArea[1] == 0 || nearbyArea[1] == 1) //Справа
                        {
                            isFinished = MakeMove(x + 1, y, false, moveCondition, inputGrid);

                            SetConsoleTextAttribute(hConsole, 14);
                            cout << "Ход компьютера: ";
                            SetConsoleTextAttribute(hConsole, 7);
                            cout << (char)(x + 66) << y + 1 << "\n";

                            isSuccessful = true;
                        }
                        break;

                    case 2:
                        if (nearbyArea[2] == 0 || nearbyArea[2] == 1) //Снизу
                        {
                            isFinished = MakeMove(x, y + 1, false, moveCondition, inputGrid);

                            SetConsoleTextAttribute(hConsole, 14);
                            cout << "Ход компьютера: ";
                            SetConsoleTextAttribute(hConsole, 7);
                            cout << (char)(x + 65) << y + 2 << "\n";

                            isSuccessful = true;
                        }
                        break;

                    case 3:
                        if (nearbyArea[3] == 0 || nearbyArea[3] == 1) //Слева
                        {
                            isFinished = MakeMove(x - 1, y, false, moveCondition, inputGrid);

                            SetConsoleTextAttribute(hConsole, 14);
                            cout << "Ход компьютера: ";
                            SetConsoleTextAttribute(hConsole, 7);
                            cout << (char)(x + 64) << y + 1 << "\n";

                            isSuccessful = true;
                        }
                        break;

                    default: break;
                }
            }

            DisplayGrid(inputGrid, computerGrid, false); //Отображаем поля обоих игроков после хода
            cout << "\n\n";
        }
        else
        {
            if (nearbyArea[0] == 2) //Если сверху есть раненная клетка
            {
                int i = y - 1;

                //Идём наверх, пока можем
                while (i - 1 >= 0 && inputGrid[i - 1][x] != 4 && inputGrid[i - 1][x] != 3 && inputGrid[i - 1][x] != 2 && !*moveCondition)
                {
                    isFinished = MakeMove(x, i - 1, false, moveCondition, inputGrid);

                    SetConsoleTextAttribute(hConsole, 14);
                    cout << "Ход компьютера: ";
                    SetConsoleTextAttribute(hConsole, 7);
                    cout << (char)(x + 65) << i << "\n";

                    DisplayGrid(inputGrid, computerGrid, false);
                    cout << "\n\n";

                    i--;
                }

                if (inputGrid[y][x] != 3) //Проверяем, потопили ли корабль. Если нет, идём в другую сторону (вниз)
                {
                    i = y;

                    while (i + 1 <= 9 && inputGrid[i + 1][x] != 4 && inputGrid[i + 1][x] != 3 && inputGrid[i + 1][x] != 2 && !*moveCondition)
                    {
                        isFinished = MakeMove(x, i + 1, false, moveCondition, inputGrid);

                        SetConsoleTextAttribute(hConsole, 14);
                        cout << "Ход компьютера: ";
                        SetConsoleTextAttribute(hConsole, 7);
                        cout << (char)(x + 65) << i + 2 << "\n";

                        DisplayGrid(inputGrid, computerGrid, false);
                        cout << "\n\n";

                        i++;
                    }
                }
            }

            if (nearbyArea[1] == 2) //Если справа есть раненная клетка
            {
                int i = x + 1;

                //Идём вправо, пока можем
                while (i + 1 <= 9 && inputGrid[y][i + 1] != 4 && inputGrid[y][i + 1] != 3 && inputGrid[y][i + 1] != 2 && !*moveCondition)
                {
                    isFinished = MakeMove(i + 1, y, false, moveCondition, inputGrid);

                    SetConsoleTextAttribute(hConsole, 14);
                    cout << "Ход компьютера: ";
                    SetConsoleTextAttribute(hConsole, 7);
                    cout << (char)(i + 66) << y + 1 << "\n";

                    DisplayGrid(inputGrid, computerGrid, false);
                    cout << "\n\n";

                    i++;
                }

                if (inputGrid[y][x] != 3) //Если не потопили корабль, идём в другую сторону (влево)
                {
                    i = x;

                    while (i - 1 >= 0 && inputGrid[y][i - 1] != 4 && inputGrid[y][i - 1] != 3 && inputGrid[y][i - 1] != 2 && !*moveCondition)
                    {
                        isFinished = MakeMove(i - 1, y, false, moveCondition, inputGrid);

                        SetConsoleTextAttribute(hConsole, 14);
                        cout << "Ход компьютера: ";
                        SetConsoleTextAttribute(hConsole, 7);
                        cout << (char)(i + 64) << y + 1 << "\n";

                        DisplayGrid(inputGrid, computerGrid, false);
                        cout << "\n\n";

                        i--;
                    }
                }
            }

            if (nearbyArea[2] == 2) //Если снизу есть раненная клетка
            {
                int i = y + 1;

                //Идём вниз, пока можем
                while (i + 1 <= 9 && inputGrid[i + 1][x] != 4 && inputGrid[i + 1][x] != 3 && inputGrid[i + 1][x] != 2 && !*moveCondition)
                {
                    isFinished = MakeMove(x, i + 1, false, moveCondition, inputGrid);

                    SetConsoleTextAttribute(hConsole, 14);
                    cout << "Ход компьютера: ";
                    SetConsoleTextAttribute(hConsole, 7);
                    cout << (char)(x + 65) << i + 2 << "\n";

                    DisplayGrid(inputGrid, computerGrid, false);
                    cout << "\n\n";

                    i++;
                }

                if (inputGrid[y][x] != 3) //Если не потопили корабль, идём в другую сторону (вверх)
                {
                    i = y;

                    while (i - 1 >= 0 && inputGrid[i - 1][x] != 4 && inputGrid[i - 1][x] != 3 && inputGrid[i - 1][x] != 2 && !*moveCondition)
                    {
                        isFinished = MakeMove(x, i - 1, false, moveCondition, inputGrid);

                        SetConsoleTextAttribute(hConsole, 14);
                        cout << "Ход компьютера: ";
                        SetConsoleTextAttribute(hConsole, 7);
                        cout << (char)(x + 65) << i << "\n";

                        DisplayGrid(inputGrid, computerGrid, false);
                        cout << "\n\n";

                        i--;
                    }
                }
            }

            if (nearbyArea[3] == 2) //Если слева есть раненная клетка
            {
                int i = x - 1;

                //Идём влево, пока можем
                while (i - 1 >= 0 && inputGrid[y][i - 1] != 4 && inputGrid[y][i - 1] != 3 && inputGrid[y][i - 1] != 2 && !*moveCondition)
                {
                    isFinished = MakeMove(i - 1, y, false, moveCondition, inputGrid);

                    SetConsoleTextAttribute(hConsole, 14);
                    cout << "Ход компьютера: ";
                    SetConsoleTextAttribute(hConsole, 7);
                    cout << (char)(i + 64) << y + 1 << "\n";

                    DisplayGrid(inputGrid, computerGrid, false);
                    cout << "\n\n";

                    i++;
                }

                if (inputGrid[y][x] != 3) //Если не потопили корабль, идём в другую сторону (вправо)
                {
                    i = x;

                    while (i + 1 <= 9 && inputGrid[y][i + 1] != 4 && inputGrid[y][i + 1] != 3 && inputGrid[y][i + 1] != 2 && !*moveCondition)
                    {
                        isFinished = MakeMove(i + 1, y, false, moveCondition, inputGrid);

                        SetConsoleTextAttribute(hConsole, 14);
                        cout << "Ход компьютера: ";
                        SetConsoleTextAttribute(hConsole, 7);
                        cout << (char)(i + 66) << y + 1 << "\n";

                        DisplayGrid(inputGrid, computerGrid, false);
                        cout << "\n\n";

                        i--;
                    }
                }
            }
        }
    }

    //Удаляем ненужные координаты
    for (int i = 0; i < 10; i++) 
    {
        for (int j = 0; j < 10; j++)
        {
            if (inputGrid[i][j] == 4 || inputGrid[i][j] == 3 || inputGrid[i][j] == 2)
            {
                vector<int> toRemove = { j, i };

                for (int k = 0; k < inputVector.size(); k++)
                {
                    if (toRemove == inputVector[k])
                    {
                        inputVector.erase(inputVector.begin() + k);
                    }
                }
            }
        }
    }

    return isFinished;
}