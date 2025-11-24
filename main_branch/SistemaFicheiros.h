#ifndef SISTEMAFICHEIROS_H
#define SISTEMAFICHEIROS_H

#include <iostream>
using namespace std;
#include <list>
#include <string>
#include <fstream>
#include <iomanip> // para o "put_time"
#include <set>
#include <sstream>

class Entrada;
class Directoria;
class Ficheiro;

class SistemaFicheiros
{
	private:
		list<Entrada*> Entradas; //lista por ser 1:N (N do lado das Entradas)
		void LoadAux(Directoria *directoriaPai, const string &path, SistemaFicheiros *sistema);
        int contarFicheirosAux(Entrada *E);
        int contarDirectoriasAux(Entrada *E);
        int memoriaAux(Entrada *E);
        Directoria *DirectoriaMaisElementosAux(Entrada *E, int &maxElementos);
        Directoria *DirectoriaMenosElementosAux(Entrada *E, int &minElementos);
        Ficheiro *FicheiroMaiorAux(Entrada *entrada, Ficheiro *atualMaior);
        Directoria *DirectoriaMaisEspacoAux(Entrada *entrada, int &maxEspaco);
        string *SearchAux(Entrada *entrada, const string &nome, int Tipo);
        bool RemoverAllAux(Directoria *directoria, bool removerDirectorias);
        static string formatarData(time_t t);
        void Escrever_XML_Aux(ofstream &f, Directoria *directoria, int nivel);
        Directoria *Ler_XML_Aux(ifstream &f, string linha);
        Directoria *ProcurarDirectoria(Entrada* entrada, const string& nome);
        bool MoveFicheiroAux(Directoria *directoria, const string &Fich, Directoria *DirNova);
        bool MoverDirectoriaAux(Entrada *atual, Directoria *origem, Directoria *destino);
        string *DataFicheiroAux(Entrada *entrada, const string &nome);
        void TreeAux(Directoria *dir, ostream &out, int nivel);
        void PesquisarAllDirectoriasAux(Entrada *entrada, const string &nome, list<string> &res);
        void PesquisarAllFicheirosAux(Entrada *entrada, const string &nome, list<string> &res);
        void RenomearFicheirosAux(Entrada *entrada, const string &fich_old, const string &fich_new);
        bool FicheiroDuplicadosAux(Entrada *entrada, std::set<string> &nomesFicheiros);
        
	public:
		SistemaFicheiros();
		virtual ~SistemaFicheiros();
		bool Load(const string &path);
		int ContarFicheiros();
		int ContarDirectorias();
		int Memoria(); 
        string *DirectoriaMaisElementos ();
        string *DirectoriaMenosElementos ();
        string *FicheiroMaior();
        string *DirectoriaMaisEspaco();
        string *Search(const string &s, int Tipo);
        bool RemoverAll(const string &s, const string &tipo);
        void Escrever_XML(const string &s);
        bool Ler_XML(const string &s);
        bool MoveFicheiro(const string &Fich, const string &DirNova);
        bool MoverDirectoria(const string &DirOld, const string &DirNew);
        string DataFicheiro(const string &ficheiro);
        void Tree(const string *fich = nullptr);
        void PesquisarAllDirectorias(list<string> &lres, const string &dir);
        void PesquisarAllFicheiros(list<string> &lres, const string &file);
        void RenomearFicheiros(const string &fich_old, const string &fich_new);
        bool FicheiroDuplicados();
        void MostrarConteudo(Directoria* dir, int nivel = 0);
        bool CopyBatch(const string &padrao, const string &DirOrigem, const string &DirDestino);
};

#endif