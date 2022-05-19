#include <iostream>

#ifndef MATRIZ_H
#define MATRIZ_H

const bool debug = false;
const double zero = 1e-7;

void corrige_zero (double &x)
{
    if (-zero < x && x < zero) x = 0;
}

template <typename T = double>
class Linha
{
    private:
        int _N;
        T * _L;

    public:
        Linha () {}
        Linha(int N, T L[])
        {
            _N = N;
            _L = new T[N];
            
            for (int i = 0; i < N; i++)
                _L[i] = L[i];
        }
        ~Linha()
        {
            delete[] _L;

            if (debug) std::cout << "Destruiu a linha\n";
        }

        int tamanho () { return _N; }

        T& operator() (int x)
        {
            return _L[x];
        }
        Linha& operator+= (Linha& l)
        {
            int N = this->tamanho();
            if (l._N != N) throw std::invalid_argument("Linhas de tamanhos distintos");
            for (int i = 0; i < N; i++) {
                (*this)(i) += l(i);
                corrige_zero((*this)(i));
            }
            return *this;
        }
        Linha& operator*= (const T c)
        {
            int N = this->tamanho();
            for (int i = 0; i < N; i++) {
                (*this)(i) *= c;
                corrige_zero((*this)(i));
            }
            return *this;
        }
        Linha& operator/= (const T c)
        {
            int N = this->tamanho();
            for (int i = 0; i < N; i++) {
                (*this)(i) /= c;
                corrige_zero((*this)(i));
            }
            return *this;
        }

        friend std::ostream& operator<< (std::ostream& op, Linha& l)
        {
            for (int i = 0; i < l._N; i++)
                op << l(i) << " ";
            return op;
        }
};

template <typename T = double>
class Matriz
{
    public:

        int _M;
        int _N;
        Linha<T>** _A;

    public:
        Matriz () {}
        Matriz (int M, int N, T A[]) 
        {
            _M = M;
            _N = N;
            _A = new Linha<T>*[M];
            for (int i = 0; i < M; i++) {
                T v[N];
                for (int j = 0; j < N; j++)
                    v[j] = A[i*N + j];
                Linha<T> *l = new Linha<T>(N, v);
                _A[i] = l;
            }
        }
        ~Matriz ()
        {
            for (int i = 0; i < _M; i++)
                delete _A[i];
            delete[] _A;

            if (debug) std::cout << "Destruiu a matriz\n";
        }

        Linha<T>& operator() (int x)
        {
            return *_A[x];
        }
        T& operator() (int x, int y)
        {
            return (*_A[x])(y);
        }
        void pivo (int x, int y)
        {
            if ((*this)(x, y) == 0) throw std::invalid_argument("Nao e possivel pivotear um valor nulo");
            (*this)(x) /= (*this)(x, y);
            for (int i = 0; i < _M; i++) {
                if (i != x) {
                    T value = (*this)(i, y);
                    if (value == 0) continue;
                    T multiplo[_N];
                    for (int i = 0; i < _N; i++)
                        multiplo[i] = -value*(*this)(x, i);
                    Linha<T> *l = new Linha<T>(_N, multiplo);
                    (*this)(i) += *l;
                    delete l;
                }
            }
        }

        friend std::ostream& operator<< (std::ostream& op, Matriz& m)
        {
            for (int i = 0; i < m._M; i++)
                op << m(i) << std::endl;
            return op;
        }
};

#endif