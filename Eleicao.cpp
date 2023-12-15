#include "Eleicao.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <algorithm>

Eleicao::Eleicao(int tipoDeputado) : tipoDeputado(tipoDeputado){};

int Eleicao::getTipoDeputado() const{
    return this->tipoDeputado;
}

bool Eleicao::partidoExiste(string key){
    auto it = this->partidos.find(key);
    if (it != this->partidos.end()) 
        return true;
    else 
        return false;      
}

Partido* Eleicao::getPartido(string key){
    auto it = this->partidos.find(key);
    return (it->second);
}

void Eleicao::addPartido(Partido *p){
    this->partidos.insert({p->getNumPartido(),p});
}

void Eleicao::addCandidatoEleito(Candidato *c){
    this->candidatosEleitos.insert({c->getNumCandidato(), c});
}

void Eleicao::addCandidato(Candidato *c){
    this->candidatos.insert({c->getNumCandidato(), c});
}

int Eleicao::getNumCandidatosEleitos(){
    return this->candidatosEleitos.size();
}

bool Eleicao::candidatoExiste(string key){
    auto it = this->candidatos.find(key);
    if (it != this->candidatos.end()) 
        return true;
    else 
        return false;
}

Candidato* Eleicao::getCandidato(string key){
    auto it = this->candidatos.find(key);
    return it->second;
}

void Eleicao::somaVotosLegenda(int numVotos){
    this->numVotosLegenda += numVotos;
}

vector<Candidato*> Eleicao::ordenaCandidatosEleitosPorVoto(){
    
    std::vector<std::pair<std::string, Candidato*>> eleitos(candidatosEleitos.begin(), candidatosEleitos.end());

    std::sort(eleitos.begin(), eleitos.end(),
              [](const auto& a, const auto& b) {
                  return a.second->getNumVotos() > b.second->getNumVotos();
              });

    std::vector<Candidato*> candidatosOrdenados;
    for (const auto& par : eleitos) {
        candidatosOrdenados.push_back(par.second);
    }

    return candidatosOrdenados;
}

vector<Candidato*> Eleicao::ordenaCandidatosPorVoto(int tipoDeputado){
    
    std::vector<std::pair<std::string, Candidato*>> eleitos(candidatos.begin(), candidatos.end());

    std::sort(eleitos.begin(), eleitos.end(),
              [](const auto& a, const auto& b) {
                  return a.second->getNumVotos() > b.second->getNumVotos();
              });

    std::vector<Candidato*> candidatosOrdenados;
    for (const auto& par : eleitos) {
        if(par.second->getCargo() == tipoDeputado)
            candidatosOrdenados.push_back(par.second);
    }

    return candidatosOrdenados;
}
void Eleicao::calculaVotosNominais(vector<Candidato*> candidatos){
    for(Candidato *c : candidatos){
        c->getPartido().somaVotosNominal(c->getNumVotos());
        this->numVotosNominais += c->getNumVotos();   
    }  
}

vector<Partido*> Eleicao::ordenaVotosPartidos(){
    
    vector<pair<string, Partido*>> eleitos(partidos.begin(), partidos.end());

    std::sort(eleitos.begin(), eleitos.end(),
              [](const auto& a, const auto& b) {
                  return a.second->getNumVotos() > b.second->getNumVotos();
              });

    std::vector<Partido*> partidosOrdenados;
    for (const auto& par : eleitos) {    
        partidosOrdenados.push_back(par.second);
    }

    return partidosOrdenados;

}

std::string removerAspas(const std::string &str)
{
    std::string novaString = str;

    // Procurar as aspas e removê-las
    size_t pos = novaString.find('"');
    while (pos != std::string::npos)
    {
        novaString.erase(pos, 1);        // Remove uma aspa na posição encontrada
        pos = novaString.find('"', pos); // Procura a próxima aspa a partir da posição atual
    }

    return novaString;
}

void leLixo(istringstream &linhaStream, int n){
    string lixo;
    for(int i = 0; i < n; i++)
        getline(linhaStream, lixo, ';');
}

