#include <iostream>
#include <list>
#include <string>
#include "SistemaFicheiros.h"
#include "Entradas.h"
#include "Directorias.h"
#include "Ficheiros.h"
#include <locale>

using namespace std;

int main() 
{
	setlocale(LC_ALL, "Portuguese");
    SistemaFicheiros sf;
    string caminho;

    cout << "Digite o caminho da diretoria a carregar: ";
    getline(cin, caminho);

    if (!sf.Load(caminho))
    {
        cout << "Erro ao carregar a diretoria '" << caminho << "'." << endl;
        return 1;
    }

    cout << "\nDiretoria '" << caminho << "' carregada com sucesso!\n";

    int opcao;
    do 
    {
        cout << "\n========= MENU SISTEMA DE FICHEIROS =========\n";
        cout << "1. Mostrar conteudo (Tree)\n";
        cout << "2. Contar ficheiros\n";
        cout << "3. Contar diretorias\n";
        cout << "4. Memoria total\n";
        cout << "5. Diretoria com mais elementos\n";
        cout << "6. Diretoria com menos elementos\n";
        cout << "7. Ficheiro maior\n";
        cout << "8. Diretoria que ocupa mais espaco\n";
        cout << "9. Pesquisar ficheiro/diretoria\n";
        cout << "10. Remover todas diretorias/ficheiros\n";
        cout << "11. Gravar para XML\n";
        cout << "12. Ler de XML\n";
        cout << "13. Mover ficheiro\n";
        cout << "14. Mover diretoria\n";
        cout << "15. Data de um ficheiro\n";
        cout << "16. Pesquisar todas diretorias com nome\n";
        cout << "17. Pesquisar todos ficheiros com nome\n";
        cout << "18. Renomear ficheiros\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        cin.ignore(); // limpar buffer

        switch(opcao)
        {
            case 1:
                cout << "Tree do sistema de ficheiros:\n";
                sf.Tree(); // sem argumento mostra na consola
                break;
            case 2:
                cout << "Numero de ficheiros: " << sf.ContarFicheiros() << endl;
                break;
            case 3:
                cout << "Numero de diretorias: " << sf.ContarDirectorias() << endl;
                break;
            case 4:
                cout << "Memoria total ocupada: " << sf.Memoria() << " bytes" << endl;
                break;
            case 5:
            {
                string* dir = sf.DirectoriaMaisElementos();
                if(dir != nullptr)
                {
                    cout << "Diretoria com mais elementos: " << *dir << endl;
                    delete dir;
                }
                else
                {
                    cout << "Nenhuma diretoria encontrada." << endl;
                }
                break;
            }
            case 6:
            {
                string* dir = sf.DirectoriaMenosElementos();
                if(dir != nullptr)
                {
                    cout << "Diretoria com menos elementos: " << *dir << endl;
                    delete dir;
                }
                else
                {
                    cout << "Nenhuma diretoria encontrada." << endl;
                }
                break;
            }
            case 7:
            {
                string* f = sf.FicheiroMaior();
                if(f != nullptr)
                {
                    cout << "Ficheiro maior: " << *f << endl;
                    delete f;
                }
                else
                {
                    cout << "Nenhum ficheiro encontrado." << endl;
                }
                break;
            }
            case 8:
            {
                string* dir = sf.DirectoriaMaisEspaco();
                if(dir != nullptr)
                {
                    cout << "Diretoria que ocupa mais espaco: " << *dir << endl;
                    delete dir;
                }
                else
                {
                    cout << "Nenhuma diretoria encontrada." << endl;
                }
                break;
            }
            case 9:
            {
                string nome;
                int tipo;
                cout << "Digite o nome a pesquisar: ";
                getline(cin, nome);
                cout << "Tipo (0 = ficheiro, 1 = diretoria): ";
                cin >> tipo;
                cin.ignore();
                string* res = sf.Search(nome, tipo);
                if(res != nullptr)
                {
                    cout << "Encontrado: " << *res << endl;
                    delete res;
                }
                else
                {
                	cout << "Nenhuma entrada encontrada" << endl;
				}
                break;
            }
            case 10:
            {
                string tipo;
                cout << "Tipo a remover (DIR ou FICH): ";
                getline(cin, tipo);
                bool res = sf.RemoverAll("", tipo);
                if(res)
                {
                    cout << "Remocao efetuada." << endl;
                }
                else
                {
                    cout << "Nada removido." << endl;
                }
                break;
            }
            case 11:
            {
                string arquivo;
                cout << "Nome do ficheiro XML para gravar: ";
                getline(cin, arquivo);
                sf.Escrever_XML(arquivo);
                break;
            }
            case 12:
            {
                string arquivo;
                cout << "Nome do ficheiro XML para ler: ";
                getline(cin, arquivo);
                bool res = sf.Ler_XML(arquivo);
                if(res)
                {
                    cout << "XML lido com sucesso." << endl;
                }
                else
                {
                    cout << "Erro ao ler XML." << endl;
                }
                break;
            }
            case 13:
            {
                string fich, dirNova;
                cout << "Nome do ficheiro a mover: ";
                getline(cin, fich);
                cout << "Diretoria destino: ";
                getline(cin, dirNova);
                bool res = sf.MoveFicheiro(fich, dirNova);
                if(res)
                {
                    cout << "Ficheiro movido com sucesso." << endl;
                }
                else
                {
                    cout << "Erro ao mover ficheiro." << endl;
                }
                break;
            }
            case 14:
            {
                string dirOld, dirNew;
                cout << "Nome da diretoria a mover: ";
                getline(cin, dirOld);
                cout << "Nova diretoria: ";
                getline(cin, dirNew);
                bool res = sf.MoverDirectoria(dirOld, dirNew);
                if(res)
                {
                    cout << "Diretoria movida com sucesso." << endl;
                }
                else
                {
                    cout << "Erro ao mover diretoria." << endl;
                }
                break;
            }
            case 15:
            {
                string fich;
                cout << "Nome do ficheiro: ";
                getline(cin, fich);
                string res = sf.DataFicheiro(fich);
                if(res != "NULL")
                {
                    cout << "Data do ficheiro: " << res << endl;
                }
                else
                {
                    cout << "Ficheiro nao encontrado." << endl;
                }
                break;
            }
            case 16:
            {
                string nome;
                cout << "Nome da diretoria a pesquisar: ";
                getline(cin, nome);
                list<string> res;
                sf.PesquisarAllDirectorias(res, nome);
                if(res.empty())
                {
                    cout << "Nenhuma diretoria encontrada." << endl;
                }
                else
                {
                    cout << "Resultados encontrados:\n";
                    for(auto &r : res) cout << r << endl;
                }
                break;
            }
            case 17:
            {
                string nome;
                cout << "Nome do ficheiro a pesquisar: ";
                getline(cin, nome);
                list<string> res;
                sf.PesquisarAllFicheiros(res, nome);
                if(res.empty())
                {
                    cout << "Nenhum ficheiro encontrado." << endl;
                }
                else
                {
                    cout << "Resultados encontrados:\n";
                    for(auto &r : res) cout << r << endl;
                }
                break;
            }
            case 18:
            {
                string oldName, newName;
                cout << "Nome antigo do ficheiro: ";
                getline(cin, oldName);
                cout << "Novo nome do ficheiro: ";
                getline(cin, newName);
                sf.RenomearFicheiros(oldName, newName);
                cout << "Renomeacao concluida." << endl;
                break;
            }
            case 0:
                cout << "Saindo..." << endl;
                break;
            default:
                cout << "Opcao invalida!" << endl;
        }

    } while(opcao != 0);

    return 0;
}
