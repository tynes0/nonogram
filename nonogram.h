#pragma once

#include "rand.h"
#include <conio.h>

#define charize(x) #@x

// recomended max table size is 9
#define TABLE_SIZE 9

#define INT_TABLE 1
#define CHAR_TABLE 0

#define TABLE_TYPE INT_TABLE

#if (TABLE_TYPE == CHAR_TABLE)
typedef char nonogram_var_t;
static constexpr char MARKED_VAR = 'x';
static constexpr char UNMARKED_VAR = ' ';
#elif (TABLE_TYPE == INT_TABLE)
typedef int nonogram_var_t;
static constexpr int MARKED_VAR = 1;
static constexpr int UNMARKED_VAR = 0;
#else
#error this game only support CHAR_TABLE and INT_TABLEs
#endif // TABLE_TYPE == CHAR_TABLE


struct nonogram_table
{
	nonogram_var_t table[TABLE_SIZE][TABLE_SIZE];
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
				tbl.table[i][j] = UNMARKED_VAR;
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
					tbl.table[i][j] = MARKED_VAR;
				}
			}
		}
		mix();
	}

	void run()
	{
		play_again:
		init();
		nonogram_var_t player_table[TABLE_SIZE][TABLE_SIZE];
		for (uint32_t i = 0; i < TABLE_SIZE; ++i)
		{
			for (uint32_t j = 0; j < TABLE_SIZE; ++j)
			{
				player_table[i][j] = UNMARKED_VAR;
			}
		}
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
				if (player_table[i][j] == MARKED_VAR) player_table[i][j] = UNMARKED_VAR;
				else player_table[i][j] = MARKED_VAR;
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
	bool check(nonogram_var_t player_input[][TABLE_SIZE])
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

	void update_table(nonogram_var_t player_table[][TABLE_SIZE], int line, int column)
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

		for (uint32_t i = 0; i < TABLE_SIZE * 2; ++i)
		{
			int rv1 = rval() - 1;
			int rv2 = rval() - 1;
			std::swap(tbl.horizontal_values[rv1], tbl.horizontal_values[rv2]);
			for (uint32_t i = 0; i < TABLE_SIZE; ++i)
			{
				std::swap(tbl.table[rv1][i], tbl.table[rv2][i]);
			}
		}
	}

private:
	nonogram_table tbl;
};
