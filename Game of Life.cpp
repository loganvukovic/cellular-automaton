#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>
#pragma execution_character_set("CP_UTF8")

using namespace std;

//Initialize the console for UTF-8 output
void setupConsole() 
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

//Output Unicode characters using WriteConsoleW
void renderUnicodeBoard(vector<vector<int>> board) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int i = 0; i < board.size(); ++i) 
    {
        for (int j = 0; j < board[i].size(); ++j) 
        {
            if (board[i][j] == 1) 
            {
                //Render 1 as ■
                const wchar_t blockChar[] = L"\u25A0";
                WriteConsoleW(hConsole, blockChar, wcslen(blockChar), nullptr, nullptr);
            }
            else 
            {
                const wchar_t spaceChar[] = L" ";
                WriteConsoleW(hConsole, spaceChar, wcslen(spaceChar), nullptr, nullptr);
            }
        }
        const wchar_t newline[] = L"\n";
        WriteConsoleW(hConsole, newline, wcslen(newline), nullptr, nullptr);
    }
}

vector<vector<int>> DeadState(int height, int width) 
{
    vector<vector<int>> deadBoard(height, vector<int>(width, 0));
    return deadBoard;
}

vector<vector<int>> RandomState(int height, int width) 
{
    vector<vector<int>> randomBoard = DeadState(height, width);
    srand(time(0));

    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            if (rand() % 2 == 1) 
            {
                randomBoard[i][j] = 1;
            }
        }
    }
    return randomBoard;
}

int main() 
{
    setupConsole();

    int width = 60;
    int height = 30;

    renderUnicodeBoard(RandomState(height, width));
}
