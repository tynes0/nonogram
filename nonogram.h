#pragma once

#include "rand.h"
#include <conio.h>

#define charize(x) #@x


// recomended max table size is 9
#define TABLE_SIZE 9

#define INT_TABLE 1
#define CHAR_TABLE 0

#define TABLE_TYPE CHAR_TABLE

struct nonogram_table
{
#if TABLE_TYPE == INT_TABLE
	int table[TABLE_SIZE][TABLE_SIZE];
#elif TABLE_TYPE == CHAR_TABLE 
	char table[TABLE_SIZE][TABLE_SIZE];
#endif // TABLE_TYPE == INT_TABLE
	int vertical_values[TABLE_SIZE];
	int horizontal_values[TABLE_SIZE];
};

class nonogram
{
public:
	void init()
	{
		for (uint32_t i = 0; i < TABLE_SIZE; ++i)
		{
			tbl.vertical_values[i] = rval();
			tbl.horizontal_values[i] = 0;
			for (uint32_t j = 0; j < TABLE_SIZE; ++j)
			{
#if TABLE_TYPE == INT_TABLE
				tbl.table[i][j] = 0;
#elif TABLE_TYPE == CHAR_TABLE 
				tbl.table[i][j] = ' ';
#endif // TABLE_TYPE == INT_TABLE
			}
		}
		{
			int temp[TABLE_SIZE];
			memcpy(temp, tbl.vertical_values, sizeof(tbl.vertical_values));
			for (uint32_t i = 0; i < TABLE_SIZE; ++i)
			{
				for (uint32_t j = 0; j < TABLE_SIZE; ++j)
				{
					if (temp[j] > 0) tbl.horizontal_values[i]++;
				}
				decrease_1_from_each_idx(temp);
			}
		}
		int tvv[TABLE_SIZE];
		int thv[TABLE_SIZE];
		memcpy(tvv, tbl.vertical_values, sizeof(tbl.vertical_values));
		memcpy(thv, tbl.horizontal_values, sizeof(tbl.horizontal_values));


		for (uint32_t i = 0; i < TABLE_SIZE; ++i)
		{
			for (uint32_t j = 0; j < TABLE_SIZE; ++j)
			{
				if (tvv[j] > 0 && thv[i] > 0)
				{
					tvv[j]--;
					thv[i]--;
#if TABLE_TYPE == INT_TABLE
					tbl.table[i][j] = 1;
#elif TABLE_TYPE == CHAR_TABLE 
					tbl.table[i][j] = 'x';
#endif // TABLE_TYPE == INT_TABLE
				}
			}
		}
		mix();
	}

	void run()
	{
		play_again:
		init();
#if TABLE_TYPE == INT_TABLE
		int player_table[TABLE_SIZE][TABLE_SIZE];
		for (uint32_t i = 0; i < TABLE_SIZE; ++i)
		{
			for (uint32_t j = 0; j < TABLE_SIZE; ++j)
			{
				player_table[i][j] = 0;
			}
		}
#elif TABLE_TYPE == CHAR_TABLE
		char player_table[TABLE_SIZE][TABLE_SIZE];
		for (uint32_t i = 0; i < TABLE_SIZE; ++i)
		{
			for (uint32_t j = 0; j < TABLE_SIZE; ++j)
			{
				player_table[i][j] = ' ';
			}
		}
#endif
		update_table(player_table, 1 , 1);
		int i = 0, j = 0;
		while (char ch = get_input())
		{
			switch (toupper(ch))
			{
			case charize(W):
				if (i > 0) i--;
				else i = TABLE_SIZE - 1;
				break;
			case charize(A):
				if (j > 0) j--;
				else j = TABLE_SIZE - 1;
				break;
			case charize(S):
				if (i < TABLE_SIZE - 1) i++;
				else i = 0;
				break;
			case charize(D):
				if (j < TABLE_SIZE - 1) j++;
				else j = 0;
				break;
			case 32:
#if TABLE_TYPE == INT_TABLE
				if (player_table[i][j] == 1) player_table[i][j] = 0;
				else player_table[i][j] = 1;
#elif TABLE_TYPE == CHAR_TABLE 
				if (player_table[i][j] == 'x') player_table[i][j] = ' ';
				else player_table[i][j] = 'x';
#endif // TABLE_TYPE == INT_TABLE

				break;
			case 13:
				if (check(player_table))
				{
					std::cout << "\nYOU WON!";
				}
				else
				{
					std::cout << "\nYOU LOST!";
				}
				std::cout << "\np - play again\n\Esc - exit";
				while (char decision = get_input())
				{
					if (toupper(decision) == 'P')
					{
						goto play_again;
					}
				}
				return;
			default:
				break;
			}
			update_table(player_table, i + 1, j + 1);
		}
	}

