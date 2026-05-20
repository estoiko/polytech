#include <iostream> 
#include <array>
#include <algorithm>

// with sorting a built-in array
int solve() 
{
	int a, b = 0;
	std::cin >> a >> b;
	
	if (a <= 0 || b <= 0)
	{
		std::cout << "А и B должны быть больше 0\n";
		return -1;
	}
	
	int x, y, z = 0;
	std::cin >> x >> y >> z;
	
	if (x <= 0 || y <= 0 || z <= 0)
	{
		std::cout << "X, Y и Z должны быть больше 0\n";
		return -1;
	}
	
	int arr[] = {x, y, z};
	std::sort(arr, arr + 3);
	
	if (arr[0] < std::min(a, b) && arr[1] < std::max(a, b))
	{
		std::cout << "Кирпич входит в отверстие\n";
	}
	else
	{
		std::cout << "Кирпич не входит в отверстие\n";
	}
	
	return 0;
}

// only using if-else statements
int solve2()
{

	int a, b = 0;
	std::cin >> a >> b;
	
	if (a <= 0 || b <= 0)
	{
		std::cout << "А и B должны быть больше 0\n";
		return -1;
	}
	
	int x, y, z = 0;
	std::cin >> x >> y >> z;
	
	if (x <= 0 || y <= 0 || z <= 0)
	{
		std::cout << "X, Y и Z должны быть больше 0\n";
		return -1;
	}
	
	if (x < a && y < b || x < b && y < a)
	{
		std::cout << "Кирпич входит в отверстие\n";
	}
	else if (x < a && z < b || x < b && z < a)
	{
		std::cout << "Кирпич входит в отверстие\n";
	}
	else if (z < a && y < b || z < b && y < a)
	{
		std::cout << "Кирпич входит в отверстие\n";
	}
	else
	{
		std::cout << "Кирпич не входит в отверстие\n";
	}
	
	return 0;
}

int main() 
{
    solve();
    return 0;
}

