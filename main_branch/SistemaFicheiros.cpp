#include "SistemaFicheiros.h"
#include "Ficheiros.h"
#include "Directorias.h"
#include <filesystem>
#include <iostream>
#include <chrono>
#include <ctime>
using namespace std;
namespace fs = std::filesystem;

SistemaFicheiros::SistemaFicheiros() 
{
	//
}

SistemaFicheiros::~SistemaFicheiros() 
{
	for (Entrada *entrada : Entradas)
    {
        if (entrada->isDirectoria())
        {
         ((Directoria*)entrada)->getConteudo().clear();       	
	    } 
        delete entrada;
    }
    Entradas.clear();
}

// Função auxiliar recursiva para o Load
void SistemaFicheiros::LoadAux(Directoria *directoriaPai, const string &path, SistemaFicheiros *sistema)
{
    for (auto &entrada : fs::directory_iterator(path)) //Loop que percorre todos os ficheiros e subpastas dentro da directoria path
    //fs::directory_iterator faz parte da biblioteca filesystem e fornece cada entrada da directoria
    {
        string nome = entrada.path().filename().string();
        string caminho = entrada.path().string();

        // Obter data da última modificação como time_t usando chrono (visto no chatgpt)
        auto ftime = fs::last_write_time(entrada);//retorna a hora da última modificação
        auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(ftime - fs::file_time_type::clock::now() + chrono::system_clock::now());//conversão para ficar no formato time_t tal como definido em "entradas.h"
        time_t data = chrono::system_clock::to_time_t(sctp); // o "data" é a data em time_t que vai ser depois formatada

        if (entrada.is_directory())
        {
            Directoria *subDirectoria = new Directoria(nome, caminho, data, sistema);
            directoriaPai->adicionarEntrada(subDirectoria);
            LoadAux(subDirectoria, caminho, sistema);
        }
        else
        {
            Ficheiro *ficheiro = new Ficheiro(nome, caminho, (int)fs::file_size(entrada), data, sistema);
            directoriaPai->adicionarEntrada(ficheiro);
        }
    }
}

bool SistemaFicheiros::Load(const string &path)
{
    if (!fs::exists(path))
    {
        cout << "Caminho inválido: " << path << endl;
        return false;
    }

    auto ftime = fs::last_write_time(path);
    auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(ftime - fs::file_time_type::clock::now() + chrono::system_clock::now());
    time_t data = chrono::system_clock::to_time_t(sctp);

    Directoria *DirectoriaInicial = new Directoria(path, path, data, this);
    Entradas.push_back(DirectoriaInicial);

    LoadAux(DirectoriaInicial, path, this);
    return true;
}

int SistemaFicheiros::contarFicheirosAux(Entrada *entrada)
{
    if (!entrada)
	{
		return 0;
	}
    if (!entrada->isDirectoria()) 
	{
	    return 1; // é ficheiro
	}
    int total = 0;
    Directoria *directoria = (Directoria*)entrada; //transformar a entrada num ponteiro de directoria, já que se verificou que é uma directoria
    for (Entrada *e : directoria->getConteudo())
    {
    	total = total + contarFicheirosAux(e);
	}
    return total;
}

int SistemaFicheiros::contarDirectoriasAux(Entrada *entrada)
{
    if (!entrada)
	{
	    return 0;
	} 
    int total = 0;
    if (entrada->isDirectoria())
    {
        total = 1; // conta a diretoria atual
        Directoria *directoria = (Directoria*)entrada;
        for (Entrada *e : directoria->getConteudo())
        {
            total = total + contarDirectoriasAux(e);
        }
    }
    return total;
}

int SistemaFicheiros::memoriaAux(Entrada *entrada)
{
    if (!entrada)
	{
		return 0;
	}
    if (!entrada->isDirectoria())
    {
    	return ((Ficheiro*)entrada)->getTamanho();
	}
    int total = 0;
    Directoria *directoria = (Directoria*)entrada;
    for (Entrada *e : directoria->getConteudo())
    {
    	total = total + memoriaAux(e);
	}
    return total;
}


