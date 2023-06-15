#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <filesystem>

int CalculateGCD(int a, int b) {
    if (b == 0)
        return a;
    else
        return CalculateGCD(b, a % b);
}

void TaskDelegationIndividualFilesWorker(const std::string& inputLine, const std::string& outputFileName) {
    std::istringstream iss(inputLine);
    int num1, num2;
    if (iss >> num1 >> num2) {
        int gcd = CalculateGCD(num1, num2);

        std::ofstream outputFile(outputFileName);
        if (outputFile) {
            outputFile << "GCD of numbers " << num1 << " and " << num2 << " is " << gcd << std::endl;
        } else {
            std::cout << "Failed to open output file: " << outputFileName << std::endl;
        }
    } else {
        std::cout << "Invalid input: " << inputLine << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <input_file_path> <output_directory_path>" << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::string outputDirectory = argv[2];

    if (!std::filesystem::exists(outputDirectory)) {
        if (!std::filesystem::create_directory(outputDirectory)) {
            std::cout << "Failed to create output directory: " << outputDirectory << std::endl;
            return 1;
        }
    }

    std::ifstream inputFile(inputFilePath);
    if (!inputFile) {
        std::cout << "Failed to open input file: " << inputFilePath << std::endl;
        return 1;
    }

    std::string line;

    unsigned int maxThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    unsigned int threadCount = 0;

    while (std::getline(inputFile, line)) {
        std::ostringstream oss;
        oss << outputDirectory << "/out_" << threadCount << ".txt";
        std::string outputFileName = oss.str();

        threads.emplace_back(TaskDelegationIndividualFilesWorker, line, outputFileName);
        threadCount++;

        if (threadCount == maxThreads) {
            for (auto& thread : threads) {
                thread.join();
            }
            threads.clear();
            threadCount = 0;
        }
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
