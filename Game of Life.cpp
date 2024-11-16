#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> DeadState(int height, int width)
{
	vector<vector<int>> deadBoard;

	for (int i = 0; i < height; i++)
	{
		deadBoard.push_back({});

		for (int j = 0; j < width; j++)
		{
			deadBoard[i].push_back(0);
		}
	}

	return deadBoard;
}

int main()
{
	int width = 5;
	int height = 5;


	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			cout << DeadState(height, width)[i][j] << " ";
		}
		cout << endl;
	}
}

