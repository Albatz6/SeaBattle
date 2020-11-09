#pragma once
#include <fstream>
#include <vector>

using namespace std;


/*   ќ—Ќќ¬Ќџ≈ ‘”Ќ ÷»»   */

//—читывает пользовательскую расстановку кораблей матрицей из 0 и 1 в двумерный массив
void ReadGridFromFile(ifstream& inputFile, int(*input)[10]);

//√енерирует матрицу, которую компьютер использует дл¤ игры
void GenerateGrid(int (*input)[10]);

//‘иксирует ход игрока и измен¤ет матрицу в соответствии с ним
bool MakeMove(int row, int column, bool isHuman, bool* moveCondition, int (*inputGrid)[10]);

//–еализует ход компьютера
bool ComputerMove(int(*inputGrid)[10], int(*computerGrid)[10], vector<vector<int>>& inputVector, bool* moveCondition, int (*nearbyArea));


/*   √–ј‘» ј   */

//¬ыводит двумерный массив с расстановкой в ASCII-графике
void DisplayGrid(int userGrid[10][10], int generatedGrid[10][10], bool displayFullComputerGrid);