#pragma once
#include <fstream>
#include <vector>

using namespace std;


/*   ¬—ѕќћќ√ј“≈Ћ№Ќџ≈ ‘”Ќ ÷»» ƒЋя ќ—Ќќ¬Ќџ’ »« CORE.CPP   */

//”дал¤ет элемент, принадлежащий кораблю с данными координатами из вектора с координатами
void RemoveElement(int x, int y, vector<vector<int>>& inputVector);

//ѕровер¤ет, существует ли элемент по данным координатам в данном векторе
bool ElementCondition(int x, int y, vector<vector<int>> inputVector);

//–азмещает корабль на поле
void PlaceShip(int dimension, int count, vector<vector<int>>& inputVector, int(*inputGrid)[10]);

//«аполн¤ет ближайшие к потопленным корабл¤м клетки синим цветом
void FillNearbyArea(int(*inputGrid)[10]);