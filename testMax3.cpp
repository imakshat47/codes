#include <iostream>
#include <algorithm> // for std::max
#include <initializer_list> // for initializer_list
using namespace std;

int main() {
    double a, b, c;

    cout << "Enter three numbers: ";
    if (!(cin >> a >> b >> c)) { // Input validation
        cerr << "Invalid input. Please enter numeric values.\n";
        return 1;
    }

    // Using std::max with initializer_list
    double maximum = std::max({a, b, c});

    cout << "The maximum of the three numbers is: " << maximum << endl;
    return 0;
}
