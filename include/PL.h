#include "Simplex.h"

#ifndef PL_H
#define PL_H

class PL
{
    public:
        int PL_M = 0;
        int PL_N = 0;
        Tableau* _T;

    public:
        PL () {}
        ~PL () 
        {
            delete _T;
        }

        Solucao solucao ()
        {
            Solucao sol_tableau = _T->simplex();
            Solucao S;
            if (sol_tableau.classificacao == "inviavel") {
                S = sol_tableau;
            }
            if (sol_tableau.classificacao == "ilimitada") {
                S = sol_tableau;
                double nova_solucao_viavel[PL_N];
                for (int i = 0; i < PL_N; i++)
                    nova_solucao_viavel[i] = (*S.solucao_viavel)(i);
                double novo_certificado_ilimitada[PL_N];
                for (int i = 0; i < PL_N; i++)
                    novo_certificado_ilimitada[i] = (*S.certificado_ilimitada)(i);
                S.setSolucao_viavel(PL_N, nova_solucao_viavel);
                S.setCertificado_ilimitada(PL_N, novo_certificado_ilimitada);
            }
            if (sol_tableau.classificacao == "otima") {
                S = sol_tableau;
                double nova_solucao_otima[PL_N];
                for (int i = 0; i < PL_N; i++)
                    nova_solucao_otima[i] = (*S.solucao_otima)(i);
                S.setSolucao_otima(PL_N, nova_solucao_otima);
            }
            return S;
        }

        friend std::istream& operator>> (std::istream& is, PL& p)
        {
            // Lemos a PL no formato especificado com <= e acrescentamos as variaveis de folga para montar o tableau

            int m, n;
            is >> m >> n;

            int M, N;
            M = m+1;
            N = n+1+m+m;
            p.PL_M = m;
            p.PL_N = n;

            double matriz[M*N];
            for (int i = 0; i < M*N; i++)
                matriz[i] = 0;

            p._T = new Tableau(M, N, matriz);

            // Leitura do vetor c
            for (int i = 0; i < n; i++) {
                double c;
                is >> c;
                (p._T->c)(i) = -c;
            }
            
            // Leitura da matriz A e vetor b
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++)
                    is >> (p._T->A)(i, j);
                is >> (p._T->b)(i);
            }

            // Registro de operacoes
            for (int i = 0; i < m; i++)
                (p._T->ro)(i, i) = 1;

            // Variaveis de folga
            for (int i = 0; i < m; i++)
                (p._T->A)(i, i+n) = 1;

            // Checar os valores zero
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    corrige_zero((*p._T)(i, j));

            return is;
        }
};

#endif