int SistemaFicheiros::ContarFicheiros()
{
    int total = 0;
    for (Entrada *e : Entradas)
    {
    	total = total + contarFicheirosAux(e);
	}
    return total;
}

int SistemaFicheiros::ContarDirectorias()
{
    int total = 0;
    for (Entrada *e : Entradas)
    {
    	total = total + contarDirectoriasAux(e);
	}
    return total;
}

int SistemaFicheiros::Memoria()
{
    int total = 0;
    for (Entrada *e : Entradas)
    {
    	total = total + memoriaAux(e);
	}
    return total;
}

// Função auxiliar recursiva para determinar a diretoria com mais elementos diretos
Directoria *SistemaFicheiros::DirectoriaMaisElementosAux(Entrada *entrada, int &maxElementos)
{
    if (!entrada || !entrada->isDirectoria())
    {
    	return nullptr;
	}

    Directoria *directoria = (Directoria*)entrada;
    Directoria *maiorDirectoria = directoria;

    // Conta quantos elementos diretos esta diretoria tem (ficheiros + subpastas diretas)
    int elementos = directoria->getConteudo().size();

    // Atualiza se esta diretoria tiver mais elementos que o máximo atual
    if (elementos > maxElementos)
    {
        maxElementos = elementos;
        maiorDirectoria = directoria;
    }

    // Percorre as subdiretorias recursivamente
    for (Entrada *subDirectoria : directoria->getConteudo())
    {
        if (subDirectoria->isDirectoria())
        {
            Directoria *candidata = DirectoriaMaisElementosAux(subDirectoria, maxElementos);
            if (candidata && (int)candidata->getConteudo().size() > (int)maiorDirectoria->getConteudo().size())
            {
            	maiorDirectoria = candidata;
			}
        }
    }

    return maiorDirectoria;
}

string *SistemaFicheiros::DirectoriaMaisElementos()
{
    if (Entradas.empty())
    {
    	cout << "Nenhuma diretoria carregada" << endl;
    	return nullptr;
	}

    int maxElementos = -1;
    Directoria *maiorDirectoria = nullptr;
    Directoria *raiz = (Directoria*)Entradas.front();

    for (Entrada *entrada : raiz->getConteudo())
    {
        if (entrada->isDirectoria())
        {
            Directoria *candidata = DirectoriaMaisElementosAux(entrada, maxElementos);
            if (candidata && (!maiorDirectoria || (int)candidata->getConteudo().size() > (int)maiorDirectoria->getConteudo().size()))
            {
            	maiorDirectoria = candidata;
			}
        }
    }

    if (maiorDirectoria)	
    {
    	return new string(maiorDirectoria->getNome());
	}
    else
    {
    	return nullptr;
	}
}

// Função auxiliar recursiva para determinar a diretoria com menos elementos diretos
Directoria *SistemaFicheiros::DirectoriaMenosElementosAux(Entrada *entrada, int &minElementos)
{
    if (!entrada || !entrada->isDirectoria())
    {
    	return nullptr;
	}

    Directoria *directoria = (Directoria*)entrada;
    Directoria *menorDirectoria = directoria;

    int elementos = directoria->getConteudo().size();


    if (minElementos == -1 || elementos < minElementos)
    {
        minElementos = elementos;
        menorDirectoria = directoria;
    }

    for (Entrada *subDirectoria : directoria->getConteudo())
    {
        if (subDirectoria->isDirectoria())
        {
            Directoria *candidata = DirectoriaMenosElementosAux(subDirectoria, minElementos);
            if (candidata && (int)candidata->getConteudo().size() < (int)menorDirectoria->getConteudo().size())
            {
            	menorDirectoria = candidata;
			}
        }
    }

    return menorDirectoria;
}

