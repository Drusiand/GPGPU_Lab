#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <cassert>

#define COL_DELIMITER ' '
#define ROW_DELIMITER std::endl

using std::vector;
using std::ifstream;
using std::ofstream;

class Matrix
{
public:
	Matrix();
	~Matrix();

	void ReadFromFile(const char* src);
	void CreateFromDims(const int rows, const int cols);

	void Print();
	void WriteToFile(const char* dst);

	int Cols();
	int Rows();
	int Count();
	double* Data();

	double Get(const int row, const int col);
	void Set(const int row, const int col, const double value);

private:
	int cols;
	int rows;
	int count;
	double* data;
};

