#include <iostream>
#include <cmath>
#include <chrono>
#include <omp.h>


double f(double x) {
    return x * x;
}

int main() {
    double a = 0.0; 
    double b = 1.0; 
    int n = 1000000;
    int numThreads = 0; 

    double result = 0.0;

    std::cout << "Enter number threads:" << std::endl;
    std::cin >> numThreads;

    auto start = std::chrono::steady_clock::now();

    #pragma omp parallel for num_threads(numThreads) reduction(+:result)
    for (int i = 0; i < n; i++) {
        double local_result = 0.0;

        // Вычисление локальных границ интегрирования и количества шагов для каждого потока
        double local_a = a + i * (b - a) / n;
        double local_b = a + (i + 1) * (b - a) / n;
        int local_n = n / numThreads;

        // Вычисление ширины подинтервала
        double h = (local_b - local_a) / local_n;

        // Цикл вычисления интеграла методом прямоугольников в каждом потоке
        for (int j = 0; j < local_n; j++) {
            // Вычисление координаты x в середине подинтервала
            double x = local_a + (j + 0.5) * h;
            // Вычисление значения функции в точке x и суммирование результатов
            local_result += f(x);
        }

        // Умножение локального результата на ширину подинтервала
        local_result *= h;

        // Суммирование локального результата с глобальным результатом с помощью операции редукции
        result += local_result;
    }


    auto end = std::chrono::steady_clock::now(); 

    std::chrono::duration<double> duration = end - start;

    std::cout << "Result: " << result << std::endl;
    std::cout << "Time: " << duration.count() << " seconds" << std::endl;

    return 0;
}
