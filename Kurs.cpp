#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

int** create_two_dim_array (int rows, int cols);
void print_two_dim_array (int** arrayAddres, int rows, int cols);
int count_live_cells (int** arrayAddres, int rows, int cols);
int count_live_neigbours (int** arrayAddres, int rows, int cols, int currentRow, int currentCol);
void create_next_generation (int** currentArray, int** nextArray, int rows, int cols);
bool compare_two_dim_array (int** firstArray, int** secondArray, int rows, int cols);
void copy_two_dim_array (int** sourceArray, int** destinationArray, int rows, int cols);
void delete_two_dim_array (int** arrayAddres, int rows);

int main()
{
    std::ifstream ifile("in.txt");
    if (!ifile)
    {
        std::cout << "File error!!!" << std::endl;
        return 1;
    }

    int rows{}, cols{};
    ifile >> rows >> cols;
    if (rows <= 0 || cols <= 0)
    {
        std::cout << " Invalid field size!" << std::endl;
        return 1;
    }

    int** currentField = create_two_dim_array(rows, cols);
    int** nextField = create_two_dim_array(rows, cols);

    int row{},col{};
    while (ifile >> row >> col)
    {
        if (row >=0 && row < rows && col >= 0 && col < cols)
        {
            currentField[row][col] = 1;
        }
    }
    ifile.close();

    int generation{1};

    while (true)
    {
        int liveCells = count_live_cells(currentField, rows, cols);
        std::cout << "Generation: " << generation << std::endl;
        std::cout << "Alive cells: " << liveCells << std::endl;
        std::cout << std::endl;
        print_two_dim_array(currentField,rows,cols);

        if (liveCells == 0)
        {
            std::cout << "Game over, All cells dead" << std::endl;
            break;
        }
        
        create_next_generation(currentField, nextField, rows, cols);

        if (compare_two_dim_array(currentField, nextField, rows, cols))
        {
            std::cout << "Game over, Stable configuration!" << std::endl;
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));

        copy_two_dim_array(nextField, currentField, rows, cols);
        ++ generation;
        std::cout << std::endl;
    }
    
    delete_two_dim_array(currentField, rows);
    delete_two_dim_array(nextField, rows);

    return EXIT_SUCCESS;
}

int** create_two_dim_array(int rows,int cols)
{
    int** array{new int* [rows]};
    for (int row{}; row < rows; ++row)
    {
        array[row] = new int[cols]{};
    }
    return array;
}

void print_two_dim_array (int** arrayAddres, int rows, int cols)
{
    for (int row{}; row < rows; ++row)
    {
        for (int col{}; col < cols; ++col)
        {
            if (arrayAddres[row][col] == 1)
            {
                std::cout << "* ";
            }
            else
            {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
}

int count_live_cells (int** arrayAddres, int rows, int cols)
{
    int liveCells{};
    for (int row{}; row < rows; ++row)
    {
        for(int col{}; col < cols; ++col)
        {
            if (arrayAddres[row][col] == 1)
            {
                ++liveCells;
            }
        }
    }
    return liveCells;
}

int count_live_neigbours (int** arrayAddres, int rows, int cols, int currentRow, int currentCol)
{
    int liveNeigbours{};
    for (int row = currentRow - 1; row <= currentRow +1; ++row)
    {
        for (int col = currentCol - 1; col <= currentCol +1; ++col)
        {
            if (row == currentRow && col == currentCol)
            {
                continue;
            }

            if (row >= 0 && row < rows && col >= 0 && col < cols)
            {
                if (arrayAddres[row][col] == 1)
                {
                    ++liveNeigbours;
                }
            }
        }
    }
    return liveNeigbours;
}

void create_next_generation (int** currentArray, int** nextArray, int rows, int cols)
{
    for(int row{}; row < rows; ++row)
    {
        for(int col{}; col < cols; ++col)
        {
            int liveNeigbours = count_live_neigbours(currentArray, rows, cols, row, col);
            if (currentArray[row][col] == 1)
            {
                if (liveNeigbours == 2 || liveNeigbours == 3)
                {
                    nextArray[row][col] = 1;
                }
                else
                {
                    nextArray[row][col] = 0;
                }
            }
            else
            {
                if (liveNeigbours == 3)
                {
                    nextArray[row][col] = 1;
                }
                else
                {
                    nextArray[row][col] = 0;
                }
            }
        }
    }
}

bool compare_two_dim_array (int** firstArray, int** secondArray, int rows, int cols)
{
    for(int row{}; row < rows; ++row)
    {
        for(int col{}; col < cols; ++col)
        {
            if(firstArray[row][col] != secondArray[row][col])
            {
                return false;
            }
        }
    }
    return true;
}

void copy_two_dim_array (int** sourceArray, int** destinationArray, int rows, int cols)
{
    for(int row{}; row < rows; ++row)
    {
        for (int col{}; col < cols; ++col)
        {
            destinationArray[row][col] = sourceArray[row][col];
        }
    }
}

void delete_two_dim_array (int** arrayAddres, int rows)
{
    for(int row{}; row < rows; ++row)
    {
        delete[] arrayAddres[row];
    }

    delete[] arrayAddres;
}