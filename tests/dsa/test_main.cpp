#include <iostream>

// Declarations of individual test runners
void runArrayTests();
void runStackTests();
void runQueueTests();
void runLinkedListTests();
void runAlgorithmTests();

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << " RUNTIME — DSA UNIT TEST SUITE" << std::endl;
    std::cout << "========================================" << std::endl;

    try {
        runArrayTests();
        runStackTests();
        runQueueTests();
        runLinkedListTests();
        runAlgorithmTests();
        std::cout << "\n>>> ALL DSA TESTS PASSED SUCCESSFULLY! <<<\n" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n[TEST FAILED]: " << e.what() << std::endl;
        return 1;
    }
}