void leitura_candidatos(Eleicao &eleicao, char path[])
{

    ifstream inputStream(path);
    string linha;
    getline(inputStream, linha); // cabeçalho
    
    while (getline(inputStream, linha))
    {
        istringstream linhaStream(linha);
        string aux;

        leLixo(linhaStream, 13);

        getline(linhaStream, aux, ';');
        aux = removerAspas(aux);

        int cargo = stoi(aux); // 6- deputado federal e 7-estadual

        leLixo(linhaStream, 2);

        getline(linhaStream, aux, ';');
        string numCandidato = removerAspas(aux);
        leLixo(linhaStream, 1);
        getline(linhaStream, aux, ';');

        string nomeCandidato = removerAspas(aux);

        leLixo(linhaStream, 8);

        getline(linhaStream, aux, ';');
        string numPartido = removerAspas(aux);
        getline(linhaStream, aux, ';');
        string siglaPartido = removerAspas(aux);

        leLixo(linhaStream, 1);
        getline(linhaStream, aux, ';');
        int numFederacao = stoi(removerAspas(aux)); // numero da fereacao (-1 = sem federacao)

        leLixo(linhaStream, 11);
        
        getline(linhaStream, aux, ';');
        string dataNascimento = aux;

        leLixo(linhaStream, 2);
        
        getline(linhaStream, aux, ';');
        int genero = stoi(removerAspas(aux)); // 2-masculino e 4-feminino

        leLixo(linhaStream, 10);

        getline(linhaStream, aux, ';');
        int sit_eleito = stoi(removerAspas(aux));

        bool candidatoEleito;

        if(sit_eleito == 2 || sit_eleito == 3)
            candidatoEleito = true;
        else
            candidatoEleito = false;

        leLixo(linhaStream, 10);

        getline(linhaStream, aux, ';');
        string destinacao_votos = removerAspas(aux); // se for valido(legenda) entra para a contagem de legenda

        bool votosVaoParaLegenda;

        if(destinacao_votos == "Válido (legenda)")
            votosVaoParaLegenda = true;
        else
            votosVaoParaLegenda = false;

        getline(linhaStream, aux, ';');
        string situacaoCandidato = removerAspas(aux); // 2 ou 16 são contabilizados

        bool existePartido = eleicao.partidoExiste(numPartido);

        Partido *partido;

        if(!existePartido){
            partido = new Partido(numPartido, siglaPartido);
            eleicao.addPartido(partido);
        }
        else{ 
            partido = eleicao.getPartido(numPartido);
        }
        
        if (situacaoCandidato == "2" || situacaoCandidato == "16" || votosVaoParaLegenda){

            Federacao federacao(numFederacao);

            Candidato *candidato = new Candidato(cargo, numCandidato, nomeCandidato, partido, federacao, dataNascimento, genero, votosVaoParaLegenda, candidatoEleito);

            if(candidatoEleito && cargo == eleicao.getTipoDeputado()){
                eleicao.addCandidatoEleito(candidato);
                candidato->getPartido().incrementaNumCandidatosEleitos();
            }

            eleicao.addCandidato(candidato);
        }
    }
    inputStream.close();
}

void leitura_votacao(Eleicao &eleicao, char path[])
{

    ifstream inputStream(path);
    string linha;
    getline(inputStream, linha); // cabeçalho
    int i=0;

    while (getline(inputStream, linha))
    {
        istringstream linhaStream(linha);
        string aux;

        leLixo(linhaStream, 17);
        
        getline(linhaStream, aux, ';');
        aux = removerAspas(aux);

        int cargo = stoi(aux); // 6- deputado federal e 7-estadual

        leLixo(linhaStream, 1);

        getline(linhaStream, aux, ';');
        string nrVotavel = removerAspas(aux);

        if (cargo == eleicao.getTipoDeputado() && !(nrVotavel == "95" || nrVotavel == "96" || nrVotavel == "97" || nrVotavel == "98"))
        {
            i++;
            leLixo(linhaStream, 1);
            
            getline(linhaStream, aux, ';');

            aux = removerAspas(aux);

            
            int numVotos = stoi(aux);


            if(eleicao.candidatoExiste(nrVotavel)){

                Candidato *c = eleicao.getCandidato(nrVotavel);
                
                Partido p = c->getPartido();

                if(c->getVotosVaoParaLegenda()){
                    p.somaVotosLegenda(numVotos);
                    eleicao.somaVotosLegenda(numVotos);
                }
                else{
                    c->somaVotos(numVotos);
                }
            }    
                if(eleicao.partidoExiste(nrVotavel)){
                    Partido *p = eleicao.getPartido(nrVotavel);
                    p->somaVotosLegenda(numVotos);
                    eleicao.somaVotosLegenda(numVotos);
                }
        }        
    }
}

void gerarRelatorio(Eleicao &eleicao, int tipoDeputado){
    
    printf("Número de vagas: %d\n\n", eleicao.getNumCandidatosEleitos());

    vector<Candidato*> eleitosOrdenados = relatorioDois(eleicao);

    vector<Candidato*> candidatosOrdenados = relatorioTresEQuatro(eleicao, tipoDeputado);

    relatorioCinco(eleicao, eleitosOrdenados, candidatosOrdenados);

    relatorioSeis(eleicao, candidatosOrdenados);
}

