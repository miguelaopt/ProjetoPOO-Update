#ifndef DIRECTORIAS_H
#define DIRECTORIAS_H

#include "Entradas.h"
#include <list>

class Directoria : public Entrada 
{
private:
    list<Entrada*> conteudoEntrada;  // relação 1:N com Entrada (ficheiros e subpastas)
    Directoria *PAI = nullptr;

public:
    Directoria(string nome, string caminho, time_t data, SistemaFicheiros *sistema);
    virtual ~Directoria();

    void adicionarEntrada(Entrada *E);
    void removerEntrada(string nome);
    list<Entrada*> &getConteudo();
    int getTamanho();
    bool isDirectoria() const;
    void setPai(Directoria *pai);
    Directoria *getPai() const;
    int contarElementosDiretos();
};

#endif
