	#include "Directorias.h"
	#include <iostream>
	using namespace std;
	
	Directoria::Directoria(string nome, string caminho, time_t data, SistemaFicheiros *sistema)
	    : Entrada(nome, caminho, data, sistema)
	{
	}
	
	Directoria::~Directoria()
	{
	    for (Entrada *entrada : conteudoEntrada)
	        delete entrada;
	    conteudoEntrada.clear();
	}
	
	void Directoria::adicionarEntrada(Entrada *E)
	{
	    conteudoEntrada.push_back(E);
	    conteudoEntrada.sort([](Entrada* a, Entrada* b) //para quando mover alguma directoria ou ficheiro, este ficar ordenado corretamente 
		{
	        return a->getNome() < b->getNome();
	    });
	}
	
	void Directoria::removerEntrada(string nome)
	{
	    for (auto it = conteudoEntrada.begin(); it != conteudoEntrada.end(); ++it)
	    {
	        if ((*it)->getNome() == nome)
	        {
	            delete *it;
	            conteudoEntrada.erase(it);
	            break;
	        }
	    }
	}
	
	list<Entrada *> &Directoria::getConteudo()
	{
	    return conteudoEntrada;
	}
	
	int Directoria::getTamanho()
	{
	    return conteudoEntrada.size();
	}
	
bool Directoria::isDirectoria() const
{
	return true;
}
	
void Directoria::setPai(Directoria *pai)
{
	PAI = pai;
}
	
Directoria *Directoria::getPai() const
{
	return PAI;
}
	
int Directoria::contarElementosDiretos()
{
	return conteudoEntrada.size();
}
	