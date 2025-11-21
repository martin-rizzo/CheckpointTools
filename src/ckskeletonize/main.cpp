#include <iostream> // for std::cout, std::endl
#include <random>   // for std::minstd_rand, std::uniform_int_distribution
#include <chrono>   // for std::chrono::system_clock

// A more interesting take on the classic "Hello World" program
// with a randomized message each time it's run
int main() {
    static const char* messages[]{
        "Hello, world!",
        "The universe acknowledges your existence.",
        "You've been chosen for something.",
        "Begging won't help you now.",
        "Obedience has its rewards",
        "Some games are played in silence",
        "Your place is already prepared",
        "Discipline leads to enlightenment"
    };
    static const int numberOfMessages = sizeof(messages) / sizeof(*messages);
    const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand generator( seed );
    std::uniform_int_distribution<int> distribution{ 0, numberOfMessages - 1 };

    // print a random message
    std::cout << messages[distribution(generator)] << std::endl;
}
