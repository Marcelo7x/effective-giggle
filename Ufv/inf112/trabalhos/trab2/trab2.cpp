#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
using namespace std;

char *converte(string str)
{ //converte uma string para um vertor de char alocado dinamicamente
    char *c = new char[str.length() + 1];
    strcpy(c, str.c_str());
    return c;
}

struct Dispositivos //struct para facilitar a ordenacao dos registros
{
    char *chave;
    char *valor;
};

void posicaoChaves(ifstream &entrada, char **argv, int &cont1, int &cont2)
{ //encontra a posicao das chaves a serem utilizadas
    //para a aordenacao

    string str; //string auxiliar
    getline(entrada, str);
    char *colunas = converte(str); //coverte a string auxiliar par aum vetor de char
    char *temp = new char[strlen(colunas)+1]; //vertor de char auxiliar
    bool achou1 = false, achou2 = false;    //indica se encontrou as chaves
    int i = 0,j = 0;
    for (i = 0, j = 0; colunas[i] != '\0'; i++)
    {
        temp[j] = colunas[i];
        temp[j + 1] = '\0';
        j++;

        if (strcmp(temp, argv[3]) == 0) //verica se encontrou a primeira chave
            achou1 = true;

        if (strcmp(temp, argv[4]) == 0) //verifica se encotrou a segunda chave
            achou2 = true;

        if ((achou1 == true) && (achou2 == true))
            break;

        if (colunas[i + 1] == ',')
        {
            i++;
            j = 0;

            if (achou1 == false)
                cont1++;
            if (achou2 == false)
                cont2++;
        }
    }
    if(cont2<cont1){
        int aux = cont2;
        cont2 = cont1;
        cont1 = aux;
    }

    delete[] colunas;
    delete[] temp;
}

Dispositivos leituraChave(ifstream &entrada, int cont1, int cont2, int igno)
{                       //le uma linha d arquivo de entrada e devolve
    Dispositivos disp1; //os regitros das chaves desejadas em struct
    string str;         //string e ponteios para char nescessarios
    char *temp, *aux;
    int pos1 = 0, pos2 = 0, j = 0;

    for (int i = 0; i < igno; i++) //ignora linhas ja lidas
        getline(entrada, str);

    getline(entrada, str); // le a linha desejadas e converte

    temp = converte(str);
   
    disp1.chave = new char[strlen(temp)];
    disp1.valor = new char[strlen(temp)];

    for (int i = 0; temp[i] != '\0'; i++)
    {
        if (temp[i] == ',')
        { //procura a posicao da primeira chave
            pos1++;
            i++;
        }
        if (pos1 > cont1)
        {
            break;
        }
        if ((cont1 == pos1) && (temp[i] != '\n'))
        { //apos encontrar a posicao da chave copia o registro pra um struct
            disp1.chave[j] = temp[i];
            disp1.chave[j + 1] = '\0';
            j++;
        }
    }


    j = 0;

    for (int i = 0; temp[i] != '\0'; i++)
    {
        if (temp[i] == ',')
        { //procura a posicao da primeira chave
            pos2++;
            i++;
        }
        if (pos2 > cont2)
        {
            break;
        }
        if ((cont2 == pos2) && (temp[i] != '\n'))
        { //apos encontrar a posicao da chave copia o registro pra um struct
            disp1.valor[j] = temp[i];
            disp1.valor[j + 1] = '\0';
            j++;
        }
    }

    delete[] temp;
    return disp1;
}

int main(int argc, char **argv){

    ifstream entrada(argv[1]); //abre o arquivo de entrada base
    if (!entrada.is_open())
    { //verifica se o arquivo foi aberto
        cerr << " Erro ao abrir arquivo de entrada.\n";
        exit(1);
    }

    int cont1 = 0, cont2 = 0;        // contadores para guardarem as posicoes das chaves usadas para a ordenaçao
    int cap_memoria = atoi(argv[2]); //capacidade de leitura

    string str; //string auxiliar para leitura de uma linha de arquivo e logo apos onverter em um vertor char
    //char colunas[n];

    posicaoChaves(entrada, argv, cont1, cont2); //funcoes para encotrar as posicoes das colunas onde estao as chaves

    int k = 0;
    bool passou = false;
    Dispositivos *aux = new Dispositivos[90];

    for(int i=0;i<90;i++){
        aux[i] = leituraChave(entrada,cont1,cont2,0);
        cout << i << " " << aux[i].chave << " " << aux[i].valor << endl;
    }

    cout << cont1 << endl;
    cout << cont2 << endl;
    //int cont_disp = part_ordena(entrada, cap_memoria, cont1, cont2); //faz leitura das linhas do arquivo base e guarda
                                                                     // em n/m ou n/m+1 arquivos auxiliares

    for(int i=0;i<90;i++){
        delete[] aux[i].chave;
        delete[] aux[i].valor;
    }

    delete[] aux;

    return 0;
}
