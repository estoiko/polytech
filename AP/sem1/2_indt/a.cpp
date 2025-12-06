#include <iostream>
#include <iomanip>
#include <cmath>

using TypeEps = long double;

TypeEps myExp(const TypeEps x, const TypeEps absError, const int numberMax) {
    if (!(x > -1.0L && x < 1.0L)) {
        throw std::out_of_range("ERROR (1)");
    }

    TypeEps currTerm = 1.0L;
    TypeEps accSum = currTerm;
    
    for (int i = 1; i <= numberMax; ++i) {
        currTerm *= x / i;

        if (std::fabsl(currTerm) < absError) {
            return accSum;
        }

        accSum += currTerm;
    }
 
    throw std::runtime_error("ERROR (2)");
}

int calcPrecision(TypeEps absError) {
    if (absError <= 0.0L) {
        throw std::runtime_error("ERROR: absError must be positive");
    }

    double order = std::log10(absError);
    if (order >= 0.0) {
        return 1;
    }
    
    int precision = static_cast<int>(std::ceil(-order));
    if (precision > std::numeric_limits<long double>::digits10) {
        precision = std::numeric_limits<long double>::digits10;
    }
    return precision;
}

void printTableLine(const int precision, const TypeEps i, const TypeEps absError, const int numberMax) {
    int width = std::max(precision + 2, 9); 
    int width2 = std::max(precision + 2, 6); 
    int width3 = std::max(precision + 5, 5); 

    std::cout << std::fixed << std::setprecision(precision);
    std::cout << "| " << std::setw(precision + 3) << i << " | ";

    TypeEps myExpValue = -1.0L;
    try {
        myExpValue = myExp(i, absError, numberMax);
        std::cout << std::setw(width) << std::left << myExpValue;
    } catch (const std::runtime_error &ex) {
        std::cout << std::setw(width) << std::left <<  ex.what();
    } catch (const std::out_of_range &ex) {
        std::cout << std::setw(width) << std::left << ex.what();
    }

    TypeEps stdExpValue = expl(i);
    std::cout << " | " << std::setw(width2) << std::left 
              << stdExpValue << " ";

    if (myExpValue != -1.0L) {
        std::cout << "| " << std::setw(width3) << std::left 
                  << std::fixed << std::setprecision(precision + 3)
                  << std::fabsl(myExpValue - stdExpValue) << " |\n";
    } else {
        std::cout << "| " << std::setw(width3 + 1) << std::left 
                  << "ERROR";
        std::cout << "|\n";
    }
}

void validateInput(const TypeEps iStart, const TypeEps iEnd, const TypeEps dX, const int numberMax) {
    if (dX <= 0) {
        throw std::runtime_error("ERROR: Step must be positive");
    }
    if (numberMax <= 0) {
        throw std::runtime_error("ERROR: Maximum terms must be positive");
    }
    if (iStart >= iEnd) {
        throw std::runtime_error("ERROR: Start must be less than end");
    }
}

int main() {
    TypeEps absError;
    std::cin >> absError;

    int outPrecision = 1;
    try {
        outPrecision = calcPrecision(absError);
    } catch (const std::runtime_error &ex) {
        std::cerr << ex.what() << "\n";
        return 1;
    }

    int numberMax;
    std::cin >> numberMax;
    
    TypeEps iStart, iEnd, dX;
    std::cin >> iStart >> iEnd >> dX;

    try {
        validateInput(iStart, iEnd, dX, numberMax);
    } catch (const std::runtime_error &ex) {
        std::cerr << ex.what() << "\n";
        return 2;
    }

    std::cout << "\n";

    int width = std::max(outPrecision + 2, 9);

    std::cout << "| " << std::setw(outPrecision + 3) << std::left << "x" << " | "
              << std::setw(width) << std::left << "func" << " | "
              << std::setw(outPrecision + 2) << std::left << "expl()" << " | "
              << std::setw(outPrecision + 6) << std::left << "diff" << "|\n";

    long long steps = llround((iEnd - iStart) / dX);
    if (steps < 0) steps = 0;

    for (long long i = 0; i <= steps; ++i) {
        TypeEps x = iStart + i * dX;
        printTableLine(outPrecision, x, absError, numberMax);
    }

    std::cout << "\n";
    std::cout << "ERROR (1): X is not within the given interval\n";
    std::cout << "ERROR (2): Precision can't be achieved\n";
}