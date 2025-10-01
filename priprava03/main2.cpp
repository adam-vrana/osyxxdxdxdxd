#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <memory>
#include <utility>
#include <cerrno>

using namespace std;

int main(int argc, char *argv[]) {
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    int timeOut = 60000000 / n; // milliseconds

    

    srand(time(NULL));
    while (true) {
        int rnd = rand() % m + 1;
        for (int i = 0; i < rnd; i++) {
            printf("%d ", (rand() % 10000 - 9) + 10);
            fflush(stdout);

        }
        usleep(timeOut);
        printf("\n");
    }
}