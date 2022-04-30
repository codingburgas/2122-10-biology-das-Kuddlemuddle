#include <misc.h>

std::string toLowerCase(std::string input)
{
	std::transform(input.begin(), input.end(), input.begin(),
		[](unsigned char c) { return std::tolower(c); });

	return input;
}

std::vector<std::string> convertP1andP2toAnswers(std::string P1, std::string P2)
{
	std::vector<std::string> returnVal;
	std::vector<std::string> rows(4);
	std::vector<std::string> cols(4);

	int pos[4][2] = {
		{0, 2},
		{0, 3},
		{1, 2},
		{1, 3}
	};

	for (int i = 0; i < 4; i++)
	{
		rows[i] = std::string(1, P1[pos[i][0]]) + std::string(1, P1[pos[i][1]]);
	}

	for (int i = 0; i < 4; i++)
	{
		cols[i] = std::string(1, P2[pos[i][0]]) + std::string(1, P2[pos[i][1]]);
	}

	for (auto& col : cols)
	{
		for (auto& row : rows)
		{
			std::string value = "";

			// Starts with A
			if (row[0] < col[0])
			{
				value += row.substr(0, 1) + col.substr(0, 1);
			}
			else
			{
				value += col.substr(0, 1) + row.substr(0, 1);
			}
			
			// Starts with B	
			if (row[1] < col[1])
			{
				value += row.substr(1, 1) + col.substr(1, 1);
			}
			else
			{
				value += col.substr(1, 1) + row.substr(1, 1);
			}

			returnVal.push_back(value);
		}
	}

	return returnVal;
}
