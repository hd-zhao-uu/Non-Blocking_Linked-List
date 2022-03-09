//  Compile: g++ main.cpp -lpthread -o main

#include <array>
#include <fstream>
#include <thread>
#include <vector>

#include "Lists/CoarseList.cpp"
#include "Lists/FineList.cpp"
#include "Lists/OptimisticList.cpp"
#include "Lists/NonBlockingList.cpp"
#include "Lists/Pair.h"
#include "Utilities/IO.cpp"
#include "Utilities/Testcases.cpp"
#include "Utilities/Timer.h"

template <class T>
void workerFunc(int id, T* set, std::vector<Pair>& testCase) {
    bool ret;
    for (auto it = testCase.begin(); it != testCase.end(); ++it) {
        // Do the operation from the test case
        switch (it->method) {
            case add:
                ret = set->add(it->input);
                // counter[id] ++; // only add its own counter field
                break;
            case rmv:
                ret = set->rmv(it->input);
                // counter[id] ++;
                break;
            case ctn:
                ret = set->ctn(it->input);
                // counter[id] ++;
                break;
            default:
                printf("Worker %d no matching method found. \n", id);
        }
    }

    // printf("Worker %d completed. \n", id);
}

void testList();
void _testList(std::vector<int>& threadNums,
            std::vector<Pair>& testCase,
            std::string& outpuFilePath, std::string& logPath);

int main() {
    testList();
}

void testList() {
    std::vector<int> threadNums({2, 4, 8, 16, 32, 64});
    std::vector<Pair> testCase;
    int testSize = 100000;

    std::string outputFilePath = "./Results/Results.txt";
    std::ofstream testFile(outputFilePath);

    std::string logPath = "./Results/data.txt";
    std::ofstream logFile(logPath);


    std::string bigDivider =
        "======================================================================"
        "================\n";
    std::string note;

    int i;

    note = "SET i = 10, values: [0...1023]\n";
    i = 10;
    std::cout << bigDivider;
    std::cout << note;
    strLine2File(outputFilePath, bigDivider);
    strLine2File(outputFilePath, note);
    strLine2File(logPath, bigDivider);
    strLine2File(logPath, note);
    createTestCases(0, 1023, testSize, i, testCase);
    _testList(threadNums, testCase, outputFilePath, logPath);

    note = "SET i = 50, values: [0...1023]\n";
    i = 50;
    std::cout << bigDivider;
    std::cout << note;
    strLine2File(outputFilePath, bigDivider);
    strLine2File(outputFilePath, note);
    strLine2File(logPath, bigDivider);
    strLine2File(logPath, note);
    createTestCases(0, 1023, testSize, i, testCase);
    _testList(threadNums, testCase, outputFilePath, logPath);

    note = "SET i = 90, values: [0...1023]\n";
    i = 90;
    std::cout << bigDivider;
    std::cout << note;
    strLine2File(outputFilePath, bigDivider);
    strLine2File(outputFilePath, note);
    strLine2File(logPath, bigDivider);
    strLine2File(logPath, note);
    createTestCases(0, 1023, testSize, i, testCase);
    _testList(threadNums, testCase, outputFilePath, logPath);
}

