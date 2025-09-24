#include <iostream>

int main() {
    int x_cor, y_cor;
    std::cin >> x_cor >> y_cor;

    if (x_cor == 0 && y_cor == 0) {
        std::cout << "I II III IV\n"; 
    } else if (x_cor == 0 && y_cor > 0) {
        std::cout << "I II\n";
    } else if (x_cor == 0 && y_cor < 0) {
        std::cout << "III IV\n";
    } else if (x_cor > 0 && y_cor == 0) {
        std::cout << "I IV\n";
    } else if (x_cor < 0 && y_cor == 0) {
        std::cout << "II III\n";
    } else if (x_cor > 0 && y_cor > 0) {
        std::cout << "I\n";
    } else if (x_cor < 0 && y_cor > 0) {
        std::cout << "II\n";
    } else if (x_cor < 0 && y_cor < 0) {
        std::cout << "III\n";
    } else if (x_cor > 0 && y_cor < 0) {
        std::cout << "IV\n";
    } 

    return 0;
}