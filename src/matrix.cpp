#include "matrix.h"
matrix::~matrix()
{
	for (size_t i = 0; i < rowsCnt; i++)
	{
		delete[] data[i];
	}
	delete[] data;
}

matrix::matrix(unsigned int rows, unsigned int cols) :rowsCnt(rows), colsCnt(cols)
{
	data = new double* [rowsCnt];
	for (size_t i = 0; i < rowsCnt; i++)
	{
		data[i] = new double[colsCnt];
	}
}
matrix::matrix(const std::initializer_list<std::initializer_list<double>>& matrixInit)
{
	int i = 0;
	rowsCnt=matrixInit.size();
	data = new double* [rowsCnt];
	for (auto& row : matrixInit)
	{
		colsCnt=row.size();
		data[i] = new double[colsCnt];
		int j = 0;
		for (auto& element:row)
		{
			data[i][j]=element;
			++j;
		}
		++i;
	}
}

matrix::matrix(const matrix& orig) :colsCnt(orig.colsCnt), rowsCnt(orig.rowsCnt)
{
	data = new double* [rowsCnt];
	for (size_t i = 0; i < rowsCnt; i++)
	{
		data[i] = new double[colsCnt];
		for (size_t j = 0; j < colsCnt; j++)
		{
			data[i][j] = orig.data[i][j];
		}
	}
}

void matrix::fill(double value)
{
	for (size_t i = 0; i < rowsCnt; i++)
	{
		for (size_t j = 0; j < colsCnt; j++)
		{
			data[i][j] = value;
		}
	}
}

matrixRow matrix::operator[](unsigned int index)
{
	matrixRow result(data[index],colsCnt);
	return result;
}

matrix matrix::operator*(matrix& rightValue)
{
	matrix result(rowsCnt, rightValue.colsCnt);
	result.fill(0);
	for (size_t i = 0; i < rowsCnt; i++)// óìíîæåíèå ìàòðèöû ñ êîîðäèíàòàìè íà ìàòðèöó ïðåîáðàçîâàíèÿ
	{
		for (size_t j = 0; j < rightValue.colsCnt; j++)
		{
			for (size_t k = 0; k < colsCnt; k++)
			{
				result[i][j] += data[i][k] * rightValue[k][j];
			}

		}
	}
	return result;
}

matrix& matrix::operator=(const matrix& rightValue)
{
	if (this == &rightValue)
		return *this;

	if (colsCnt == rightValue.colsCnt && rowsCnt == rightValue.rowsCnt)
	{
		for (size_t i = 0; i < rowsCnt; i++)
		{
			for (size_t j = 0; j < colsCnt; j++)
			{
				data[i][j] = rightValue.data[i][j];
			}
		}
		return *this;
	}

	for (size_t i = 0; i < rowsCnt; i++)
	{
		delete[] data[i];
	}
	delete[] data;
	rowsCnt = rightValue.rowsCnt;
	colsCnt = rightValue.colsCnt;
	data = new double* [rowsCnt];
	for (size_t i = 0; i < rowsCnt; i++)
	{
		data[i] = new double[colsCnt];
		for (size_t j = 0; j < colsCnt; j++)
		{
			data[i][j] = rightValue.data[i][j];
		}
	}
	return *this;
}
matrix& matrix::operator=(const std::initializer_list<std::initializer_list<double>>& matrixInit)
{
	for (size_t i = 0; i < rowsCnt; i++)
	{
		delete[] data[i];
	}
	delete[] data;
	int i = 0;
	rowsCnt = matrixInit.size();
	data = new double* [rowsCnt];
	for (auto& row : matrixInit)
	{
		colsCnt = row.size();
		data[i] = new double[colsCnt];
		int j = 0;
		for (auto& element : row)
		{
			data[i][j] = element;
			++j;
		}
		++i;
	}
	return *this;
}

void matrixRow::operator=(std::initializer_list<double>& newRow)
{
	int count = 0;
	for (auto& element : newRow)
	{
		row[count] = element;
		++count;
	}
}