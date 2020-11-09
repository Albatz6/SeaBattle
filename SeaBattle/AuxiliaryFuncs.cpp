#include <fstream>
#include <ctime>
#include <vector>
#include <random>
//#include <algorithm>

using namespace std;


/*   ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ ОСНОВНЫХ ИЗ CORE.CPP   */

//Сравнивает два вектора
/*bool compare(vector<int>& v1, std::vector<int>& v2)
{
    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());
    return v1 == v2;
}*/

//Удаляет элемент, принадлежащий кораблю с данными координатами из вектора с координатами
void RemoveElement(int x, int y, vector<vector<int>>& inputVector)
{
    vector<int> centralElement = { x, y };
    //cout << "x: " << x << " y: " << y << endl;

    vector<int> upperElement = { x, y - 1 };
    //cout << "x: " << x << " y: " << y - 1 << endl;

    vector<int> lowerElement = { x, y + 1 };
    //cout << "x: " << x << " y: " << y + 1 << endl;

    vector<int> leftElement = { x - 1, y };
    //cout << "x: " << x - 1 << " y: " << y << endl;

    vector<int> rightElement = { x + 1, y };
    //cout << "x: " << x + 1 << " y: " << y << endl;

    vector<int> upperLeftElement = { x - 1, y - 1 };
    //cout << "x: " << x - 1 << " y: " << y - 1 << endl;

    vector<int> upperRightElement = { x + 1, y - 1 };
    //cout << "x: " << x + 1 << " y: " << y - 1 << endl;

    vector<int> lowerLeftElement = { x - 1, y + 1 };
    //cout << "x: " << x - 1 << " y: " << y + 1 << endl;

    vector<int> lowerRightElement = { x + 1, y + 1 };
    //cout << "x: " << x + 1 << " y: " << y + 1 << endl;

    //centralElement
    for (int i = 0; i < inputVector.size(); i++)
    {
        if (centralElement == inputVector[i])
        {
            inputVector.erase(inputVector.begin() + i);
        }
    }

    //upperElement
    for (int i = 0; i < inputVector.size(); i++)
    {
        if (upperElement == inputVector[i])
        {
            inputVector.erase(inputVector.begin() + i);
        }
    }

    //lowerElement
    for (int i = 0; i < inputVector.size(); i++)
    {
        if (lowerElement == inputVector[i])
        {
            inputVector.erase(inputVector.begin() + i);
        }
    }

    //leftElement
    for (int i = 0; i < inputVector.size(); i++)
    {
        if (leftElement == inputVector[i])
        {
            inputVector.erase(inputVector.begin() + i);
        }
    }

    //rightElement
    for (int i = 0; i < inputVector.size(); i++)
    {
        if (rightElement == inputVector[i])
        {
            inputVector.erase(inputVector.begin() + i);
        }
    }

    //upperLeftElement
    for (int i = 0; i < inputVector.size(); i++)
    {
        if (upperLeftElement == inputVector[i])
        {
            inputVector.erase(inputVector.begin() + i);
        }
    }

    //upperRightElement
    for (int i = 0; i < inputVector.size(); i++)
    {
        if (upperRightElement == inputVector[i])
        {
            inputVector.erase(inputVector.begin() + i);
        }
    }

    //lowerLeftElement
    for (int i = 0; i < inputVector.size(); i++)
    {
        if (lowerLeftElement == inputVector[i])
        {
            inputVector.erase(inputVector.begin() + i);
        }
    }

    //lowerRightElement
    for (int i = 0; i < inputVector.size(); i++)
    {
        if (lowerRightElement == inputVector[i])
        {
            inputVector.erase(inputVector.begin() + i);
        }
    }
}

//Проверяет, существует ли элемент по данным координатам в данном векторе
bool ElementCondition(int x, int y, vector<vector<int>> inputVector)
{
    vector<int> element = { x, y };

    for (int i = 0; i < inputVector.size(); i++)
    {
        if (element == inputVector[i])
        {
            return true;
        }
    }

    return false;
}

