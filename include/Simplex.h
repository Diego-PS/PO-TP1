#include "Matriz.h"

#ifndef SIMPLEX_H
#define SIMPLEX_H

class Solucao
{
    public:
        int tamanho_x;
        std::string classificacao;
        Linha<>* certificado_ilimitada = nullptr;
        Linha<>* solucao_otima = nullptr;
        Linha<>* certificado_otima = nullptr;
        Linha<>* certificado_inviabilidade = nullptr;
        Linha<>* solucao_viavel = nullptr;
        double valor_otimo;
        Linha<int>* base_viavel = nullptr;

        Solucao () {}
        ~Solucao () {}

        void setTamanho_x (int tam) { tamanho_x = tam; }
        void setClassificacao (std::string clas) { classificacao = clas; }
        void setCertificado_ilimitada (int n, double certificado[]) { certificado_ilimitada = new Linha<>(n, certificado); }
        void setSolucao_otima (int n, double solucao[]) { solucao_otima = new Linha<>(n, solucao); }
        void setCertificado_otima (int n, double certificado[]) { certificado_otima = new Linha<>(n, certificado); }
        void setCertificado_inviabilidade (int n, double certificado[]) { certificado_inviabilidade = new Linha<>(n, certificado); }
        void setSolucao_viavel (int n, double solucao[]) { solucao_viavel = new Linha<>(n, solucao); }
        void setValor_otimo (double val) { valor_otimo = val; }
        void setBase (int n, int base[]) { base_viavel = new Linha<int>(n, base); }

        friend std::ostream& operator<< (std::ostream& op, Solucao s)
        {
            if (s.classificacao == "otima") {
                op << "otima" << std::endl;
                if (debug) op << "valor otimo: ";
                op << s.valor_otimo << std::endl;
                if (debug) op << "solucao otima: ";
                op << *s.solucao_otima << std::endl;
                if (debug) op << "certificado de otimalidade: ";
                op << *s.certificado_otima;
            }
            if (s.classificacao == "ilimitada") {
                op << "ilimitada" << std::endl;
                if (debug) op << "solucao viavel: ";
                op << *s.solucao_viavel << std::endl;
                if (debug) op << "certificado de ilimitada: ";
                op << *s.certificado_ilimitada;
            }
            if (s.classificacao == "inviavel") {
                op << "inviavel" << std::endl;
                if (debug) op << "certificado de inviabilidade: ";
                op << *s.certificado_inviabilidade;
            }
            return op;
        }
};

class Tableau : public Matriz<>
{
    class A_matriz 
    {
        public:
            Tableau& t;
            int m;
            int n;
            A_matriz(Tableau& _t) : t(_t) 
            {
                m = _t._M-1;
                n = _t._N - _t._M;
            }
            void pivo (int x, int y) { t.pivo(x+1, y+t._M-1); }
            double& operator() (int x, int y) { return t(x+1, y+t._M-1); }
    };
    class b_coluna
    {
        public:
            Tableau& t;
            int n;
            b_coluna(Tableau& _t) : t(_t) { n = _t._M-1; }
            double& operator() (int x) { return t(x+1, t._N-1); }
    };
    class c_linha
    {
        public:
            Tableau& t;
            int n;
            c_linha(Tableau& _t) : t(_t) { n = _t._N - _t._M; }
            double& operator() (int x) { return t(0, x+t._M-1); }
    };
    class d_linha
    {
        public:
            Tableau& t;
            int n;
            d_linha(Tableau& _t) : t(_t) { n = _t._M-1; }
            double& operator() (int x) { return t(0, x); }
    };
    class ro_matriz
    {
        public:
            Tableau& t;
            int n;
            ro_matriz(Tableau& _t) : t(_t) { n = _t._M-1; }
            double& operator() (int x, int y) { return t(x+1, y); }
    };

    public:
        A_matriz& A;
        b_coluna& b;
        c_linha& c;
        d_linha& d;
        ro_matriz& ro;

        Tableau (int M, int N, double matriz[]) : Matriz<>(M, N, matriz), A(*(new A_matriz(*this))), b(*(new b_coluna(*this))), c(*(new c_linha(*this))), d(*(new d_linha(*this))), ro(*(new ro_matriz(*this))) {}
        ~Tableau () {
            delete &A;
            delete &b;
            delete &c;
            delete &d;
            delete &ro;
        }

        double& valor_otimo () { return (*this)(0, _N-1); }