string *SistemaFicheiros::DirectoriaMenosElementos()
{
    if (Entradas.empty())
    {
    	cout << "Nenhuma diretoria carregada" << endl;
    	return nullptr;
	}

    int minElementos = 10000000;
    Directoria *menorDirectoria = nullptr;
    Directoria *raiz = (Directoria*)Entradas.front();

    for (Entrada *entrada : raiz->getConteudo())
    {
        if (entrada->isDirectoria())
        {
            Directoria *candidata = DirectoriaMenosElementosAux(entrada, minElementos);
            if (candidata && (!menorDirectoria || (int)candidata->getConteudo().size() < (int)menorDirectoria->getConteudo().size()))
            {
            	menorDirectoria = candidata;
			}
        }
    }

    if (menorDirectoria)	
    {
    	return new string(menorDirectoria->getNome());
	}
    else
    {
        return nullptr;
	}
}

Ficheiro *SistemaFicheiros::FicheiroMaiorAux(Entrada *entrada, Ficheiro *atualMaior)
{
    if (!entrada)
    {
    	return atualMaior;
	}

    if (entrada->isDirectoria())
    {
        Directoria *directoria = (Directoria*)entrada;
        for (Entrada *entrada : directoria->getConteudo())
        {
        	atualMaior = FicheiroMaiorAux(entrada, atualMaior);
		}
    }
    else
    {
        Ficheiro *f = (Ficheiro*)entrada;
        if (!atualMaior || f->getTamanho() > atualMaior->getTamanho())
        {
        	atualMaior = f;
		}
    }
    return atualMaior;
}

string *SistemaFicheiros::FicheiroMaior()
{
    if (Entradas.empty())
    {
    	cout << "Nenhum ficheiro carregado" << endl;
        return nullptr;
	}

    Ficheiro *maiorFicheiro = nullptr;

    for (Entrada *entrada : Entradas)
    {
    	maiorFicheiro = FicheiroMaiorAux(entrada, maiorFicheiro);
	}

    if (maiorFicheiro)
    {
    	return new string(maiorFicheiro->getCaminho());
	}
    else
    {
    	return nullptr;
	}
}


Directoria *SistemaFicheiros::DirectoriaMaisEspacoAux(Entrada *entrada, int &maxEspaco)
{
    if (!entrada || !entrada->isDirectoria())
    {
    	return nullptr;
	}

    Directoria *directoria = (Directoria*)entrada;
    Directoria *maiorDirectoria = directoria;

    int espaco = 0;
    for (Entrada *entrada : directoria->getConteudo())
    {
    	espaco = espaco + memoriaAux(entrada);
	}

    if (espaco > maxEspaco)
    {
        maxEspaco = espaco;
        maiorDirectoria = directoria;
    }
    
    for (Entrada *entrada : directoria->getConteudo())
    {
        if (entrada->isDirectoria())
        {
            Directoria *candidata = DirectoriaMaisEspacoAux(entrada, maxEspaco);
            if (candidata && memoriaAux(candidata) > memoriaAux(maiorDirectoria))
            {
            	maiorDirectoria = candidata;
			}
        }
    }

    return maiorDirectoria;
}

string *SistemaFicheiros::DirectoriaMaisEspaco()
{
    if (Entradas.empty())
    {
    	cout << "Nenhuma diretoria carregada" << endl;
    	return nullptr;
	}

    int maxEspaco = -1;
    Directoria *maiorDirectoria = nullptr;
    Directoria *raiz = (Directoria*)Entradas.front();

    for (Entrada *entrada : raiz->getConteudo())
    {
        if (entrada->isDirectoria())
        {
            Directoria *candidata = DirectoriaMaisEspacoAux(entrada, maxEspaco);
            if (candidata && (!maiorDirectoria || memoriaAux(candidata) > memoriaAux(maiorDirectoria)))
            {
            	maiorDirectoria = candidata;
			}
        }
    }

    if (maiorDirectoria)
    {
    	return new string(maiorDirectoria->getCaminho());
	}
    else
    {
    	cout << "Nenhuma diretoria encontrada" << endl;
    	return nullptr;
	}
}

