#include <iostream>

int main() {
    int mass;
    int sum = 0;
    while(std::cin >> mass) {
        mass = mass / 3 - 2;
        while(mass > 0) {
            sum += mass;
            mass = mass / 3 - 2;
        }
    }
    std::cout << sum << std::endl;
}