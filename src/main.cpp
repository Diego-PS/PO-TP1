#include <iomanip>
#include "PL.h"

int main () {
    PL P;
    std::cin >> P;
    std::cout << std::setprecision(7) << std::fixed << P.solucao() << std::endl;
}