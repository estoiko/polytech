#include <iostream> 

int main() 
{
    int a, b = 0;
    std::cin >> a >> b;

    if (a <= 0 || b <= 0)
    {
        std::cout << "a and b should be greater than 0\n";
        return -1;
    }

    int x, y, z = 0;
    std::cin >> x >> y >> z;

    if (x <= 0 || y <= 0 || z <= 0) 
    {
        std::cout << "x, y and z should be greater than 0\n";
        return -1;
    }
    
    // without sort func
    if (x < a && y < b || x < b && y < a) 
    {
        std::cout << "yes\n";
    }
    else if (x < a && z < b || x < b && z < a)
    {
        std::cout << "yes\n";
    }
    else if (z < a && y < b || z < b && y < a)
    {
        std::cout << "yes\n";
    }
    else 
    {
        std::cout << "no\n";
    }

    return 0;
}