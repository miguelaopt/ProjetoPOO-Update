#include "Ficheiros.h"
#include <iostream>
using namespace std;


Ficheiro::Ficheiro(string nome, string caminho, int tamanho, time_t data, SistemaFicheiros* sistema)
    :Entrada(nome, caminho, data, sistema)
{
	TAMANHO = tamanho;
}

Ficheiro::~Ficheiro()
{
	//destructor
}

int Ficheiro::getTamanho() 
{
	return TAMANHO;
}


bool Ficheiro::isDirectoria() const
{
    return false; // é um ficheiro, não uma diretoria
}