string *SistemaFicheiros::SearchAux(Entrada *entrada, const string &nome, int Tipo)
{
    if (!entrada)
	{
		return nullptr;
	} 

    if (Tipo == 0 && !entrada->isDirectoria() && entrada->getNome() == nome)
    {
    	return new string(entrada->getCaminho());
	}

    if (Tipo == 1 && entrada->isDirectoria() && entrada->getNome() == nome)
    {
    	return new string(entrada->getCaminho());
	}

    if (entrada->isDirectoria())
    {
        Directoria *directoria = (Directoria*)entrada;
        for (Entrada *subEntrada : directoria->getConteudo())
        {
            string *caminhoEncontrado = SearchAux(subEntrada, nome, Tipo);
            if (caminhoEncontrado)
            {
            	return caminhoEncontrado;
			}
        }
    }
    return nullptr;
}

string *SistemaFicheiros::Search(const string &s, int Tipo)
{
    if (Entradas.empty())
    {
    	cout << "Nenhum dado carregado" << endl;
    	return nullptr;
	}
    for (Entrada *entrada : Entradas)
    {
        string *caminhoEncontrado = SearchAux(entrada, s, Tipo);
        if (caminhoEncontrado)
        {
        	return caminhoEncontrado;
		}
    }
    return nullptr;
}

bool SistemaFicheiros::RemoverAllAux(Directoria *directoria, bool removerDirectorias)
{
    bool removido = false;
    auto &conteudo = directoria->getConteudo();

    for (auto it = conteudo.begin(); it != conteudo.end(); )
    {
        Entrada *entrada = *it; // Obtém cada entrada dentro da diretoria

        if (removerDirectorias && entrada->isDirectoria())
        {
            delete entrada;
            it = conteudo.erase(it);
            removido = true;
        }
        else if (!removerDirectorias && !entrada->isDirectoria())
        {
            delete entrada;
            it = conteudo.erase(it);
            removido = true;
        }
        else
        {
            if (entrada->isDirectoria())
            {
                bool resultadoRecursivo = RemoverAllAux((Directoria*)entrada, removerDirectorias);
                if (resultadoRecursivo)
                {
                    removido = true; 
                }
            }
            ++it;
        }
    }

    return removido;
}

bool SistemaFicheiros::RemoverAll(const string &s, const string &tipo)
{
    if (Entradas.empty())
    {
        return false;
    }

    bool removerDirectorias = (tipo == "DIR");
    bool sucesso = false;

    for (Entrada *entrada : Entradas)
    {
        if (entrada->isDirectoria())
        {
            bool resultado = RemoverAllAux((Directoria*)entrada, removerDirectorias);
            if (resultado)
            {
                sucesso = true; 
            }
        }
    }
    return sucesso;
}

string SistemaFicheiros::formatarData(time_t t)
{
	tm *localTime = localtime(&t);
	stringstream ss;
	ss << put_time(localTime, "%d-%m-%Y %H:%M:%S");
    return ss.str();
}

void SistemaFicheiros::Escrever_XML_Aux(ofstream &f, Directoria *directoria, int nivel)
{
    // espaços de indentação
    for(int i = 0; i < nivel; i++)
    {
    	f << "  ";
	}

    f << "<Directoria nome=\"" << directoria->getNome() << "\" data=\"" << formatarData(directoria->getData()) << "\">\n";

    for (Entrada *entrada : directoria->getConteudo())
    {
        if (entrada->isDirectoria())
        {
            Escrever_XML_Aux(f, (Directoria*)entrada, nivel + 1);
        }
        else
        {
            for(int i = 0; i < nivel + 1; i++)
                f << "  ";

            Ficheiro *ficheiro = (Ficheiro*)entrada;

            f << "<Ficheiro nome=\"" << ficheiro->getNome()<< "\" tamanho=\"" << ficheiro->getTamanho()<< "\" data=\"" << formatarData(ficheiro->getData())<< "\" />\n";
        }
    }

    for(int i = 0; i < nivel; i++)
    {
    	f << "  ";
	}
    f << "</Directoria>\n";
}

