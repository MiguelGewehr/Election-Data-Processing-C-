#include "Partido.h"

Partido::Partido(string numPartido, string siglaPartido) : numPartido(numPartido), siglaPartido(siglaPartido){}

string Partido::getNumPartido() const{
    return this->numPartido;
}

void Partido::incrementaNumCandidatosEleitos(){
    this->numCandidatosEleitos++;
}

void Partido::somaVotosLegenda(int numVotos){
    this->numVotosLegenda += numVotos;
}