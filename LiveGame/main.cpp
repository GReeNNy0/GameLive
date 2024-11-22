#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

char** createSpace(int rows, int colums); //создание поля
void fillSpace_(char** arr, int rows, int colums);//заполнение поля -
void fillSpaceLive(char** arr, int rows, int colums); // заполнение поля жизнью
void printSpace(char** arr, int rows, int colums); // вывод поля
void DeleteSpace(char** array, int rows); // очистка
int alliveCellsCounter(char** array, int rows, int colums); //подсчет живых клеток
void newGeneration(char** arr, int rows, int colums); // анализ клеток
int countNeighbors(char** arr, int rows, int cols, int i, int j); //подсчет количества соседей
void newGeneration(char** arr, int rows, int cols); //заполнение нового поколения 
void copyArray(char** arr1, char** arr2, int rows, int cols); // копирование массива
bool uslovie(char** arr1, char** arr2, int rows, int cols); // условие прекращения игры (равенство двух поколений )


int main() {
    int rows{}, colums{}, gen{ 1 };
    int Alivecells{};
    setlocale(LC_ALL, "ru");
    std::ifstream f("C:\\Users\\21\\Desktop\\file for c\\file.txt");
    if (!f) {
        std::cerr << "Файл не открылся" << std::endl;
        return 1;
    }

    f >> rows >> colums;

    char** pokolenie = createSpace(rows, colums);
    char** previospokolenie = createSpace(rows, colums);
    fillSpace_(pokolenie, rows, colums);

    int i, j;
    while (f >> i >> j) {
        if (i >= 0 && i < rows && j >= 0 && j < colums) {
            fillSpaceLive(pokolenie, i, j);
        }
    }

    std::cout << "Игра началась. Нажмите Enter" << std::endl;

    Alivecells = alliveCellsCounter(pokolenie, rows, colums);
    while (Alivecells > 0)
    {

        std::cout << "Поколение " << gen << ":" << std::endl;
        printSpace(pokolenie, rows, colums);
        Alivecells = alliveCellsCounter(pokolenie, rows, colums);
        std::cout << "Количество живых клеток: " << Alivecells << std::endl;
        std::cout << "Нажмите Enter" << std::endl;
        std::cin.get(); // Ожидает нажатия Enter
        copyArray(pokolenie, previospokolenie, rows, colums);
        newGeneration(pokolenie, rows, colums);
        gen++;
        system("cls");
        if (Alivecells <= 0)
        {
            printSpace(pokolenie, rows, colums);
            std::cout << "Game over,все вымерли" << "\n" << "Поколение " << gen << "\t" << "Количество живых клеток: " << Alivecells << std::endl;
            break;
        }
        else if (uslovie(pokolenie, previospokolenie, rows, colums))
        {
            printSpace(pokolenie, rows, colums);
            std::cout << "Game over,мир остановился" << "\n" << "Поколение " << gen << "\t" << "Количество живых клеток: " << Alivecells << std::endl;
            break;
        }


    }


    DeleteSpace(pokolenie, rows);
    return 0;
}

char** createSpace(int rows, int colums)
{
    char** arr = new char* [rows];
    for (int i = 0; i < rows; ++i)
    {
        arr[i] = new char[colums];
    }
    return arr;
}

void fillSpace_(char** arr, int rows, int colums)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < colums; ++j)
        {
            arr[i][j] = '-';
        }
    }
}

void fillSpaceLive(char** arr, int i, int j)
{
    arr[i][j] = '*';
}

void printSpace(char** arr, int rows, int colums)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < colums; ++j)
        {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void DeleteSpace(char** array, int rows)
{
    for (int i = 0; i < rows; ++i)
    {
        delete[] array[i];
    }
    delete[] array;
}
int alliveCellsCounter(char** arr, int rows, int colums)
{
    int counter{};
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < colums; ++j)
        {
            if (arr[i][j] == '*')
            {
                counter++;
            }
        }
    }
    return counter;
}

int countNeighbors(char** arr, int rows, int cols, int i, int j)
{
    std::vector<std::pair<int, int>> directions =
    {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };

    int counter = 0;
    for (const auto& direction : directions)
    {
        int ni = i + direction.first;
        int nj = j + direction.second;

        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && arr[ni][nj] == '*')
        {
            counter++;
        }
    }
    return counter;
}
void newGeneration(char** arr, int rows, int cols)
{
    char** newArr = createSpace(rows, cols);
    fillSpace_(newArr, rows, cols); // Заполнение новыми значениями

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            int liveNeighbors = countNeighbors(arr, rows, cols, i, j);

            if (arr[i][j] == '*' && (liveNeighbors == 2 || liveNeighbors == 3))
            {
                newArr[i][j] = '*';
            }
            else if (arr[i][j] == '-' && liveNeighbors == 3)
            {
                newArr[i][j] = '*';
            }
            else
            {
                newArr[i][j] = '-';
            }
        }
    }

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            arr[i][j] = newArr[i][j];
        }
    }

    DeleteSpace(newArr, rows);
}
void copyArray(char** arr1, char** arr2, int rows, int cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j) {
            arr2[i][j] = arr1[i][j];
        }
    }
}
bool uslovie(char** arr1, char** arr2, int rows, int cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j) {
            if (arr2[i][j] != arr1[i][j])
            {
                return false;
            }
        }
    }
    return true;
}