#include "../inc/PsxFile.h"
#include <stdio.h>
#include <math.h>

using namespace psx::io;
using namespace std;

static void get_coverage(const string &test, const char *logPath, int &executed, int &total) {
    auto log = File::open(logPath);
    string line;
    const string file = "File '";
    const string linesExecuted = "Lines executed:";
    const string percentOf = "% of ";
    char *end;

    executed = 0;
    total = 0;
    /* test == PsxException
        File 'src/inc/PsxException.h'
        Lines executed:84.09% of 44
        Creating 'PsxException.h.gcov'
    */

    do {
        line = log.readLine(); // File 'src/inc/PsxException.h'

        if ((line.find(file) == 0) && (line.find(test + ".h") != string::npos) ) {
            line = log.readLine(); // Lines executed:84.09% of 44
            PsxAssert(line.find(linesExecuted) == 0);

            line.erase(0, linesExecuted.size()); // 84.09% of 44
            auto percent = std::strtof(line.c_str(), &end);

            line.erase(0, static_cast<size_t>(end-line.c_str())); // % of 44
            PsxAssert(line.find(percentOf) == 0);

            line.erase(0, percentOf.size()); // 44
            total = std::strtod(line.c_str(), &end);
            executed = int(round(percent * total / 100.0));
            break;
        }
    
    } while(line.size() > 0);
}

int main(const int argc, const char *argv[]) {
    const auto number_of_arguments = argc - 1;
    PsxAssert(number_of_arguments == 4);
    char *end;
    auto minimum_percent = std::strtof(argv[4], &end);
    PsxAssert(end != argv[4]);
    string test_name = argv[1];
    int executed = 0, total = 0;

    get_coverage(test_name, argv[2], executed, total);

    auto percent = executed * 100.0 / total;
    auto type = percent < minimum_percent ? "error" : "warning";

    printf("::%s file=src/inc/%s.h,line=1,col=1,endColumn=1,title=Code Coverage::%d lines executed out of %d (%0.2f%%)\n", type, test_name.c_str(), executed, total, percent);

    auto gcov = File::open(argv[3]);

    return percent < minimum_percent ? 1 : 0;
}