vector<Candidato*> relatorioDois(Eleicao &eleicao){

        vector<Candidato*> candidatosEleitosOrdenados = eleicao.ordenaCandidatosEleitosPorVoto();

        string deputado;
        
        if(eleicao.getTipoDeputado() == 7) deputado = "Deputados estaduais";
        else deputado = "Deputados federais";
        
        cout << deputado + " eleitos:" << endl;

        int i=1;

        for (Candidato *c : candidatosEleitosOrdenados) {
            cout << i << " - " << c->getNome() << "(" << c->getPartido().getSiglaPartido() << ", " << c->getNumVotos() << " votos)"<< std::endl;
            i++;    
        }

        return candidatosEleitosOrdenados;
    }

    vector<Candidato*> relatorioTresEQuatro(Eleicao &eleicao, int TipoDeputado){

        cout << "\nCandidatos mais votados (em ordem decrescente de votação e respeitando número de vagas):" << endl;

        vector<Candidato*> candidatosOrdenados = eleicao.ordenaCandidatosPorVoto(TipoDeputado);

        vector<Candidato*> candidatosQueSeriamEleitos;
        int idxCandidatosQueSeriamEleitos[eleicao.getNumCandidatosEleitos()]; 

        int j = 1;
        int idx = 0;
        
        for(Candidato* c : candidatosOrdenados){
            
            if(j > eleicao.getNumCandidatosEleitos()) break;

            if(!c->getCandidatoEleito()){
                candidatosQueSeriamEleitos.push_back(c);
                idxCandidatosQueSeriamEleitos[idx] = j;
                idx++;
            }

            cout << j << " - " << c->getNome() << "(" << c->getPartido().getSiglaPartido() << ", " << c->getNumVotos() << " votos)"<< endl;
            j++;
        }

        cout << "\nTeriam sido eleitos se a votação fosse majoritária, e não foram eleitos:\n(com sua posição no ranking de mais votados)" << endl;
        
        int i=0;
        for(Candidato *c : candidatosQueSeriamEleitos){
           cout << idxCandidatosQueSeriamEleitos[i] << " - " << c->getNome() << "(" << c->getPartido().getSiglaPartido() << ", " << c->getNumVotos() << " votos)"<< endl;
           i++; 
        }
        
        return candidatosOrdenados;
    }

    void relatorioCinco(Eleicao eleicao, vector<Candidato*> candidatosEleitosOrdenados, vector<Candidato*> candidatosOrdenados){

        vector<Candidato*> candidatosQueNaoSeriamEleitos;

        int idxCandidato[candidatosEleitosOrdenados.size()];
        int idx=0; 
        
        int lastSize = candidatosEleitosOrdenados.size() -1;

        for(Candidato *c : candidatosEleitosOrdenados){

            if(c->getNumVotos() < candidatosOrdenados[lastSize]->getNumVotos()){
                
                auto it = find(candidatosOrdenados.begin(), candidatosOrdenados.end(), c);
                idxCandidato[idx] = distance(candidatosOrdenados.begin(), it)+1;
                candidatosQueNaoSeriamEleitos.push_back(c);
                idx++;
            }
        }

        cout << "\nEleitos, que se beneficiaram do sistema proporcional:\n(com sua posição no ranking de mais votados)" << endl;

        int i=0;
        for(Candidato *c : candidatosQueNaoSeriamEleitos){
            cout << idxCandidato[i] << " - " << c->getNome() << "(" << c->getPartido().getSiglaPartido() << ", " << c->getNumVotos() << " votos)"<< endl;   
            i++;
        }
    }

    void relatorioSeis(Eleicao eleicao, vector<Candidato*> candidatos){

        eleicao.calculaVotosNominais(candidatos);

        cout << "\nVotação dos partidos e número de candidatos eleitos:" << endl;

        vector<Partido*> partidos = eleicao.ordenaVotosPartidos();

        int i = 1;
        for(Partido *p : partidos){
            
            string candidatosEleitos;
        
            if(p->getNumCandidatosEleitos() > 1) candidatosEleitos = " candidatos eleitos";
            else candidatosEleitos = " candidato eleito";

            string nominais;

            if(p->getVotosNominal() > 1) nominais =  " nominais";
            else nominais = " nominal";

            string votos;

            if(p->getNumVotos() > 1) votos = " votos";
            else votos = " voto"; 

            cout << i << " - " << p->getSiglaPartido() << " - " << p->getNumPartido() << ", " << p->getNumVotos() << votos << " (" << p->getVotosNominal() << nominais << " e " << p->getVotosLegenda() << " de legenda), " << p->getNumCandidatosEleitos() << candidatosEleitos << endl;
            i++;
        }
    }