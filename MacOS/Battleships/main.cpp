//
//  main.cpp
//  Battleships
//
//  Created by albatz on 24.10.2020.
//

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include "CoreFuncs.hpp"
#include <vector>

using namespace std;


#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

int main() {
    setlocale(LC_ALL,"Russian_Russia.1251"); //Для поддержки вывода кириллицы

    ifstream file; //Файл пользовательской расстановки, если она будет
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
            cout << GREEN << "==Файл открыт==\n\n" << RESET;
        }

        ReadGridFromFile(file, userGridPointer); //Для тестового запуска закомментировать
    }
    else
    {
        GenerateGrid(userGridPointer); //Иначе генерируем расстановку
        usleep(1000); //Ждём, чтобы была разница по времени для использования в сидах при генерации расстановок
    }

    GenerateGrid(genGridPointer);

    cout << "Правила игры\n";
    cout << "============\n";
    cout << "Первый ход делаете вы, затем ход делает компьютер.\n";
    cout << "В случае попадания в корабль право следующего хода получает игрок, совершивший попадание.\n";
    cout << "Слева будет отображаться ваше поле, а справа - поле компьютера.\n";
    cout << "Чтобы сделать ход, нужно вписать ячейку, в которую должен быть нанесён выстрел.\n";
    cout << "Например: G4\n\n";

    cout << "Обозначения\n";
    cout << "===========\n";
    cout << CYAN << "#" << RESET;
    cout << " - корабль не тронут\n";

    cout << "\033[43m" << "#" << RESET;
    cout << " - корабль ранен\n";

    cout << "\033[7;49;31m" << "#" << RESET;
    cout << " - корабль потоплен\n";

    cout << "\033[7;49;34m" << "#" << RESET;
    cout << " - неактивная клетка\n\n\n";
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
                    cout << RED << "Неверный формат введённой ячейки\n" << RESET;
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
        cout << MAGENTA << "Вы победили!\n\n" << RESET;
    }
    else
    {
        cout << MAGENTA << "Электронный мир победил!\n\n" << RESET;

        cout << "Итоговое поле: \n";
        DisplayGrid(userGrid, generatedGrid, true);
        cout << "\n\n";
    }

    file.close();
}
