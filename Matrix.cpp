#include "Matrix.h"

Matrix::Matrix()
{
    cols = 0;
    rows = 0;
    count = 0;
    data = nullptr;
}

Matrix::~Matrix()
{
    delete data;
    data = nullptr;
}

void Matrix::ReadFromFile(const char* src)
{
    ifstream f(src);
    assert(f.fail() != true);

    f >> rows;
    f >> cols;

    count = rows * cols;

    data = new double[count];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (f.eof()) {
                delete data;
                data = nullptr;
                f.close();
                return;
            }
            f >> data[i * cols + j];
        }
    }
    f.close();
}

void Matrix::CreateFromDims(const int rows, const int cols)
{
    this->rows = rows;
    this->cols = cols;

    count = rows * cols;

    data = new double[count];

    for (int i = 0; i < count; i++) {
            data[i] = 0;
    }
}

void Matrix::Print()
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << data[i * rows + j] << COL_DELIMITER;
            auto a = data[i * cols + j];
        }
        std::cout << ROW_DELIMITER;
    }
}

void Matrix::WriteToFile(const char* dst)
{
    ofstream f(dst);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            f << data[i * rows + j] << COL_DELIMITER;
            auto a = data[i * cols + j];
        }
        f << ROW_DELIMITER;
    }
    f.close();
}

int Matrix::Cols()
{
    return cols;
}

int Matrix::Rows()
{
    return rows;
}

int Matrix::Count()
{
    return count;
}

double* Matrix::Data()
{
    return data;
}

double Matrix::Get(const int row, const int col)
{
    return data[row * rows + col];
}

void Matrix::Set(const int row, const int col, const double value)
{
    data[row * rows + col] = value;
}