	void print_result()
	{
		for (uint32_t i = 0; i < TABLE_SIZE; ++i)
		{
			for (uint32_t j = 0; j < TABLE_SIZE; ++j)
			{
				std::cout << tbl.table[i][j] << ' ';
			}
			std::cout << '\n';
		}
	}

private:
#if TABLE_TYPE == INT_TABLE
	bool check(int player_input[][TABLE_SIZE])
#elif TABLE_TYPE == CHAR_TABLE
	bool check(char player_input[][TABLE_SIZE])
#endif // 
	{
		for (uint32_t i = 0; i < TABLE_SIZE; ++i)
		{
			for (uint32_t j = 0; j < TABLE_SIZE; ++j)
			{
				if (player_input[i][j] != tbl.table[i][j]) return false;
			}
		}
		return true;
	}

	int rval()
	{
		return static_cast<int>((wrand(time(NULL)) % TABLE_SIZE) + 1);
	}

	void decrease_1_from_each_idx(int* data)
	{
		for (uint32_t i = 0; i < TABLE_SIZE; ++i)
		{
			if (data[i] > 0)
			{
				data[i]--;
			}
		}
	}

#if TABLE_TYPE == INT_TABLE
	void update_table(int player_table[][TABLE_SIZE], int line, int column)
#elif TABLE_TYPE == CHAR_TABLE
	void update_table(char player_table[][TABLE_SIZE], int line, int column)
#endif // TABLE_TYPE == INT_TABLE
	{
		system("cls");
		std::cout << "    ";
		for (uint32_t i = 0; i < TABLE_SIZE; ++i)
		{
			std::cout << tbl.vertical_values[i] << "   ";
		}
		std::cout << "\n ";
		for (uint32_t i = 0; i < TABLE_SIZE; ++i)
		{
			std::cout << "   -";
		}
		std::cout << "\n";
		for (uint32_t i = 0; i < TABLE_SIZE; ++i)
		{
			std::cout << tbl.horizontal_values[i] << "|  ";
			for (uint32_t j = 0; j < TABLE_SIZE; ++j)
			{
				std::cout << player_table[i][j] << "   ";
			}
			std::cout << "\n\n";
		}
		std::cout << "\n";
		std::cout << "\tCONTROLS\nMove up: w\nMove down: s\nMove left: a\nMove right: d\nMark - remark: space\nSubmit: Enter\n\n";
		std::cout << line << ". line " << column << ". column";
	}

	char get_input()
	{
		char in;
		in = _getch();

		if (in == char(27))
		{
			return static_cast<char>(0);
		}

		return in;
	}

	void mix()
	{
		auto swap = [](int& left, int& right)
		{
			int temp = left;
			left = right;
			right = temp;
		};
#if TABLE_TYPE == CHAR_TABLE
		auto swapci = [](char& left, char& right)
		{
			char temp = left;
#elif TABLE_TYPE == INT_TABLE
		auto swapci = [](int& left, int& right)
		{
			int temp = left;
#endif
			left = right;
			right = temp;
		};
		for (uint32_t i = 0; i < TABLE_SIZE * 2; ++i)
		{
			int rv1 = rval() - 1;
			int rv2 = rval() - 1;
			swap(tbl.horizontal_values[rv1], tbl.horizontal_values[rv2]);
			for (uint32_t i = 0; i < TABLE_SIZE; ++i)
			{
				swapci(tbl.table[rv1][i], tbl.table[rv2][i]);
			}
		}
	}

private:
	nonogram_table tbl;
};
