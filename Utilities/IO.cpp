#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <vector>


void resultVector2File(std::string& filePath, std::vector<std::string>& results){
    std::ofstream outFile;
    outFile.open(filePath, std::ofstream::out | std::ofstream::app);
    for(auto&elem : results) {
        outFile << elem; 
    }

}


void strLine2File(std::string& filePath, std::string& line) {
    std::ofstream outFile;
    outFile.open(filePath, std::ofstream::out | std::ofstream::app);
    outFile << line; 
}

// void loadPairsFromFile(std::string testfile, std::vector<Pair>& testPairs) {
//     std::ifstream infile(testfile);
//     if (infile.is_open()) {
//         std::string line;
//         while (std::getline(infile, line)) {
//             std::istringstream iss(line);
//             std::string methodStr;
//             int input;
//             iss >> methodStr >> input;

//             methodname method;
//             if (methodStr == "add")
//                 method = methodname::add;
//             else if (methodStr == "rmv")
//                 method = methodname::rmv;
//             else
//                 method = methodname::ctn;
//             testPairs.push_back({method, input});
//         }
//         infile.close();
//     }
//     printf("[Debug] Load %ld test Pairs.\n", testPairs.size() );
// }