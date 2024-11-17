#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>
#pragma execution_character_set("CP_UTF8")
using namespace std;

void renderBoard(vector<vector<int>>);

//Initialize the console for UTF-8 output
void setupConsole() 
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

//Calculate next state
vector<vector<int>> nextState(vector<vector<int>> neighbors, vector<vector<int>> originalState, int height, int width)
{
    vector<vector<int>> nextState(neighbors.size(), vector<int>(neighbors[0].size(), 0));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //If a cell is dead and has three neighbors, it comes to life
            if (originalState[i][j] == 0 && neighbors[i][j] == 3)
                nextState[i][j] = 1;
            //If a cell is alive...
            else if (originalState[i][j] == 1)
            {
                //... and has few neighbors, it dies to underpopulation
                if (neighbors[i][j] == 0 || neighbors[i][j] == 1)
                    nextState[i][j] = 0;
                //...and has the right number of neighbors, it survives
                else if (neighbors[i][j] == 2 || neighbors[i][j] == 3)
                    nextState[i][j] = 1;
                //...and has too many neighbors, it dies to overpopulation
                else if (neighbors[i][j] > 3)
                    nextState[i][j] = 0;
            }
        }
    }
    Sleep(300);
    renderBoard(nextState);
    return nextState;
}

void CountNeighbors(vector<vector<int>> board, int height, int width)
{
    vector<vector<int>> neighborMatrix(board.size(), vector<int>(board[0].size(), 0));
    int curNeighbor = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Top three neighbors
            if (i != 0)
            {
                if (board[i - 1][j] == 1) curNeighbor++;
                if (j != 0 && board[i - 1][j - 1] == 1) curNeighbor++;
                if (j != board[i].size() - 1 && board[i - 1][j + 1] == 1) curNeighbor++;
            }
            //Bottom three neighbors
            if (i != board.size() - 1)
            {
                if (board[i + 1][j] == 1) curNeighbor++;
                if (j != 0 && board[i + 1][j - 1] == 1) curNeighbor++;
                if (j != board[i].size() - 1 && board[i + 1][j + 1] == 1) curNeighbor++;
            }
            //Direct left neighbor
            if (j != 0)
            {
                if (board[i][j - 1] == 1) curNeighbor++;
            }
            //Direct right neighbor
            if (j != board[i].size() - 1)
            {
                if (board[i][j + 1] == 1) curNeighbor++;
            }
            neighborMatrix[i][j] = curNeighbor;
            curNeighbor = 0;
        }
    }

    nextState(neighborMatrix, board, height, width);
}

//Output Unicode characters using WriteConsoleW
void renderBoard(vector<vector<int>> board) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << endl;

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
        if (i != board.size() - 1)
        {
            const wchar_t newline[] = L"\n";
            WriteConsoleW(hConsole, newline, wcslen(newline), nullptr, nullptr);
        }
    }

    CountNeighbors(board, board.size(), board[0].size());
}

vector<vector<int>> DeadState(int height, int width) 
{
    vector<vector<int>> deadBoard(height, vector<int>(width, 0));
    return deadBoard;
}

//Generate random starting state
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


    vector<vector<int>> board = RandomState(height,width);
    renderBoard(board);
}
