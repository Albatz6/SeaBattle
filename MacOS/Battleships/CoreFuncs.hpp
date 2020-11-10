//
//  CoreFuncs.hpp
//  Battleships
//
//  Created by albatz on 24.10.2020.
//

#ifndef CoreFuncs_hpp
#define CoreFuncs_hpp

#include <stdio.h>
#include <fstream>
#include <vector>

using namespace std;


/*   ОСНОВНЫЕ ФУНКЦИИ   */

//Считывает пользовательскую расстановку кораблей матрицей из 0 и 1 в двумерный массив
void ReadGridFromFile(ifstream& inputFile, int(*input)[10]);

//Генерирует матрицу, которую компьютер использует для игры
void GenerateGrid(int (*input)[10]);

//Фиксирует ход игрока и изменяет матрицу в соответствии с ним
bool MakeMove(int row, int column, bool isHuman, bool* moveCondition, int (*inputGrid)[10]);

//Реализует ход компьютера
bool ComputerMove(int(*inputGrid)[10], int(*computerGrid)[10], vector<vector<int>>& inputVector, bool* moveCondition, int (*nearbyArea));


/*   ГРАФИКА   */

//Выводит двумерный массив с расстановкой в ASCII-графике
void DisplayGrid(int userGrid[10][10], int generatedGrid[10][10], bool displayFullComputerGrid);
#endif /* CoreFuncs_hpp */
