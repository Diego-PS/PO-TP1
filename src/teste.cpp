#include "PL.h"

int main () {
    /*
    int m = 4, n = 3;
    double matriz[] = {
        -5, -1, 6,
        10, 5, 18,
        19, 0, 2,
        5, -3, 0
    };
    Matriz<> M(m, n, matriz);
    std::cout << M << std::endl;
    M.pivo(0,0);
    std::cout << M << std::endl;
    */

   /*
   int m = 4, n = 7;
   double matriz[] = {
       0, 0, 0, -2, -4, -8, 0,
       1, 0, 0, 1, 0, 0, 1,
       0, 1, 0, 0, 1, 0, 1,
       0, 0, 1, 0, 0, 1, 1
   };
   Tableau T(m, n, matriz, 3, 2);
   std::cout << T << std::endl;
   std::cout << (*T.A)(0, 0) << std::endl;
   //T.A->pivo(0, 0);
   std::cout << T << std::endl;
   int base[] = {0, 1, 2};
   std::cout << T.simplex(3, base) << std::endl;
   */

  PL P;
  std::cin >> P;
  std::cout << P.solucao() << std::endl;
}