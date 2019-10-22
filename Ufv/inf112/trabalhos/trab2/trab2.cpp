#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <iomanip>
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

void Quick(Dispositivos *disps, int inicio, int fim){ //metodo de ordenacao para struct de vetor char
   
   Dispositivos pivo, aux; //auxiliares
   int i, j, meio;
   
   i = inicio;
   j = fim;
   
   meio = (int) ((i + j) / 2);
   pivo = disps[meio];  // o pivo eh o do meio
   
   do{
       
        while (strcmp(disps[i].chave, pivo.chave) < 0) i = i + 1;
        while (strcmp(disps[j].chave, pivo.chave) > 0) j = j - 1; 
        if(i <= j)
        {
            aux = disps[i];
            disps[i] = disps[j];
            disps[j] = aux;
            i = i + 1;
            j = j - 1;
        }
    }while(j > i);
   
   if(inicio < j) Quick(disps, inicio, j);
   if(i < fim) Quick(disps, i, fim);   

}

void posicaoChaves(ifstream &entrada, char **argv, int &cont1, int &cont2)
{ //encontra a posicao das chaves a serem utilizadas
    //para a aordenacao

    string str; //string auxiliar
    getline(entrada, str);  // faz a leitura de uma linha do arquivo base e grava em uma string auxiliar
    char *colunas = converte(str); //coverte a string auxiliar par aum vetor de char
    char *temp = new char[strlen(colunas)+1]; //vertor de char auxiliar
    *temp = '\0';
    bool achou1 = false, achou2 = false;    //indica se encontrou as chaves
    int i = 0,j = 0;
    for (i = 0, j = 0; colunas[i] != '\0'; i++)
    {
            if(isalnum(colunas[i]) || colunas[i] == '.'){ // se for um caracter,numero ou ponto e grava no vetor auxiliar
                temp[j] = colunas[i];
                temp[j + 1] = '\0';
                j++;
            }
            
            if (strcmp(temp, argv[3]) == 0) //verica se encontrou a primeira chave
                achou1 = true;

            if (strcmp(temp, argv[4]) == 0) //verifica se encotrou a segunda chave
                achou2 = true;

            if ((achou1 == true) && (achou2 == true))   //se encontrar as duas chaves o laco de repeticao acaba
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

Dispositivos leituraChave(ifstream &entrada, int cont1, int cont2)
{                       //le uma linha d arquivo de entrada e devolve
    Dispositivos disps; //os regitros das chaves desejadas em struct
    string str;         //string e ponteios para char nescessarios
    char *temp, *aux;
    int pos1 = 0, pos2 = 0, j = 0;
    bool acabou = false;    
   
    getline(entrada, str); // le a linha desejadas e converte

    temp = converte(str);
    disps.chave = new char[strlen(temp)+1];
    disps.valor = new char[strlen(temp)+1];

    for (int i = 0; temp[i] != '\0'; i++)
    {
        if (temp[i] == ',')
        { //procura a posicao da primeira chave
            pos1++;
            i++;
        }
        
        if (pos1 > cont1)
            break;

        if ((cont1 == pos1) && (temp[i] != '\n') && (temp[i] != '\"'))
        { //apos encontrar a posicao da chave copia o registro pra um struct
            disps.chave[j] = temp[i];
            disps.chave[j + 1] = '\0';
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
        if ((cont2 == pos2) && (temp[i] != '\n') && (temp[i] != '\"'))
        { //apos encontrar a posicao da chave copia o registro pra um struct
            disps.valor[j] = temp[i];
            disps.valor[j + 1] = '\0';
            j++;
        }
    }
    
    //cout << "leu " << disps.chave << ' ' << disps.valor << " ";

    delete[] temp;
    return disps;
}

int part_ordena(ifstream &entrada, int cap_memoria, int cont1, int cont2)
{ //le registros, ordena-os e grava em arquivos auxiliares
    char disp[20];
    Dispositivos *disps = new Dispositivos[cap_memoria];
    int contArquivos = 0, cont, j;

    while (true)
    {
        if (entrada.peek() == -1)
            break;

        sprintf(disp, "arquivo%d.txt", contArquivos); //abre arquivo auxiliar para a gravacao
        fstream saida(disp, std::fstream::out | std::fstream::app);
        
        if (!saida.is_open())
        { //verifica se arquivo foi aberto
            cerr << " Erro ao abrir arquivo de fstream.\n";
            exit(1);
        }
        cont = 0;

        for (int i = 0; i < cap_memoria; i++)
        {
            if (entrada.peek() == -1)
                break;
            

            disps[i] = leituraChave(entrada, cont1, cont2); //extrai registros
            cont++;
        }

        Quick(disps, 0, cont-1);    // ordena as os dispositivos de arcodo com a chave

        for (int i = 0; i < cont; i++)
        {
            if (i != cont - 1)
                if (isalnum(disps[i].chave[0]))//grava os registros ordenados nos arquivos
                    saida << disps[i].chave << "," << disps[i].valor << endl;
            else
                saida << disps[i].chave << "," << disps[i].valor;
            
            delete[] disps[i].chave;
            delete[] disps[i].valor;
        }
        contArquivos++;
        saida.close(); //fecha os arquivos
    }
   
    delete[] disps;
    
    return contArquivos;   // retorna o numeto de arquivos criados
}

void intercala(int quantArquivos, int capac_memoria)
{
    Dispositivos *disps = new Dispositivos[quantArquivos], menor;
    long double resultFinal = 0;
    char nomeArquivo[20], aux[100];
    int cont = 0,contFinal = 0, pos = 0;
    int *contDisp = new int[quantArquivos];
    bool *acabouDisp = new bool[quantArquivos], *podeAcabar = new bool[quantArquivos];
    bool acabou = false;

    for (int i = 0; i < quantArquivos; i++)
    {
        acabouDisp[i] = false;
        podeAcabar[i] = false;
        contDisp[i] = 0;
    }
    
    ifstream *arquivo = new ifstream[quantArquivos]; //cria um vetor para arquivo para leitura
    for (int j = 0; j < quantArquivos; j++) // 
    {
        sprintf(nomeArquivo, "arquivo%d.txt", j);   //abre todos os arquivos auxiliares
        arquivo[j].open(nomeArquivo);
        if (!arquivo[j].is_open())  // verifica se abriu
        {
            cerr << "Nao abriu o arquivo\n";
            exit(1);
        }
    }

    for (int i = 0; i < quantArquivos; i++)
    {
        if (!acabouDisp[i])
        {
            disps[i] = leituraChave(arquivo[i], 0, 1);  //faz a leitura do primeiro de cada arquivo auxiliar
            contDisp[i]++;
        }
    }
    int i = 0;
    while (!acabou)
    {
         for (int i = 0; i < quantArquivos; i++) // coloca o primeiro dispositivo como o menor, se o primeiro arquivo acabar
        {                                        // que corresponde ao primeiro arquivo o menor passa ser o proximo, assim em diante
            if (!acabouDisp[i])
            {
                menor.valor = disps[i].valor;
                menor.chave = disps[i].chave;
                break;
            }
        }
        for (int i = 0; i < quantArquivos; i++) //verifica qual dispositivo realmente eh o menor
        {
            if (!acabouDisp[i])
            {
                if (strcmp(disps[i].chave, menor.chave) <= 0)
                {
                    menor.chave = disps[i].chave;
                    menor.valor = disps[i].valor;
                    pos = i;    //grava a posiçao do dispositivo que contem a menor chave
                }
            }
        }
        //imprime a media de acordo com a chave, usando a posicao do menor disositivo encontrado
        if (resultFinal == 0){ //a primeira vez imprime a primeira chave
            cout << disps[pos].chave << ", ";
            resultFinal = stod(disps[pos].valor);   //grava o primero valor para calcular a media
            strcpy(aux, disps[pos].chave);  //grava a primeira chave em um aux
            contFinal = 1;
        } 

        else if (strcmp(disps[pos].chave, aux) == 0)
        {
            resultFinal += stod(disps[pos].valor);  //soma o valor das proximas chaves que sao iguais
            contFinal++;
        }

        else if (strcmp(disps[pos].chave, aux) > 0)
        {   //se a chave muda.. imprime a media
            cout << fixed << setprecision(30) << resultFinal/contFinal << endl;
            cout << disps[pos].chave << ", ";   //imprime a chave atulizada
            resultFinal = stod(disps[pos].valor);   //grava o primero valor da chave atualizada para calcular a media
            strcpy(aux, disps[pos].chave);  //copiar a chave para um auxiliar
            contFinal = 1;
        }
        
        delete[] disps[pos].chave;
        delete[] disps[pos].valor;

        if (podeAcabar[pos]){
            acabouDisp[pos] = true;
        } 

        if (!acabouDisp[pos])   //verifica se o arquivo acabou
        {
            if (arquivo[pos].peek() == -1)
            {
                acabouDisp[pos] = true; //quando acaba nao eh mais ultilizado
                i++;
            }

            else    //se nao acabou
            {
                disps[pos] = leituraChave(arquivo[pos], 0, 1);  //le a proxima linha do arquivo foi encontrada a menor chave
                if (arquivo[pos].peek() == 0)  //verifica se acabou 
                    podeAcabar[pos] = true;
                
                contDisp[pos]++;
            }
        }

        acabou = true;
        for (int i = 0; i < quantArquivos; i++) //verifica se todos os arquivos acabaram
            if (!acabouDisp[i])
            {
                acabou = false;
                break;
            }
        if (acabou) //imprime a ultima media da ultima chave
            cout << fixed << setprecision(30) << resultFinal/contFinal << endl;
    }

    for(int j = 0; j < quantArquivos; j++)
        arquivo[j].close();

    for(int i = 0; i < quantArquivos ;i++){
        delete[] disps[i].chave;
        delete[] disps[i].valor;
    }
    delete[] arquivo;
    delete[] disps;
    delete[] acabouDisp;
    delete[] podeAcabar;
    delete[] contDisp;
}


int main(int argc, char **argv){

    ifstream entrada(argv[1]); //abre o arquivo de entrada base
    if (!entrada.is_open())
    { //verifica se o arquivo foi aberto
        cerr << " Erro ao abrir arquivo de entrada.\n";
        exit(1);
    }

    int cont1 = 0, cont2 = 0, quantArquivos;        // contadores para guardarem as posicoes das chaves usadas para a ordenaçao
    int cap_memoria = atoi(argv[2]); //capacidade de leitura

    string str; //string auxiliar para leitura de uma linha de arquivo e logo apos onverter em um vertor char
    //char colunas[n];

    posicaoChaves(entrada, argv, cont1, cont2); //funcoes para encotrar as posicoes das colunas onde estao as chaves
    
    quantArquivos = part_ordena(entrada, cap_memoria, cont1, cont2); //cria n.m ou  n/m+1 arquivos auxiliares com m lihas ordenadas

    intercala(quantArquivos, cap_memoria); //intercala os arquivos auxiliares e imprime a medi de cada chave do arquivo base

    return 0;
}
