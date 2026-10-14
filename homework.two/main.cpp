#include <iostream>
#include <limits>
#include <new>
#include <stdexcept>

namespace novikov
{
    size_t safe_add(size_t a, size_t b)
    {
        if (a > std::numeric_limits<size_t>::max() - b)
        {
            throw std::overflow_error("Overflow in addition");
        }
        return a + b;
    }

    size_t safe_multiply(size_t a, size_t b)
    {
        if (a != 0 && b > std::numeric_limits<size_t>::max() / a)
        {
            throw std::overflow_error("Overflow in multiplication");
        }
        return a * b;
    }

    void delete_matrix(int** matrix, size_t rows)
    {
        if (matrix != nullptr)
        {
            for (size_t i = 0; i < rows; ++i)
            {
                delete[] matrix[i];
            }
            delete[] matrix;
        }
    }

    int** allocate_matrix(size_t rows, size_t cols)
    {
        safe_multiply(rows, sizeof(int*));
        safe_multiply(cols, sizeof(int));
        int** matrix = new int*[rows]();
        try
        {
            for (size_t i = 0; i < rows; ++i)
            {
                matrix[i] = new int[cols];
            }
        }
        catch (...)
        {
            delete_matrix(matrix, rows);
            throw;
        }
        return matrix;
    }

    int** allocate_result_matrix(size_t n1, size_t m1, size_t n2, size_t m2)
    {
        size_t n = std::max(n1, n2);
        safe_multiply(n, sizeof(int*));
        int** result = new int*[n]();
        try
        {
            for (size_t i = 0; i < n; ++i)
            {
                size_t length = 0;
                if (i < n1)
                {
                    length = safe_add(length, m1);
                }
                if (i < n2)
                {
                    length = safe_add(length, m2);
                }
                safe_multiply(length, sizeof(int));
                result[i] = new int[length];
            }
        }
        catch (...)
        {
            delete_matrix(result, n);
            throw;
        }
        return result;
    }

    int** concat_rows(const int* const* lhs, size_t n1, size_t m1, const int* const* rhs, size_t n2, size_t m2)
    {
        int** result = allocate_result_matrix(n1, m1, n2, m2);
        for (size_t i = 0; i < n1; ++i)
        {
            for (size_t j = 0; j < m1; ++j)
            {
                result[i][j] = lhs[i][j];
            }
        }
        for (size_t i = 0; i < n2; ++i)
        {
            size_t shift = 0;
            if (i < n1)
            {
                shift = m1;
            }
            for (size_t j = 0; j < m2; ++j)
            {
                result[i][shift + j] = rhs[i][j];
            }
        }
        return result;
    }

    void print_result(const int* const* matrix, size_t n1, size_t m1, size_t n2, size_t m2)
    {
        size_t n = std::max(n1, n2);
        for (size_t i = 0; i < n; ++i)
        {
            size_t length = 0;
            if (i < n1)
            {
                length += m1;
            }
            if (i < n2)
            {
                length += m2;
            }
            for (size_t j = 0; j < length; ++j)
            {
                std::cout << matrix[i][j] << ' ';
            }
            std::cout << '\n';
        }
    }
}

int main()
{
    constexpr int success_code = 0;
    constexpr int input_error_code = 1;
    constexpr int memory_error_code = 2;
    constexpr int unknown_error_code = 3;
    int code = success_code;
    size_t n1 = 0, n2 = 0;
    size_t m1 = 0, m2 = 0;
    int** lhs = nullptr;
    int** rhs = nullptr;
    int** result = nullptr;
    try
    {
        std::cout << "Enter the number of rows and columns for the first matrix: ";
        if (!(std::cin >> n1 >> m1) || n1 == 0 ||  m1 == 0)
        {
            throw std::invalid_argument("Invalid input");
        }
        std::cout << "Enter the number of rows and columns for the second matrix: ";
        if (!(std::cin >> n2 >> m2) || n2 == 0 || m2 == 0)
        {
            throw std::invalid_argument("Invalid input");
        }
        lhs = novikov::allocate_matrix(n1, m1);
        rhs = novikov::allocate_matrix(n2, m2);
        std::cout << "Enter the elements of the first matrix: ";
        for (size_t i = 0; i < n1; ++i)
        {
            for (size_t j = 0; j < m1; ++j)
            {
                if (!(std::cin >> lhs[i][j]))
                {
                    throw std::invalid_argument("Invalid input");
                }
            }
        }
        std::cout << "Enter the elements of the second matrix: ";
        for (size_t i = 0; i < n2; ++i)
        {
            for (size_t j = 0; j < m2; ++j)
            {
                if (!(std::cin >> rhs[i][j]))
                {
                    throw std::invalid_argument("Invalid input");
                }
            }
        }
        result = novikov::concat_rows(lhs, n1, m1, rhs, n2, m2);
        std::cout << "==Result Matrix==\n";
        novikov::print_result(result, n1, m1, n2, m2);
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        code = input_error_code;
    }
    catch (const std::bad_alloc& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        code = memory_error_code;
    }
    catch (const std::overflow_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        code = memory_error_code;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        code = unknown_error_code;
    }
    novikov::delete_matrix(lhs, n1);
    novikov::delete_matrix(rhs, n2);
    novikov::delete_matrix(result, std::max(n1, n2));
    return code;
}
