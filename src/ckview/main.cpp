#include <iostream>
#include <random>
#include <chrono>

// A more interesting take on the classic "Hello World" program
// with a randomized message each time it's run
int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 8); // Updated range

    const char* messages[] {
        "Hello, world!",
        "The universe acknowledges your existence.",
        "You've been chosen for something.",
        "Begging won't help you now.",
        "Obedience has its rewards",
        "Some games are played in silence",
        "Your place is already prepared",
        "Discipline leads to enlightenment"
    };
    std::cout << messages[dist(gen)] << std::endl;
}

