#include "../inc/PsxFile.h"
#include <stdio.h>
#include <math.h>

using namespace psx;
using namespace std;

static void get_coverage(const string &test, const char *logPath, int &executed, int &total, string &path) {
    auto log = File::open(logPath);
    string line;
    const string file = "File '";
    const string linesExecuted = "Lines executed:";
    const string percentOf = "% of ";
    char *end;

    executed = 0;
    total = 0;
    path.clear();

    /* test == PsxException
        File 'src/inc/PsxException.h'
        Lines executed:84.09% of 44
        Creating 'PsxException.h.gcov'
    */

    do {
        line = log.readLine(); // File 'src/inc/PsxException.h'

        if ((line.find(file) == 0) && (line.find(test + ".h") != string::npos) ) {
            line.erase(0, file.size()); // src/inc/PsxException.h'

            auto apostrophe = line.find("'");
            PsxAssert(apostrophe != string::npos);
            path = line.substr(0, apostrophe);

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

static string trim(const string &value, const char *set=" \t\r\n") {
    auto trimmed = value;
    const auto first = value.find_first_not_of(set);

    if (first == string::npos) {
        trimmed.clear();
        return trimmed;
    }

    trimmed.erase(0, first);

    const auto last = trimmed.find_last_not_of(set);
    PsxAssert(last != string::npos);

    trimmed.erase(last + 1);
    return trimmed;
}

static void parse_gcov(const string &path) {
    auto gcov = psx::File::open(path);
    bool seen_code_yet = false;

    while (!gcov.endOfFile()) {
        auto line = gcov.readLine();
        
        if (line.size() == 0) {
            break;
        }
        
        const auto firstField = line.find(":"); // status
        if (firstField == string::npos) {
            printf("WHAT1: %s\n", line.c_str());
            continue;
        }
        PsxAssert(firstField != string::npos);
        const auto secondField = line.find(":", firstField + 1); // line number
        if (secondField == string::npos) {
            printf("WHAT2: %s\n", line.c_str());
            continue;
        }
        PsxAssert(secondField != string::npos);
        auto type = trim(line.substr(0, firstField));
        auto lineNumber = trim(line.substr(firstField+1, secondField - firstField - 1));
        auto code = trim(line.substr(secondField + 1), " \t\r\n};");
        
        seen_code_yet = seen_code_yet || (type != "-" && code.find("class") != 0);
        const auto unexecuted = (type[0] == '#') || (type[0] == '=') || (seen_code_yet && type == "-");
        const auto empty = code.size() == 0;
        const auto marked = code.find("// NOTEST") != string::npos;
        const auto preprocessor = code[0] == '#';
        const auto commented = code.find("//") == 0;
        const auto templat = code.find("template") == 0;

        if (unexecuted && !empty && !marked && !preprocessor && !commented && !templat) {
            printf("%s:%s\n", lineNumber.c_str(), trim(line.substr(secondField + 1), "\r\n").c_str());
        }


    }
}

int main(const int argc, const char *argv[]) {
    const auto number_of_arguments = argc - 1;
    PsxAssert(number_of_arguments == 4);
    char *end;
    auto minimum_percent = std::strtof(argv[4], &end);
    PsxAssert(end != argv[4]);
    string test_name = argv[1];
    int executed = 0, total = 0;
    string source_path;

    get_coverage(test_name, argv[2], executed, total, source_path);

    auto percent = executed * 100.0 / total;
    auto type = percent < minimum_percent ? "error" : "notice";

    printf("::%s file=%s,line=1,col=1,endColumn=1,title=Code Coverage::%d lines executed out of %d (%0.2f%%)\n", type, source_path.c_str(), executed, total, percent);

    parse_gcov(argv[3]);

    return percent < minimum_percent ? 1 : 0;
}