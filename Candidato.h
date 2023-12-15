#ifndef _CANDIDATO_H_
#define _CANDIDATO_H_

#include <iostream>
#include <string>

using namespace std;

class Partido;
#include "Federacao.h"

class Candidato
{
    int cargo;
    string numCandidato;
    string nomeCandidato;
    Partido *partido;
    Federacao federacao;
    string dataNascimento;
    bool candidatoEleito;
    int genero;
    bool votosVaoParaLegenda;
    int numVotos=0;

public:
    Candidato(int cargo, string numCandidato, string nomeCandidato, Partido *partido, Federacao federacao, string dataNascimento, int genero, bool votosVaoParaLegenda, bool candidatoEleito);
    void somaVotos(int numVotos);
    int getCargo() const;
    string getNumCandidato() const;
    string getNome() const;
    Partido* getPartido();
    Federacao getFederacao();
    string getDataNascimento();
    bool getCandidatoEleito() const;
    int getGenero();
    bool getVotosVaoParaLegenda() const;
    int getNumVotos() const;
};

#endif