void SistemaFicheiros::Escrever_XML(const string &s)
{
    ofstream f(s);
    if (!f.is_open())
    {
        cout << "Erro ao criar ficheiro XML: " << s << endl;
        return;
    }

    f << "<SistemaFicheiros>\n";

    for (Entrada *entrada : Entradas)
    {
        if (entrada->isDirectoria())
        {
        	Escrever_XML_Aux(f, (Directoria*)entrada, 1);
		}
    }
    f << "</SistemaFicheiros>\n";
    f.close();

    cout << "Sistema gravado corretamente em \"" << s << ".xml\"\n";
}

Directoria *SistemaFicheiros::Ler_XML_Aux(ifstream &f, string linha)
{
    // Encontrar posição do nome
    int posNomeInicio = linha.find("nome=\"");
    if (posNomeInicio == -1) 
	{
		return nullptr;
	}
    posNomeInicio = posNomeInicio + 6; // o +6 serve para ignorar o 'nome="'
    int posNomeFim = linha.find("\"", posNomeInicio); // para o nome ir até as " do fim do nome, retirar a parte da data
    if (posNomeFim == -1) 
	{
		return nullptr;
	}

    string nome = linha.substr(posNomeInicio, posNomeFim - posNomeInicio);

    // Encontrar posição da data
    int posDataInicio = linha.find("data=\"");
    if (posDataInicio == -1) 
	{
		return nullptr; 
	}
    posDataInicio = posDataInicio + 6;

    int posDataFim = linha.find("\"", posDataInicio);
    if (posDataFim == -1)
	{
	    return nullptr;
	}

    string dataStr = linha.substr(posDataInicio, posDataFim - posDataInicio);

    // Converter string em time_t
    tm t = {};
    istringstream ss(dataStr);
    ss >> get_time(&t, "%d-%m-%Y %H:%M:%S");
    time_t data = mktime(&t);

    Directoria *novaDirectoria = new Directoria(nome, nome, data, this);

    // Ler linhas até encontrar o "</Directoria>"
    string linhaAtual;
    while (getline(f, linhaAtual))
    {
        if (linhaAtual.find("<Ficheiro") != -1)
        {
            int pInicio = linhaAtual.find("nome=\"");
            int pFim = linhaAtual.find("\"", pInicio + 6);
            int pInicioTam = linhaAtual.find("tamanho=\"");
            int pFimTam = linhaAtual.find("\"", pInicioTam + 9);
            int pInicioData = linhaAtual.find("data=\"");
            int pFimData = linhaAtual.find("\"", pInicioData + 6);

            // Só cria o ficheiro se todos os atributos forem encontrados
            if (pInicio != -1 && pFim != -1 && pInicioTam != -1 && pFimTam != -1 && pInicioData != -1 && pFimData != -1)
            {
                string nomeF = linhaAtual.substr(pInicio + 6, pFim - (pInicio + 6));
                int tamanho = stoi(linhaAtual.substr(pInicioTam + 9, pFimTam - (pInicioTam + 9)));
                string dataF = linhaAtual.substr(pInicioData + 6, pFimData - (pInicioData + 6));

                tm tf = {};
                istringstream sf(dataF);
                sf >> get_time(&tf, "%d-%m-%Y");
                time_t dataFich = mktime(&tf);

                Ficheiro *ficheiro = new Ficheiro(nomeF, nomeF, tamanho, dataFich, this);
                novaDirectoria->adicionarEntrada(ficheiro);
            }
        }
        else if (linhaAtual.find("<Directoria") != -1)
        {
            Directoria *sub = Ler_XML_Aux(f, linhaAtual);
            if (sub) novaDirectoria->adicionarEntrada(sub);
        }
        else if (linhaAtual.find("</Directoria>") != -1)
        {
            break;
        }
    }    

    return novaDirectoria;
}

