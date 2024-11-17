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

vector<vector<int>> nextState(vector<vector<int>> board)
{
    vector<vector<int>> nextState(board.size() - 1, vector<int>(board[0].size() - 1, 0));
    int curNeighbor = 0;

    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board.size(); j++)
        {
            //Count Neighbors
            //
            if (i != 0)
            {
                if (board[i - 1][j] == 1) curNeighbor++;
                if (j != 0 && board[i - 1][j - 1] == 1) curNeighbor++;
                if (j != board[i].size() - 1 && board[i - 1][j + 1] == 1) curNeighbor++;
            }
            if (i != board.size() - 1)
            {
                if (board[i + 1][j] == 1) curNeighbor++;
                if (j != 0 && board[i + 1][j - 1] == 1) curNeighbor++;
                if (j != board[i].size() - 1 && board[i + 1][j + 1] == 1) curNeighbor++;
            }
            if (j != 0)
            {
                if (board[i][j - 1] == 1) curNeighbor++;
            }
            if (j != board[i].size() - 1)
            {
                if (board[i][j + 1] == 1) curNeighbor++;
            }
            cout << curNeighbor;
            curNeighbor = 0;
        }
    }

    return nextState;
}


//Output Unicode characters using WriteConsoleW
void renderBoard(vector<vector<int>> board) 
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
    cout << endl;

    //renderBoard(nextState(board));
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

    int width = 5;
    int height = 5;


    vector<vector<int>> board = RandomState(height,width);
    renderBoard(board);
    nextState(board);
}
