#ifndef FICHEIROS_H
#define FICHEIROS_H

#include "Entradas.h"

class Ficheiro : public Entrada 
{
private:
    int TAMANHO;// em bytes

public:
    Ficheiro(string nome, string caminho, int tamanho, time_t data, SistemaFicheiros* sistema);
    virtual ~Ficheiro();
    int getTamanho();
    bool isDirectoria() const;
};
#endif