void _testList(std::vector<int>& threadNums,
            std::vector<Pair>& testCase,
            std::string& outputFilePath, std::string& logPath) {
    // input values: {0,1,...,7}
    std::string smallDivider =
        "----------------------------------------------------------------------"
        "----------------\n";
    std::string mark;

    double startTime, endTime, elapsed, opPer10Sec;
    std::vector<std::string> results;
    std::vector<std::string> logs;

    // NonBlocking List Set
    for (auto& nThread : threadNums) {
        GET_TIME(startTime);
        std::thread* workers = new std::thread[nThread];
        int* counter = new int[nThread];
        NonBlockingList* nbl = new NonBlockingList();

        for (int i = 0; i < nThread; i++) {
            workers[i] = std::thread(workerFunc<NonBlockingList>, i,
                                     std::ref(nbl), std::ref(testCase));
        }

        for (int i = 0; i < nThread; i++) {
            workers[i].join();
        }

        GET_TIME(endTime);
        elapsed = endTime - startTime;
        opPer10Sec = testCase.size() * nThread / elapsed * 10;

        char buff[100];
        snprintf(
            buff, sizeof(buff),
            "[OUTPUT] %2d threads, %24s: %12.3lf operations per 10 seconds.\n",
            nThread, "NonBlocking List Set", opPer10Sec);
        std::string result = buff;
        results.push_back(result);
        printf(
            "[OUTPUT] %2d threads, %24s: %12.3lf operations per 10 seconds.\n",
            nThread, "NonBlocking List Set", opPer10Sec);

        char logBuff[100];
        snprintf(logBuff, sizeof(logBuff), "%2d, %12.3lf\n", nThread,
                 opPer10Sec);
        std::string log = logBuff;
        logs.push_back(log);

        delete[] workers;
        delete nbl;
    }

    resultVector2File(outputFilePath, results);
    results.clear();
    std::cout << smallDivider;
    strLine2File(outputFilePath, smallDivider);

    mark = "[NonBlocking List Set]\n";
    strLine2File(logPath, mark);
    resultVector2File(logPath, logs);
    strLine2File(logPath, smallDivider);
    logs.clear();


    // Coarse-grained List Set
    for (auto& nThread : threadNums) {
        GET_TIME(startTime);
        std::thread* workers = new std::thread[nThread];
        // int* counter = new int[nThread];
        CoarseList* cl = new CoarseList();

        for (int i = 0; i < nThread; i++) {
            workers[i] = std::thread(workerFunc<CoarseList>, i, std::ref(cl),
                                     std::ref(testCase));
        }

        for (int i = 0; i < nThread; i++) {
            workers[i].join();
        }

        GET_TIME(endTime);
        elapsed = endTime - startTime;
        opPer10Sec = testCase.size() * nThread / elapsed * 10;

        char buff[100];
        snprintf(
            buff, sizeof(buff),
            "[OUTPUT] %2d threads, %24s: %12.3lf operations per 10 seconds.\n",
            nThread, "Coarse-grained List Set", opPer10Sec);
        std::string result = buff;
        results.push_back(result);

        char logBuff[100];
        snprintf(logBuff, sizeof(logBuff), "%2d, %12.3lf\n", nThread,
                 opPer10Sec);
        std::string log = logBuff;
        logs.push_back(log);

        printf(
            "[OUTPUT] %2d threads, %24s: %12.3lf operations per 10 seconds.\n",
            nThread, "Coarse-grained List Set", opPer10Sec);

        delete[] workers;
        delete cl;
    }

    resultVector2File(outputFilePath, results);
    results.clear();
    std::cout << smallDivider;
    strLine2File(outputFilePath, smallDivider);

    mark = "[Coarse-grained List Set]\n";
    strLine2File(logPath, mark);
    resultVector2File(logPath, logs);
    strLine2File(logPath, smallDivider);
    logs.clear();

    // Fine-grained List Set
    for (auto& nThread : threadNums) {
        GET_TIME(startTime);
        std::thread* workers = new std::thread[nThread];
        // int* counter = new int[nThread];
        FineList* fl = new FineList();

        for (int i = 0; i < nThread; i++) {
            workers[i] = std::thread(workerFunc<FineList>, i, std::ref(fl),
                                     std::ref(testCase));
        }

        for (int i = 0; i < nThread; i++) {
            workers[i].join();
        }

        GET_TIME(endTime);
        elapsed = endTime - startTime;
        opPer10Sec = testCase.size() * nThread / elapsed * 10;

        char buff[100];
        snprintf(
            buff, sizeof(buff),
            "[OUTPUT] %2d threads, %24s: %12.3lf operations per 10 seconds.\n",
            nThread, "Fine-grained List Set", opPer10Sec);
        std::string result = buff;
        results.push_back(result);
        printf(
            "[OUTPUT] %2d threads, %24s: %12.3lf operations per 10 seconds.\n",
            nThread, "Fine-grained List Set", opPer10Sec);

        char logBuff[100];
        snprintf(logBuff, sizeof(logBuff), "%2d, %12.3lf\n", nThread,
                 opPer10Sec);
        std::string log = logBuff;
        logs.push_back(log);

        delete[] workers;
        delete fl;
    }

    resultVector2File(outputFilePath, results);
    results.clear();
    std::cout << smallDivider;
    strLine2File(outputFilePath, smallDivider);

    mark = "[Fine-grained List Set]\n";
    strLine2File(logPath, mark);
    resultVector2File(logPath, logs);
    strLine2File(logPath, smallDivider);
    logs.clear();

    // Optimistic List Set
    for (auto& nThread : threadNums) {
        GET_TIME(startTime);
        std::thread* workers = new std::thread[nThread];
        int* counter = new int[nThread];
        OptimisticList* ol = new OptimisticList();

        for (int i = 0; i < nThread; i++) {
            workers[i] = std::thread(workerFunc<OptimisticList>, i,
                                     std::ref(ol), std::ref(testCase));
        }

        for (int i = 0; i < nThread; i++) {
            workers[i].join();
        }

        GET_TIME(endTime);
        elapsed = endTime - startTime;
        opPer10Sec = testCase.size() * nThread / elapsed * 10;

        char buff[100];
        snprintf(
            buff, sizeof(buff),
            "[OUTPUT] %2d threads, %24s: %12.3lf operations per 10 seconds.\n",
            nThread, "Optimistic List Set", opPer10Sec);
        std::string result = buff;
        results.push_back(result);
        printf(
            "[OUTPUT] %2d threads, %24s: %12.3lf operations per 10 seconds.\n",
            nThread, "Optimistic List Set", opPer10Sec);

        char logBuff[100];
        snprintf(logBuff, sizeof(logBuff), "%2d, %12.3lf\n", nThread,
                 opPer10Sec);
        std::string log = logBuff;
        logs.push_back(log);

        delete[] workers;
        delete ol;
    }

    resultVector2File(outputFilePath, results);
    results.clear();
    std::cout << smallDivider;
    strLine2File(outputFilePath, smallDivider);

    mark = "[Optimistic List Set]\n";
    strLine2File(logPath, mark);
    resultVector2File(logPath, logs);
    strLine2File(logPath, smallDivider);
    logs.clear();
}