//Размещает корабль на поле
//dimension - размерность корабля
//count - количество кораблей
//inputVector - вектор с координатами
//inputGrid - матрица, на которой размещается корабль
void PlaceShip(int dimension, int count, vector<vector<int>>& inputVector, int(*inputGrid)[10])
{
    srand(time(NULL)); //Для генерации случайных чисел с сидом на времени

    int numberOfCoord; //Хранит порядковый номер случайной координаты в векторе
    int angleParameter; //Определяет угол поворота корабля. 0 - вверх; 1 - вправо; 2 - вниз; 3 - влево
    int x, y; //Координаты по Х и Y
    bool isSuccessful; //Флаг, определяющий, получилось ли поставить корабль

    for (int i = 0; i < count; i++)
    {
        //Генерирует случайное число от нуля до длины входного вектора по формуле rand() % (b - a + 1) + a
        //где b = vector.size() - 1; где а = 0;
        numberOfCoord = rand() % inputVector.size();

        vector<int> coordElement = inputVector[numberOfCoord]; //Берём вектор с координатой из общего вектора
        isSuccessful = false;
        x = coordElement[0];
        y = coordElement[1];

        while (!isSuccessful)
        {
            angleParameter = rand() % 4;

            switch (angleParameter) //Подбор позиции корабля
            {
            case 0: //Направление вверх
                //cout << x << "  " << (y - (dimension - 1)) << endl;
                if ((y - ((dimension - 1)) >= 0) && (ElementCondition(x, (y - (dimension - 1)), inputVector)))
                {
                    for (int i = 0; i < dimension; i++)
                    {
                        inputGrid[y - i][x] = 1;
                        RemoveElement(x, y - i, inputVector);
                    }

                    isSuccessful = true;
                }
                break;

            case 1: //Направление направо
                //cout << (x + (dimension - 1)) << "  " << y << endl;
                if ((x + (dimension - 1) <= 9) && (ElementCondition((x + (dimension - 1)), y, inputVector)))
                {
                    for (int i = 0; i < dimension; i++)
                    {
                        inputGrid[y][x + i] = 1;
                        RemoveElement(x + i, y, inputVector);
                    }

                    isSuccessful = true;
                }
                break;

            case 2: //Направление вниз
                //cout << x << "  " << (y + (dimension - 1)) << endl;
                if ((y + (dimension - 1) <= 9) && (ElementCondition(x, (y + (dimension - 1)), inputVector)))
                {
                    for (int i = 0; i < dimension; i++)
                    {
                        inputGrid[y + i][x] = 1;
                        RemoveElement(x, y + i, inputVector);
                    }

                    isSuccessful = true;
                }
                break;

            case 3: //Направление налево
                //cout << (x - (dimension - 1)) << "  " << y << endl;
                if ((x - (dimension - 1) >= 0) && (ElementCondition((x - (dimension - 1)), y, inputVector)))
                {
                    for (int i = 0; i < dimension; i++)
                    {
                        inputGrid[y][x - i] = 1;
                        RemoveElement(x - i, y, inputVector);
                    }

                    isSuccessful = true;
                }
                break;

            default: break;
            }
        }
    }
}

//Заполняет ближайшие к потопленным кораблям клетки синим цветом
void FillNearbyArea(int(*inputGrid)[10])
{
    for (int i = 0; i < 10; i++) //Раскраска окрестностей в синий цвет
    {
        for (int j = 0; j < 10; j++)
        {
            if (inputGrid[i][j] == 3)
            {
                if (i - 1 >= 0 && inputGrid[i - 1][j] != 3)
                {
                    inputGrid[i - 1][j] = 4;

                    if (j - 1 >= 0)
                    {
                        inputGrid[i - 1][j - 1] = 4;
                    }

                    if (j + 1 <= 9)
                    {
                        inputGrid[i - 1][j + 1] = 4;
                    }
                }

                if (i + 1 <= 9 && inputGrid[i + 1][j] != 3)
                {
                    inputGrid[i + 1][j] = 4;

                    if (j - 1 >= 0)
                    {
                        inputGrid[i + 1][j - 1] = 4;
                    }

                    if (j + 1 <= 9)
                    {
                        inputGrid[i + 1][j + 1] = 4;
                    }
                }

                if (j - 1 >= 0 && inputGrid[i][j - 1] != 3)
                {
                    inputGrid[i][j - 1] = 4;
                }

                if (j + 1 <= 9 && inputGrid[i][j + 1] != 3)
                {
                    inputGrid[i][j + 1] = 4;
                }
            }
        }
    }
}