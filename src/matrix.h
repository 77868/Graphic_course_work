#pragma once
#include <iostream>
class matrixRow
{
private:
	double* row;
	unsigned int colsCnt;
public:
	matrixRow(double* row,unsigned int colsCnt) :row(row),colsCnt(colsCnt) {};
	void operator=(std::initializer_list<double>& newRow);
	double& operator[](unsigned int index)
	{
		return row[index];
	}
};


class matrix
{
private:
	double** data;	
	unsigned int rowsCnt, colsCnt;
public:
	~matrix();
	matrix(unsigned int rows, unsigned int cols);
	matrix(const matrix& orig);
	matrix(const std::initializer_list<std::initializer_list<double>>& matrixInit);
	void fill(double value);
	matrix operator*(matrix& rightValue);
	matrix& operator=(const matrix& rightValue);
	matrix& operator=(const std::initializer_list<std::initializer_list<double>>& matrixInit);
	matrixRow operator[](unsigned int index);
};