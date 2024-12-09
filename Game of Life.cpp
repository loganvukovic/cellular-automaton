#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>
#include <fstream>
using namespace std;

void renderBoard(vector<vector<int>>, int, int, char, int);

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
    renderBoard(nextState, 0, 0, 'N', 1);
    return nextState;
}

vector<vector<int>> langtonAntNext(vector<vector<int>> originalState, int height, int width, int antX, int antY, char antDirection)
{
    vector<vector<int>> nextState(originalState.size(), vector<int>(originalState[0].size(), 0));
    nextState = originalState;

    if (nextState[antY][antX] == 0)
    {
        nextState[antY][antX] = 1;
        switch (antDirection)
        {
        case 'N':
            antDirection = 'E';
            break;
        case 'E':
            antDirection = 'S';
            break;
        case 'S':
            antDirection = 'W';
            break;
        case 'W':
            antDirection = 'N';
            break;
        }
    }
    else
    {
        nextState[antY][antX] = 0;
        switch (antDirection)
        {
        case 'N':
            antDirection = 'W';
            break;
        case 'W':
            antDirection = 'S';
            break;
        case 'S':
            antDirection = 'E';
            break;
        case 'E':
            antDirection = 'N';
            break;
        }
    }

    switch (antDirection)
    {
    case 'N':
        antY--;
        break;
    case 'W':
        antX--;
        break;
    case 'S':
        antY++;
        break;
    case 'E':
        antX++;
        break;
    }
    Sleep(300);
    renderBoard(nextState, antX, antY, antDirection, 2);
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
void renderBoard(vector<vector<int>> board, int antX, int antY, char antDirection, int rule) 
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
                const wchar_t spaceChar[] = L"  ";
                WriteConsoleW(hConsole, spaceChar, wcslen(spaceChar), nullptr, nullptr);
            }
        }
        if (i != board.size() - 1)
        {
            const wchar_t newline[] = L"\n";
            WriteConsoleW(hConsole, newline, wcslen(newline), nullptr, nullptr);
        }
    }

    switch (rule)
    {
    case 1:
        CountNeighbors(board, board.size(), board[0].size());
        break;
    case 2:
        langtonAntNext(board, board.size(), board[0].size(), antX, antY, antDirection);
        break;
    }
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

void loadFromFile()
{
    fstream file("input.txt");
    int column = 0;
    int maxColumn = 0;
    int row = 0;
    char c;

    vector<vector<int>> fileBoard;
    fileBoard.push_back({});
        
    while (file.get(c))
    {
        if (c == '\n')
        {
            if (row > 0 && column < maxColumn)
            {
                cout << "Invalid input. Proceeding with random life generation.\n";
                Sleep(3000);
                return;
            }
            fileBoard.push_back({});
            row++;
            maxColumn = column;
            column = 0;
        }
        else if (c == '0' || c == '1')
        {
            if (row > 0 && column >= maxColumn)
            {
                cout << "Invalid input. Proceeding with random life generation.\n";
                Sleep(3000);
                return;
            }
            int x = c - '0';
            fileBoard[row].push_back(x);
            column++;
        }
        else
        {
            cout << "Invalid input. Proceeding with random life generation.\n";
            Sleep(3000);
            return;
        }
    }
    file.close();
    if (column < maxColumn || column > maxColumn)
    {
        cout << "Invalid input. Proceeding with random life generation.\n";
        Sleep(3000);
        return;
    }

    renderBoard(fileBoard, 0, 0, 'N', 1);
}

void StartLife(int rule)
{
    fstream file("input.txt");
    //Check if file is empty
    if (file.peek() != std::ifstream::traits_type::eof())
        loadFromFile();

    //If no input provided, proceed with random generation

    int width = 58;
    int height = 30;

    cout << "Enter height: ";
    cin >> height;
    cout << "Enter width: ";
    cin >> width;
    cout << endl;
    vector<vector<int>> board = RandomState(height, width);
    CountNeighbors(board, height, width);
}

void StartAnt(int rule)
{
    int height = 50;
    int width = 50;
    int antX = width / 2;
    int antY = height / 2;

    vector<vector<int>> board = DeadState(height, width);
    langtonAntNext(board, height, width, antX, antY, 'N');
}

void SelectRules(int rule)
{
    switch (rule)
    {
    case 1:
        StartLife(rule);
        break;
    case 2:
        StartAnt(rule);
        break;
    }
}

int main() 
{
    setupConsole();

    int rule = 0;

    cout << "Select your rule:" << endl << endl
        << "1. Conway's Game of Life" << endl
        << "2. Langton's Ant" << endl << endl;
    while (true)
    {
        cin >> rule;
        if (rule > 0 && rule < 3)
            break;
        else cout << "Not a valid rule. Select again." << endl;
    }

    SelectRules(rule);
}
