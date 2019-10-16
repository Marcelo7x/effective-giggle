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
    char chave[100];
    int valor;
};

void imprimeChaves(/*char **argv,*/ Dispositivos *disp1, int linhas)
{
    //cout << argv[3] << " " << argv[4] << endl;
    for (int i = 0; i < linhas; i++)
        cout << disp1[i].chave << " " << disp1[i].valor << endl;
}

void posicaoChaves(ifstream &entrada, char **argv, int &cont1, int &cont2)
{ //encontra a posicao das chaves a serem utilizadas
    //para a aordenacao

    string str; //string auxiliar
    getline(entrada, str);
    char *colunas = converte(str); //coverte a string auxiliar par aum vetor de char

    char *temp = new char[sizeof(colunas)]; //vertor de char auxiliar
    bool achou1 = false, achou2 = false;    //indica se encontrou as chaves

    for (int i = 0, j = 0; colunas[i] != '\0'; i++)
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
    cout << "get = " << str << endl;
    temp = converte(str);

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
    aux = new char[sizeof(temp)];

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
            aux[j] = temp[i];
            aux[j + 1] = '\0';
            j++;
        }
    }

    disp1.valor = atoi(aux);

    delete[] aux;
    return disp1;
}

int part_ordena(ifstream &entrada, int cap_memoria, int cont1, int cont2)
{ //le registros, ordena-os e grava em um dispositivo auxiliar
    char disp[20];
    Dispositivos *disp1 = new Dispositivos[cap_memoria], disp_temp;
    int cont_disp = 0, cont, j;
    bool passou = false;

    while (true)
    {
        if (entrada.eof())
            break;

        sprintf(disp, "disp%d.txt", cont_disp); //abre arquivo auxiliar para a gravacao
        fstream saida(disp, std::fstream::in | std::fstream::out | std::fstream::app);
        if (!saida.is_open())
        { //verifica se arquivo foi aberto
            cerr << " Erro ao abrir arquivo de fstream.\n";
            exit(1);
        }

        cont = 0;

        for (int i = 0; i < cap_memoria; i++)
        {
            if (entrada.eof())
            {
                passou = true;
                break;
            }

            disp1[i] = leituraChave(entrada, cont1, cont2, 0); //extrai registros
            cont++;

            if (i > 0)
            {
                j = i;
                while (j > 0)
                {

                    if (strcmp(disp1[j].chave, disp1[j - 1].chave) < 0) //ordena os registros
                    {
                        disp_temp = disp1[j - 1];
                        disp1[j - 1] = disp1[j];
                        disp1[j] = disp_temp;
                        j--;
                    }
                    else
                        break;
                }
            }
        }

        for (int i = 0; i < cont; i++)
        {
            if (i != cont - 1)
            {
                //grava os registros ordenados nos arquivos
                saida << disp1[i].chave << "," << disp1[i].valor << endl;
            }
            else
            {
                saida << disp1[i].chave << "," << disp1[i].valor;
            }
        }
        cont_disp++;
        saida.close(); //fecha os arquivos
    }

    delete[] disp1;
    return cont_disp;
}

void intercala(int cont_dispositivos, int capac_memoria, int cont1, int cont2)
{
    Dispositivos disps[cont_dispositivos];
    char disp_aux[20];
    int cont = 0;
    int contDisp[cont_dispositivos] = {0};
    bool acabouDisp[cont_dispositivos] = {false};
    bool acabou = false;
    int quantDisp = 0, a;

    for (int i = 0; i < cont_dispositivos; i++)
        acabouDisp[i] = false;
    

    for (int i = 0; i < cont_dispositivos; i++)
    {

        sprintf(disp_aux, "disp%d.txt", i);

        ifstream arquivo(disp_aux);
        if (!arquivo.is_open())
        {
            cerr << "Nao abriu o arquivo\n";
            exit(1);
        }

        disps[i] = leituraChave(arquivo, 0, 1, 0);
        cout << "disp " << disps[i].chave << " "<< disps[i].valor << endl;
        arquivo.close();
        contDisp[i]++;
    }

    Dispositivos menor;
    int pos = 0, posaux, numd = 0;


   while (true)
   {
       for (int i = 0; i < cont_dispositivos; i++)
           if (acabouDisp[i] == false)
           {
               menor = disps[i];
               break;
           }
        cout << menor.chave << " " << menor.valor; 
        

        for (int i = 0; i < cont_dispositivos; i++)
        {
            if (!acabouDisp[i])
            {
                if (strcmp(disps[i].chave, menor.chave) < 0)
                {
                    cout << " knncec\n";
                    menor = disps[i];
                    pos = i;
                    numd = i;
                }
            }
        }
        cout << "";
        sprintf(disp_aux, "disp%d.txt", cont_dispositivos);

        fstream arquivo(disp_aux, std::fstream::out | std::fstream::app);
        if (!arquivo.is_open())
        {
            cerr << "Nao abriu o arquivo final\n";
            exit(1);
        }

        arquivo << menor.chave << "," << menor.valor << endl;
        cout << menor.chave << " " << menor.valor << "menoor\n";
        arquivo.close();

        
        cout << pos << " " << numd <<" pos\n";
        if (!acabouDisp[numd])
        {
            sprintf(disp_aux, "disp%d.txt", numd);
            ifstream arquivo2(disp_aux);
            if (!arquivo2.is_open()){
                cerr << " Erro ao abrir arquivo de ifstream.\n";
                exit(1);
            }
            
            disps[pos] = leituraChave(arquivo2, 0, 1, contDisp[numd]);
            contDisp[numd]++;
        
            if (arquivo2.eof())
                acabouDisp[numd] == true;

            arquivo2.close();
        }
        else
        {
            for (int i = 0; i < cont_dispositivos; i++)
            {
                if (!acabouDisp[i]){
                    numd = i;
                    break;
                }
            }
             sprintf(disp_aux, "disp%d.txt", numd);
            ifstream arquivo2(disp_aux);
            if (!arquivo2.is_open()){
                cerr << " Erro ao abrir arquivo de ifstream.\n";
                exit(1);
            }
            
            disps[pos] = leituraChave(arquivo2, 0, 1, contDisp[numd]);
            contDisp[numd]++;
        
            if (arquivo2.eof())
                acabouDisp[numd] == true;

            arquivo.close();
        }
        
        acabou = true;
        for (int i = 0; i < cont_dispositivos; i++)
        {
            if (!acabouDisp[i])
                acabou == false; 
        }
   }
   
    
}
int main(int argc, char **argv)
{

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
    int cont_disp = part_ordena(entrada, cap_memoria, cont1, cont2); //faz leitura das linhas do arquivo base e guarda
                                                                     // em n/m ou n/m+1 arquivos auxiliares

    intercala(cont_disp,cap_memoria,cont1,cont2);

    entrada.close();

    return 0;
}
