#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]) {
    vector<string> args;
    vector<string> files;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            args.push_back("-s");
        } else if (strcmp(argv[i], "-r") == 0) {
            args.push_back("-r");
        } else if (strcmp(argv[i], "-t") == 0) {
            args.push_back("-t");
        } else {
            files.push_back(argv[i]);
        }
    }

    for (auto &file : files) {
        struct stat fileStat;

        if (stat(file.c_str(), &fileStat) < 0) {
            fprintf(stderr, "Error: Cannot access file %s\n", file.c_str());
            continue;
        }

        string command;
        for (auto &arg : args) {
            if (arg == "-s") {
                command += "size " + to_string(fileStat.st_size);
            } else if (arg == "-r") {
                command += "permissions " + to_string(fileStat.st_mode & 0777);
            } else if (arg == "-t") {
                struct tm *timeinfo = localtime(&fileStat.st_mtime);
                command += "last_modified ";
                command += asctime(timeinfo);
                if (!command.empty() && command.back() == '\n') {
                    command.pop_back(); // odstranit newline
                }
            }
            command += "\t";
        }

        command += file;
        printf("%s\n", command.c_str());
    }
}
