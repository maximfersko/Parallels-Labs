#include <iostream>
#include <cmath>
#include <pthread.h>
#include <chrono>

struct ThreadData {
    double a; 
    double b; 
    int n; 
    double* result;
    pthread_mutex_t* mutex; 
};


double f(double x) {
    return x * x;
}

void* MidPointRectangleMethod(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    double a = data->a;
    double b = data->b;
    int n = data->n;
    double* result = data->result;
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        double x = a + (i + 0.5) * h;
        sum += f(x);
    }
    
    pthread_mutex_lock(data->mutex);
    *result += h * sum;
    pthread_mutex_unlock(data->mutex);
    
    return nullptr;
}

int main() {
    const double a = 0.0; // Нижний предел интегрирования
    const double b = 1.0; // Верхний предел интегрирования
    int n = 1000000; // Количество шагов
    int numThreads = 0; // Количество потоков
    
    double result = 0.0;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);
    
    std::cout << "Enter number threads:" << std::endl;
    std::cin >> numThreads;

    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];

    auto start = std::chrono::steady_clock::now(); 

    // Распределение работы между потоками
    for (int i = 0; i < numThreads; i++) {
        threadData[i].a = a + i * (b - a) / numThreads; // вычисление нижнинего предела интегрирования 
        threadData[i].b = a + (i + 1) * (b - a) / numThreads; // вычисление верхнего предела интегрирования 
        threadData[i].n = n / numThreads; // уставление шагов интегрирования для каждого потокв 
        threadData[i].result = &result;
        threadData[i].mutex = &mutex;
        pthread_create(&threads[i], nullptr, MidPointRectangleMethod, (void*)&threadData[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], nullptr);
    }

    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> duration = end - start;

    std::cout << "Result: " << result << std::endl;
    std::cout << "Time: " << duration.count() << " s" << std::endl;

    pthread_mutex_destroy(&mutex);

    return 0;
}
