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
    double valor;
};

void imprimeChaves(/*char **argv,*/ Dispositivos *disp1, int linhas)
{
    //cout << argv[3] << " " << argv[4] << endl;
    //for (int i = 0; i < linhas; i++)
        //cout << disp1[i].chave << " " << disp1[i].valor << endl;
}

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
    aux = new char[strlen(temp)];

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

    disp1.valor = atof(aux);

    delete[] temp;
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
            cout << "aloc1\n";

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
                        delete disp_temp.chave;
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
        for(int i = 0; i < cap_memoria ;i++){
        delete[] disp1[i].chave;
        cout << "desaloc1\n";
    }
    }

   
    delete[] disp1;
    
    return cont_disp;
}

void intercala(int cont_dispositivos, int capac_memoria)
{
    Dispositivos *disps = new Dispositivos[cont_dispositivos];
    char disp_aux[20];
    int cont = 0, a = 0;
    int *contDisp = new int[cont_dispositivos];
    bool *acabouDisp = new bool[cont_dispositivos], *podeAcabar = new bool[cont_dispositivos];
    bool acabou = false;
    bool *desalocouChave = new bool[cont_dispositivos];
    bool desalocouMenor = false;

    for (int i = 0; i < cont_dispositivos; i++)
    {
        acabouDisp[i] = false;
        podeAcabar[i] = false;
        contDisp[i] = 0;
        desalocouChave[i] = false;
    }

    for (int i = 0; i < cont_dispositivos; i++)
    {
        if (!acabouDisp[i])
        {

            sprintf(disp_aux, "disp%d.txt", i);

            ifstream arquivo(disp_aux);
            if (!arquivo.is_open())
            {
                cerr << "Nao abriu o arquivo\n";
                exit(1);
            }

            disps[i] = leituraChave(arquivo, 0, 1, 0);
            cout << "aloc2\n";
            //cout << "disp " << disps[i].chave << " " << disps[i].valor << endl;

            arquivo.close();
            contDisp[i]++;
        }
    }

    Dispositivos menor;
    menor.chave = new char[sizeof(disps[0].chave)*3];
    int pos = 0;

    while (!acabou)
    {
        //cout << "entrou while\n";
        for (int i = 0; i < cont_dispositivos; i++)
        {
            //cout << "primeiro for\n";
            if (!acabouDisp[i])
            {
                menor.valor = disps[i].valor;
                menor.chave = disps[i].chave;

                break;
            }
        }
        //cout << menor.chave << "," << menor.valor << endl;
        for (int i = 0; i < cont_dispositivos; i++)
        {
            //cout << "segundo for\n";
            if (!acabouDisp[i])
            {
                if (strcmp(disps[i].chave, menor.chave) <= 0)
                {
                    menor.chave = disps[i].chave;
                    menor.valor = disps[i].valor;
                    pos = i;
                }
            }
        }

        for (int i = 0; i < cont_dispositivos; i++)
            //cout << "dispositivo " << i << " " << disps[i].chave << ", " << disps[i].valor << endl;
            //cout << menor.chave << "," << menor.valor << endl;
            //cout << "saiu segundo for\n";

        sprintf(disp_aux, "disp%d.txt", cont_dispositivos);

        fstream arquivo(disp_aux, std::fstream::out | std::fstream::app);
        if (!arquivo.is_open())
        {
            cerr << "Nao abriu o arquivo final\n";
            exit(1);
        }

        arquivo << menor.chave << "," << menor.valor << endl;
        //cout << " menor final " << menor.chave << "," << menor.valor << endl;
        arquivo.close();
        
        delete[] disps[pos].chave;
        cout << "desaloc2\n";
        desalocouChave[pos] = true;

        if (podeAcabar[pos])
            acabouDisp[pos] = true;
        
        

        //cout << "contDisp[pos] = " << contDisp[pos];

        if (!acabouDisp[pos])
        {
            sprintf(disp_aux, "disp%d.txt", pos);

            ifstream arquivo(disp_aux);
            if (!arquivo.is_open())
            {
                cerr << "Nao abriu o arquivo\n";
                exit(1);
            }

            if (arquivo.eof())
            {
                acabouDisp[pos] = true;
            }
            else
            {

                //cout << " dispositivo " << pos << endl;
                //cout << "contador disp " << pos << " " << contDisp[pos] << endl;
                disps[pos] = leituraChave(arquivo, 0, 1, contDisp[pos]);
                cout << "aloc3\n";
                if (arquivo.eof())
                {
                    podeAcabar[pos] = true;
                }

                desalocouChave[pos] = false;
                contDisp[pos]++;
            }
            arquivo.close();
        }

        acabou = true;
        for (int i = 0; i < cont_dispositivos; i++)
            if (!acabouDisp[i])
            {
                acabou = false;
                //cout << " nao acabou disp " << i << endl;
                break;
            }
        
        //if(!desalocouMenor)
            //delete[] menor.chave;
    }
    
    for(int i = 0; i < cont_dispositivos ;i++)
        if(!desalocouChave[i]){
            delete[] disps[i].chave;
            cout << "desaloc3\n";
        }
    
    delete[] disps;
    delete[] acabouDisp;
    delete[] podeAcabar;
    delete[] contDisp;
    delete[] desalocouChave;
    delete[] menor.chave;
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

    intercala(cont_disp, cap_memoria);

    entrada.close();

    return 0;
}