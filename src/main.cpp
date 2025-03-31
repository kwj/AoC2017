#include <exception>
#include <iostream>
#include <string>

import solver;

int
main(int argc, char *argv[]) {
    try {
        if (argc == 1) {
            solver::runAllSolver();
        } else {
            auto day = std::stoi(argv[1]);
            if (argc == 2) {
                solver::runSolver(day);
            } else {
                solver::runSolver(day, argv[2]);
            }
        }
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;

        return -1;
    }

    return 0;
}