bool SistemaFicheiros::Ler_XML(const string &s)
{
    ifstream f(s);
    if (!f.is_open())
    {
        cout << "Erro ao abrir ficheiro XML: " << s << endl;
        return false;
    }

    for (Entrada *e : Entradas)
        delete e;
    Entradas.clear();

    string linha;
    getline(f, linha);
    if (linha.find("<SistemaFicheiros>") == -1)
    {
        cout << "Formato XML inválido." << endl;
        f.close();
        return false;
    }

    while (getline(f, linha))
    {
        if (linha.find("<Directoria") != -1)
        {
            Directoria *raiz = Ler_XML_Aux(f, linha);
            if (raiz) Entradas.push_back(raiz);
        }
        else if (linha.find("</SistemaFicheiros>") != -1)
        {
            break;
        }
    }

    f.close();
    return true;
}

Directoria *SistemaFicheiros::ProcurarDirectoria(Entrada* entrada, const string& nome)
{
    if (!entrada) return nullptr;

    if (entrada->isDirectoria() && entrada->getNome() == nome)
        return (Directoria*)entrada;

    if (entrada->isDirectoria())
    {
        Directoria* d = (Directoria*)entrada;
        for (Entrada* e : d->getConteudo())
        {
            Directoria* r = ProcurarDirectoria(e, nome);
            if (r) return r;
        }
    }
    return nullptr;
}

bool SistemaFicheiros::MoveFicheiroAux(Directoria *directoria, const string &Fich, Directoria *DirNova)
{
    for (auto it = directoria->getConteudo().begin(); it != directoria->getConteudo().end(); ++it)
    {
        Entrada *entrada = *it;
        if (!entrada->isDirectoria() && entrada->getNome() == Fich)
        {
            if (entrada->getCaminho() == DirNova->getCaminho()) 
                return false; // já está na diretoria nova
            
            // Remove da diretoria atual e adiciona na nova
            it = directoria->getConteudo().erase(it);
            DirNova->adicionarEntrada(entrada);
            return true;
        }
        else if (entrada->isDirectoria())
        {
            if (MoveFicheiroAux((Directoria*)entrada, Fich, DirNova))
                return true;
        }
    }
    return false;
}

bool SistemaFicheiros::MoveFicheiro(const string &Fich, const string &DirNova)
{
    Directoria* raiz = (Directoria*)Entradas.front();

    Directoria* dirNovaPtr = ProcurarDirectoria(raiz, DirNova);
    if (!dirNovaPtr) return false;

    return MoveFicheiroAux(raiz, Fich, dirNovaPtr);
}

bool SistemaFicheiros::MoverDirectoriaAux(Entrada *atual, Directoria *origem, Directoria *destino)
{
    if (!atual || !atual->isDirectoria()) return false;

    Directoria *dir = (Directoria*)atual;

    for (auto it = dir->getConteudo().begin(); it != dir->getConteudo().end(); ++it)
    {
        Entrada *e = *it;

        // Encontrou a diretoria a mover
        if (e == origem)
        {
            // Remove da diretoria atual
            it = dir->getConteudo().erase(it);

            // Adiciona na diretoria nova
            destino->adicionarEntrada(e);
            return true;
        }

        // Se for diretoria, continua a procurar
        if (e->isDirectoria())
        {
            if (MoverDirectoriaAux(e, origem, destino))
                return true;
        }
    }

    return false;
}


bool SistemaFicheiros::MoverDirectoria(const string &DirOld, const string &DirNew)
{
    if (Entradas.empty()) return false;

    Directoria *raiz = (Directoria*)Entradas.front();

    Directoria *origem = ProcurarDirectoria(raiz, DirOld);
    Directoria *destino = ProcurarDirectoria(raiz, DirNew);

    if (!origem || !destino) 
        return false;

    // Impedir mover para dentro dela própria
    if (origem == destino)
        return false;

    // Impedir mover origem para dentro de um subdiretório dela própria
    Directoria *aux = destino;
    while (aux)
    {
        if (aux == origem)
            return false; // destino é subdiretoria de origem → proibido

        aux = aux->getPai();  
    }

    return MoverDirectoriaAux(raiz, origem, destino);
}


