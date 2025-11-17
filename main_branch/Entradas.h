#ifndef ENTRADAS_H
#define ENTRADAS_H

#include <string>
#include <iostream>
using namespace std;

class SistemaFicheiros; // Evitar dependência circular

class Entrada
{
	private:
	    string NOME;
        string CAMINHO;
        time_t DATA;// formato DD/MM/AAAA
        SistemaFicheiros *Sistema; //ponteiro por ser 1:N (1 do lado do Sistema)
	public:
		Entrada(string nome, string caminho, time_t data, SistemaFicheiros *sistema);
		virtual ~Entrada();
		string getNome();
        string getCaminho();
        time_t getData();
        SistemaFicheiros *getSistema();
        void setNome(string novoNome);
        void setCaminho(string novoCaminho);
        void setData(time_t novaData);
        string getDataFormatada();
        virtual bool isDirectoria() const = 0;
};


#endif
