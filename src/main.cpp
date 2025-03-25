#include <string>

import solver;

int
main(int argc, char *argv[]) {
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

    return 0;
}