string *SistemaFicheiros::DataFicheiroAux(Entrada *entrada, const string &nome)
{
    if (!entrada) return nullptr;

    if (!entrada->isDirectoria() && entrada->getNome() == nome)
        return new string(entrada->getDataFormatada());

    if (entrada->isDirectoria())
    {
        Directoria *dir = (Directoria*)entrada;
        for (Entrada *e : dir->getConteudo())
        {
            string *res = DataFicheiroAux(e, nome);
            if (res) return res;
        }
    }
    return nullptr;
}

string SistemaFicheiros::DataFicheiro(const string &ficheiro)
{
    for (Entrada *entrada : Entradas)
    {
        string *res = DataFicheiroAux(entrada, ficheiro);
        if (res) return *res;
    }
    return "NULL";
}

void SistemaFicheiros::TreeAux(Directoria *dir, ostream &out, int nivel)
{
    for (int i = 0; i < nivel; ++i) out << "  ";
    out << "[DIR] " << dir->getNome() << endl;

    for (Entrada *e : dir->getConteudo())
    {
        if (e->isDirectoria())
            TreeAux((Directoria*)e, out, nivel+1);
        else
        {
            for (int i = 0; i <= nivel; ++i) out << "  ";
            out << "- " << e->getNome() << endl;
        }
    }
}

void SistemaFicheiros::Tree(const string *fich)
{
    if (fich)
    {
        ofstream out(*fich);
        if (!out.is_open()) return;
        for (Entrada *e : Entradas)
            if (e->isDirectoria())
                TreeAux((Directoria*)e, out, 0);
    }
    else
    {
        for (Entrada *e : Entradas)
            if (e->isDirectoria())
                TreeAux((Directoria*)e, cout, 0);
    }
}

void SistemaFicheiros::PesquisarAllDirectoriasAux(Entrada *entrada, const string &nome, list<string> &res)
{
    if (!entrada) return;
    if (entrada->isDirectoria() && entrada->getNome() == nome)
        res.push_back(entrada->getCaminho());

    if (entrada->isDirectoria())
    {
        Directoria *dir = (Directoria*)entrada;
        for (Entrada *e : dir->getConteudo())
            PesquisarAllDirectoriasAux(e, nome, res);
    }
}

void SistemaFicheiros::PesquisarAllDirectorias(list<string> &lres, const string &dir)
{
    for (Entrada *e : Entradas)
        PesquisarAllDirectoriasAux(e, dir, lres);
}

void SistemaFicheiros::PesquisarAllFicheirosAux(Entrada *entrada, const string &nome, list<string> &res)
{
    if (!entrada) return;
    if (!entrada->isDirectoria() && entrada->getNome() == nome)
        res.push_back(entrada->getCaminho());

    if (entrada->isDirectoria())
    {
        Directoria *dir = (Directoria*)entrada;
        for (Entrada *e : dir->getConteudo())
            PesquisarAllFicheirosAux(e, nome, res);
    }
}

void SistemaFicheiros::PesquisarAllFicheiros(list<string> &lres, const string &file)
{
    for (Entrada *e : Entradas)
        PesquisarAllFicheirosAux(e, file, lres);
}

void SistemaFicheiros::RenomearFicheirosAux(Entrada *entrada, const string &fich_old, const string &fich_new)
{
    if (!entrada) return;
    if (!entrada->isDirectoria() && entrada->getNome() == fich_old)
        entrada->setNome(fich_new);

    if (entrada->isDirectoria())
    {
        Directoria *dir = (Directoria*)entrada;
        for (Entrada *e : dir->getConteudo())
            RenomearFicheirosAux(e, fich_old, fich_new);
    }
}

void SistemaFicheiros::RenomearFicheiros(const string &fich_old, const string &fich_new)
{
    for (Entrada *e : Entradas)
        RenomearFicheirosAux(e, fich_old, fich_new);
}