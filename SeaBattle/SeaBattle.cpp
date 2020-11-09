// SeaBattle.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "CoreFuncs.h"
#include <vector>

using namespace std;


int main()
{
    #pragma region Начальные параметры программы
    setlocale(LC_ALL, "Russian"); //Для поддержки вывода кириллицы

    ifstream file; //Файл пользовательской расстановки, если она будет
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Для управления цветом текста в консоли
    string filename, startOption; //startOption - хранит ответ пользователя на вопрос об автоматической генерации расстановки
    
    bool isFinished = false; //Флаг состояния игры. True будет значить, что игра закончена
    bool moveCondition = true; //Флаг состояния хода. True означает, что ход делает пользователь, False - компьютер

    int nearbyArea[4] = {-1, -1, -1, -1}; //Массив для обозначения состояния клеток в четырех направлениях от данной. Нужен для реализации хода компьютера 
    
    int generatedGrid[10][10]; //Матрица с расстановкой кораблей компьютера
    int userGrid[10][10]; //Матрица с расстановкой кораблей пользователя

    vector<vector<int>> userGridCoords; //Хранит все координаты поля пользователя, в которые компьютер может ходить
    vector<int> lastCoords = { -1, -1, -1 }; //Хранит данные о последнем выстреле пользователя (х, y, номер координаты в userGridCoords)

    int (*genGridPointer)[10] = generatedGrid; //Указатели на массивы
    int (*userGridPointer)[10] = userGrid;

    #pragma region Для тестов
    //Заполняем вектор векторами с координатами каждой отдельной клетки поля пользователя
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            vector<int> element;

            element.push_back(i);
            element.push_back(j);

            userGridCoords.push_back(element);
        }
    }

    for (int i = 0; i < 10; i++) //Для тестов
    {
        for (int j = 0; j < 10; j++)
        {
            userGrid[i][j] = 0;
        }
    }

    userGrid[0][0] = 1;
    userGrid[1][0] = 1;
    userGrid[2][0] = 1;
    userGrid[3][0] = 1;

    userGrid[5][0] = 1;
    userGrid[6][0] = 1;
    userGrid[7][0] = 1;

    userGrid[9][0] = 1;
    userGrid[9][1] = 1;

    userGrid[7][4] = 1;
    userGrid[7][5] = 1;

    userGrid[0][9] = 1;
    userGrid[1][9] = 1;
    userGrid[2][9] = 1;
    #pragma endregion
    #pragma endregion


    cout << "Морской бой\n";
    cout << "===========\n";

    bool isCorrectOption = false;
    while (!isCorrectOption)
    {
        cout << "Использовать расстановку из файла? [Y/n] ";
        cin >> startOption;

        if (startOption == "y" || startOption == "Y" || startOption == "n" || startOption == "N")
        {
            isCorrectOption = true;
            cout << "\n";
        }
    }

    if (startOption == "y" || startOption == "Y") //Если согласились подгружать расстановку из файла
    {
        cout << "Введите имя файла с расстановкой: ";
        cin >> filename;
        cout << "\n";

        ifstream file(filename);

        //Проверка открытия файла (для тестового запуска закомментировать)
        if (!file)
        {
            cout << "Не удалось открыть указанный файл\n\n";
            system("pause");
            exit(1);
        }
        else
        {
            SetConsoleTextAttribute(hConsole, 10); //Зелёный цвет текста
            cout << "==Файл открыт==\n\n";
            SetConsoleTextAttribute(hConsole, 7);
        }

        ReadGridFromFile(file, userGridPointer); //Для тестового запуска закомментировать
    }
    else
    {
        GenerateGrid(userGridPointer); //Иначе генерируем расстановку
        Sleep(1000); //Ждём, чтобы была разница по времени для использования в сидах при генерации расстановок
    }

    GenerateGrid(genGridPointer);

    #pragma region Выведение правил игры
    cout << "Правила игры\n";
    cout << "============\n";
    cout << "Первый ход делаете вы, затем ход делает компьютер.\n";
    cout << "В случае попадания в корабль право следующего хода получает игрок, совершивший попадание.\n";
    cout << "Слева будет отображаться ваше поле, а справа - поле компьютера.\n";
    cout << "Чтобы сделать ход, нужно вписать ячейку, в которую должен быть нанесён выстрел.\n";
    cout << "Например: G4\n\n";
    
    cout << "Обозначения\n";
    cout << "===========\n";
    SetConsoleTextAttribute(hConsole, 11); //Описания цветов по номерам находятся в DisplayGrid в Core.cpp
    cout << "#";
    SetConsoleTextAttribute(hConsole, 7);
    cout << " - корабль не тронут\n";

    SetConsoleTextAttribute(hConsole, 111);
    cout << "#";
    SetConsoleTextAttribute(hConsole, 7);
    cout << " - корабль ранен\n";

    SetConsoleTextAttribute(hConsole, 79);
    cout << "#";
    SetConsoleTextAttribute(hConsole, 7);
    cout << " - корабль потоплен\n";

    SetConsoleTextAttribute(hConsole, 16);
    cout << "#";
    SetConsoleTextAttribute(hConsole, 7);
    cout << " - неактивная клетка\n\n\n";
    #pragma endregion
    DisplayGrid(userGrid, generatedGrid, false);
    cout << "\n\n";

    //Игровой процесс
    while (!isFinished)
    {
        int row, column; //Координаты ячейки, которую укажет пользователь
        bool isValidPosition = false; //Обозначает, является ли введенная пользователем позиция существующей
        string position;

        if (moveCondition) //Ход делает пользователь
        {
            while (!isValidPosition) //Проверка правильности введённой пользователем позиции
            {
                cout << "Введите ячейку для выстрела: ";
                cin >> position;

                column = ((int)(toupper(position[0]))) - 65; //Перевод символов от A до J в 0-10 по коду ASCII

                if (position.length() == 3) //Перевод числовой координаты клетки для выстрела
                {
                    row = atoi((position.substr(1, 2)).c_str()) - 1; //Если ввели координату со значением 10

                }
                else
                {
                    row = ((int)position[1]) - 49;
                }

                if (column <= 10 && column >= 0 && row <= 10 && row >= 0) //Проверка правильности диапазона введенный координат
                {
                    isValidPosition = true;
                }
                else
                {
                    SetConsoleTextAttribute(hConsole, 12); //Красный цвет текста
                    cout << "Неверный формат введённой ячейки\n";
                    SetConsoleTextAttribute(hConsole, 7);
                }
            }

            isFinished = MakeMove(column, row, true, &moveCondition, genGridPointer);
            DisplayGrid(userGrid, generatedGrid, false);
            cout << "\n\n";
        }
        else //Ход делает компьютер
        {
            while (!moveCondition && !isFinished) //Если компьютер попал в корабль, делаем ходы, пока флаг хода позволяет
            {
                isFinished = ComputerMove(userGridPointer, genGridPointer, userGridCoords, &moveCondition, nearbyArea);
            }
        }
    }

    //Поздравление пользователя/компьютера с победой
    if (moveCondition == true)
    {
        SetConsoleTextAttribute(hConsole, 13); //Устанавливает фиолетовый цвет текста консоли
        cout << "Вы победили!\n\n";
        SetConsoleTextAttribute(hConsole, 7);
    }
    else
    {
        SetConsoleTextAttribute(hConsole, 13);
        cout << "Электронный мир победил!\n\n";
        SetConsoleTextAttribute(hConsole, 7);

        cout << "Итоговое поле: \n";
        DisplayGrid(userGrid, generatedGrid, true);
        cout << "\n\n";
    }

    file.close();
    system("pause");
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.