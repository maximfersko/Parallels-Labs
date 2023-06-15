#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <sstream>
#include <cmath>

std::vector<int> PrimeFactors(int number) {
    std::vector<int> factors;
    for (int i = 2; i <= sqrt(number); ++i) {
        while (number % i == 0) {
            factors.push_back(i);
            number /= i;
        }
    }
    if (number > 1) {
        factors.push_back(number);
    }
    return factors;
}

int main() {
    std::string inputFilePath = "input.txt";
    std::ifstream inputFile(inputFilePath);
    if (!inputFile) {
        std::cout << "Error opening file: " << inputFilePath << std::endl;
        return 1;
    }

    std::mutex outputMutex;

    std::thread outputThread([&]() {
        int fileCounter = 1;
        while (true) {
            std::string outputFilePath;
            {
                std::ostringstream oss;
                oss << "out" << std::hex << std::this_thread::get_id() << "_" << fileCounter << ".txt";
                outputFilePath = oss.str();
            }

            std::ofstream outputFile(outputFilePath);
            if (!outputFile) {
                std::lock_guard<std::mutex> lock(outputMutex);
                std::cout << "Error creating file: " << outputFilePath << std::endl;
                return;
            }

            std::string line;
            {
                std::lock_guard<std::mutex> lock(outputMutex);
                if (!std::getline(inputFile, line)) {
                    return;
                }
            }

            int number = std::stoi(line);
            std::vector<int> factors = PrimeFactors(number);

            {
                std::lock_guard<std::mutex> lock(outputMutex);
                std::cout << "Number: " << number << std::endl;
                std::cout << "Prime factors: ";
                for (int factor : factors) {
                    std::cout << factor << " ";
                }
                std::cout << std::endl;
            }

            for (int factor : factors) {
                outputFile << factor << " ";
            }
            outputFile.close();

            fileCounter++;
        }
    });

    std::thread computationThread([&]() {
        while (true) {
            std::string line;
            {
                std::lock_guard<std::mutex> lock(outputMutex);
                if (!std::getline(inputFile, line)) {
                    return;
                }
            }

            int number = std::stoi(line);
            std::vector<int> factors = PrimeFactors(number);

            {
                std::lock_guard<std::mutex> lock(outputMutex);
                std::cout << "Number: " << number << std::endl;
                std::cout << "Prime factors: ";
                for (int factor : factors) {
                    std::cout << factor << " ";
                }
                std::cout << std::endl;
            }

            std::string outputFilePath;
            {
                std::ostringstream oss;
                oss << "out" << std::hex << std::this_thread::get_id() << ".txt";
                outputFilePath = oss.str();
            }

            std::ofstream outputFile(outputFilePath);
            if (!outputFile) {
                std::lock_guard<std::mutex> lock(outputMutex);
                std::cout << "Error creating file: " << outputFilePath << std::endl;
                return;
            }

            for (int factor : factors) {
                outputFile << factor << " ";
            }
            outputFile.close();
        }
    });

    computationThread.join();
    outputThread.join();

    return 0;
}