        Solucao simplex (int tam, int base[])
        {
            if (debug) {
                std::cout << "base: ";
                for (int i = 0; i < tam; i++)
                    std::cout << base[i] << " ";
                std::cout << std::endl;
                std::cout << (*this) << std::endl;
            }

            // Colocamos a PL em formato canonico
            for (int i = 0; i < tam; i++) {
                if (debug) std::cout << "Estou tentando pivotear " << i << " " << base[i] << std::endl;
                A.pivo(i, base[i]);
                if (debug) std::cout << (*this) << std::endl;
            }
            
            // Escolhemos a primeira coluna cuja primeira entrada seja negativa
            int y = -1;
            for (int i = 0; i < c.n; i++) {
                if (c(i) < 0) {
                    y = i;
                    break;
                }
            }

            if (debug) std::cout << "coluna: " << y << std::endl;

            // Se nao encontramos, terminamos o simplex e sabemos a solucao otima
            if (y == -1) {
                Solucao S;
                S.setClassificacao("otima");

                int n = _N - 1;
                double solucao_otima[n];
                for (int i = 0; i < n; i++)
                    solucao_otima[i] = 0;
                for (int i = 0; i < tam; i++)
                    solucao_otima[base[i]] = b(i);
                S.setSolucao_otima(n, solucao_otima);

                n = _M - 1;
                
                double certificado_otima[n];
                for (int i = 0; i < n; i++)
                    certificado_otima[i] = d(i);
                S.setCertificado_otima(n, certificado_otima);

                S.setValor_otimo(this->valor_otimo());

                S.setBase(tam, base);
                return S;
            }

            // Agora procuramos o elemento com a menor razão b(i)/e(i, y)
            const double inf = 1e300;
            double menor = inf;
            int x = -1;
            for (int i = 0; i < b.n; i++) {
                double e = A(i, y);
                if (e <= 0) continue;
                if (b(i)/e < menor) {
                    menor = b(i)/e;
                    x = i;
                }
            }

            if (debug) std::cout << "linha: " << x << std::endl;

            // Se nenhum valor da coluna e positivo, a PL e ilimitada
            if (x == -1) {
                Solucao S;
                S.setClassificacao("ilimitada");
                int n = _N - 1;
                double certificado[n];
                for (int i = 0; i < n; i++)
                    certificado[i] = 0;
                certificado[y] = 1;
                for (int i = 0; i < tam; i++) {
                    int k = base[i];
                    for (int j = 0; j < _M-1; j++) {
                        if (A(j, k) != 0)
                            certificado[k] = -A(j, y);
                    }
                }
                S.setCertificado_ilimitada(n, certificado);
                double solucao[n];
                for (int i = 0; i < n; i++)
                    solucao[i] = 0;
                for (int i = 0; i < tam; i++)
                    solucao[base[i]] = b(i);
                S.setSolucao_viavel(n, solucao);
                return S;
            }

            // Entao alteramos a x-esima coluna da base por y
            base[x] = y;

            // E repetimos o processo
            return simplex(tam, base);
        }

        Solucao simplex ()
        {
            if (debug) std::cout << "Agora estou comecando o simplex sem base\n";

            // Primeiro fazemos b >= 0
            for (int i = 0; i < b.n; i++)
                if (b(i) < 0)
                    (*this)(i+1) *= -1;

            if (debug) std::cout << (*this) << std::endl;
            
            // Agora construimos a PL auxiliar
            int M = _M;
            int N = _N+_M-1;
            double matriz[M*N];
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    matriz[i*N + j] = 0;

            Tableau *auxiliar = new Tableau(M, N, matriz);
            
            // primeira linha da PL auxiliar
            for (int i = c.n; i < _N-1; i++)
                (auxiliar->c)(i) = 1;

            if (debug) {
                std::cout << "primeira linha da auxiliar\n";
                std::cout << (*auxiliar) << std::endl;
            }

            // a matriz A
            for (int i = 0; i < A.m; i++)
                for (int j = 0; j < A.n; j++)
                    (auxiliar->A)(i, j) = A(i, j);

            if (debug) {
                std::cout << "a matriz A\n";
                std::cout << (*auxiliar) << std::endl;
            }

            // a matriz identidade
            for (int i = 0; i < _M-1; i++)
                (auxiliar->A)(i, _N-_M+i) = 1;

            if (debug) {
                std::cout << "a matriz identidade\n";
                std::cout << (*auxiliar) << std::endl;
            }

            // a matriz de registro de operacoes
            for (int i = 0; i < ro.n; i++)
                (auxiliar->ro)(i, i) = ro(i, i);

            if (debug) {
                std::cout << "a matriz de registro de operacoes\n";
                std::cout << (*auxiliar) << std::endl;
            }
            
            // o vetor b
            for (int i = 0; i < b.n; i++)
                (auxiliar->b)(i) = b(i);
            
            if (debug) {
                std::cout << "o vetor b\n";
                std::cout << (*auxiliar) << std::endl;
            }

            // verificamos o zero
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    corrige_zero((*auxiliar)(i, j));

            // acabamos de criar a PL auxiliar
            if (debug) {
                std::cout << "Esta e a PL auxiliar:\n";
                std::cout << *auxiliar << std::endl;
            }

            int base[M-1];
            for (int i = 0; i < M-1; i++)
                base[i] = _N-_M+i;

            Solucao solucao_aux = auxiliar->simplex(M-1, base);

            // atualizamos o tableau

            for (int i = 0; i < ro.n; i++)
                for (int j = 0; j < ro.n; j++)
                    ro(i, j) = (auxiliar->ro)(i, j);

            for (int i = 0; i < A.m; i++)
                for (int j = 0; j < A.n; j++)
                    A(i, j) = (auxiliar->A)(i, j);

            for (int i = 0; i < b.n; i++)
                b(i) = (auxiliar->b)(i);

            this->valor_otimo() = auxiliar->valor_otimo();

            if (debug) std::cout << "Solucao da PL auxiliar:\n" << solucao_aux << std::endl;

            Solucao S;

            if (solucao_aux.valor_otimo != 0) {
                S.setClassificacao("inviavel");
                int n = solucao_aux.certificado_otima->tamanho();
                double certificado[n];
                for (int i = 0; i < n; i++)
                    certificado[i] = (*solucao_aux.certificado_otima)(i);
                S.setCertificado_inviabilidade(n, certificado);
            } else {
                int n = 0;
                for (int i = 0; i < solucao_aux.base_viavel->tamanho(); i++)
                    if ((*solucao_aux.base_viavel)(i) < A.n) n++;
                int base_viavel[n];
                int j = 0;
                for (int i = 0; i < n; i++) {
                    if ((*solucao_aux.base_viavel)(i) < A.n)
                        base_viavel[j++] = (*solucao_aux.base_viavel)(i);
                }
                S = simplex(n, base_viavel);
            }

            delete auxiliar;

            return S;
        }
};

#endif