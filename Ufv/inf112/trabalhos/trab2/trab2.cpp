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

void Quick(Dispositivos *disps, int inicio, int fim){
   
   Dispositivos pivo, aux;
   int i, j, meio;
   
   i = inicio;
   j = fim;
   
   meio = (int) ((i + j) / 2);
   pivo = disps[meio];
   
   do{
       
        while (strcmp(disps[i].chave, pivo.chave) < 0) i = i + 1;
        while (strcmp(disps[j].chave, pivo.chave) > 0) j = j - 1;
        //cout << "a" << endl; 
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
    getline(entrada, str);
    char *colunas = converte(str); //coverte a string auxiliar par aum vetor de char
    char *temp = new char[strlen(colunas)+1]; //vertor de char auxiliar
    *temp = '\0';
    bool achou1 = false, achou2 = false;    //indica se encontrou as chaves
    int i = 0,j = 0;
    for (i = 0, j = 0; colunas[i] != '\0'; i++)
    {
            if(isalnum(colunas[i]) || colunas[i] == '.'){
                temp[j] = colunas[i];
                temp[j + 1] = '\0';
                j++;
            }
            
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
    Dispositivos disps; //os regitros das chaves desejadas em struct
    string str;         //string e ponteios para char nescessarios
    char *temp, *aux;
    int pos1 = 0, pos2 = 0, j = 0;
    bool acabou = false;    
   
    for (int i = 0; i < igno; i++){ //ignora linhas ja lidas
        getline(entrada, str);
    }

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
{ //le registros, ordena-os e grava em um dispositivo auxiliar
    char disp[20];
    Dispositivos *disps = new Dispositivos[cap_memoria];
    int cont_disp = 0, cont, j;
    bool passou = false;

    while (true)
    {
        if (entrada.peek() == -1)
            break;

        sprintf(disp, "arquivo%d.txt", cont_disp); //abre arquivo auxiliar para a gravacao
        fstream saida(disp, std::fstream::in | std::fstream::out | std::fstream::app);
        if (!saida.is_open())
        { //verifica se arquivo foi aberto
            cerr << " Erro ao abrir arquivo de fstream.\n";
            exit(1);
        }

        cont = 0;

        for (int i = 0; i < cap_memoria; i++)
        {
            if (entrada.peek() == -1)
            {
                passou = true;
                break;
            }

            disps[i] = leituraChave(entrada, cont1, cont2, 0); //extrai registros
            //cout << "comfir " << disps[i].chave << ' ' << disps[i].valor << endl;
            cont++;
        }

        Quick(disps, 0, cont-1);

        for (int i = 0; i < cont; i++)
        {
            if (i != cont - 1)
            {
                if (isalnum(disps[i].chave[0]))
                {
                //grava os registros ordenados nos arquivos
                saida << disps[i].chave << "," << disps[i].valor << endl;
                //cout << disps[i].chave << "," << disps[i].valor << endl;
                }
            }
            else
            {
                saida << disps[i].chave << "," << disps[i].valor;
                //cout << disps[i].chave << "," << disps[i].valor << " acab"<< endl;
            }
            
            delete[] disps[i].chave;
            delete[] disps[i].valor;
        }
        cont_disp++;
        saida.close(); //fecha os arquivos
    }
   
    delete[] disps;
    
    return cont_disp;
}

void intercala(int cont_dispositivos, int capac_memoria)
{
    Dispositivos *disps = new Dispositivos[cont_dispositivos];
    long double resultFinal = 0;
    char disp_aux[25], disp_aux2[25];
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
    
    ifstream arquivo[cont_dispositivos];
    for (int j = 0; j < cont_dispositivos; j++)
    {
        sprintf(disp_aux, "arquivo%d.txt", j);
        //sprintf(disp_aux2, "arquivo%d", i);
                 
                arquivo[j].open(disp_aux);
                if (!arquivo[j].is_open())
                {
                    cerr << "Nao abriu o arquivo\n";
                    exit(1);
                }
    }

    for (int i = 0; i < cont_dispositivos; i++)
    {
        if (!acabouDisp[i])
        {

           /* sprintf(disp_aux, "arquivo%d.txt", i);

            ifstream arquivo(disp_aux);
            if (!arquivo.is_open())
            {
                cerr << i <<"Nao abriu o arquivo\n";
                
                exit(1);
            }*/

            disps[i] = leituraChave(arquivo[i], 0, 1, 0);
            //cout << "disp " << disps[i].chave << " " << disps[i].valor << endl;

            //arquivo.close();
            contDisp[i]++;
        }
    }

    Dispositivos menor;
    char auxx[100];
    int contFinal = 0;
    

    //menor.chave = new char[strlen(disps[0].chave)*3];
    int pos = 0;
    int i = 0;

    /*sprintf(disp_aux, "arquivoFinal%d.txt", cont_dispositivos);

        fstream arquivoFinal(disp_aux, std::fstream::out | std::fstream::app);
        if (!arquivoFinal.is_open())
        {
            cerr << "Nao abriu o arquivo final\n";
            exit(1);
        }*/
    

    while (!acabou)
    {
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
        
        //cout << disps[pos].chave << "," << disps[pos].valor << endl;
        //cout << contFinal << endl;
        if (resultFinal == 0){
            cout << disps[pos].chave << " ";
            resultFinal = stod(disps[pos].valor);
            strcpy(auxx, disps[pos].chave);
            contFinal = 1;
        } 
        else if (strcmp(disps[pos].chave, auxx) == 0)
        {
            resultFinal += stod(disps[pos].valor);
            contFinal++;
        }
        else if (strcmp(disps[pos].chave, auxx) > 0)
        {
            cout << fixed << setprecision(15) << resultFinal/contFinal << endl;
           // cout << auxx << " " << resultFinal << " "<< contFinal << endl;
            cout << disps[pos].chave << " ";
            resultFinal = stod(disps[pos].valor);
            strcpy(auxx, disps[pos].chave);
            contFinal = 1;
        }
        

        delete[] disps[pos].chave;
        delete[] disps[pos].valor;
        desalocouChave[pos] = true;
        

        if (podeAcabar[pos]){
            acabouDisp[pos] = true;
        } 

        if (!acabouDisp[pos])
        {
            /*sprintf(disp_aux, "arquivo%d.txt", pos);

            ifstream arquivo(disp_aux);
            if (!arquivo.is_open())
            {
                cerr << "Nao abriu o arquivo\n";
                exit(1);
            }*/

            if (arquivo[pos].peek() == -1)
            {
                acabouDisp[pos] = true;
                i++;
            }

            else
            {
                disps[pos] = leituraChave(arquivo[pos], 0, 1, 0);
                if (arquivo[pos].peek() == 0)
                {
                    podeAcabar[pos] = true;
                }

                desalocouChave[pos] = false;
                contDisp[pos]++;
            }
            //arquivo.close();
        }

        acabou = true;
        for (int i = 0; i < cont_dispositivos; i++)
            if (!acabouDisp[i])
            {
                acabou = false;
                break;
            }
        if (acabou){
            cout << fixed << setprecision(15) << resultFinal/contFinal << endl;
        }
        
    }
    //arquivoFinal.close();
    for (int j = 0; j < cont_dispositivos; j++)
            {
                arquivo[j].close();
            }
    for(int i = 0; i < cont_dispositivos ;i++)
        if(!desalocouChave[i]){
            delete[] disps[i].chave;
            delete[] disps[i].valor;
        }
    
    delete[] disps;
    delete[] acabouDisp;
    delete[] podeAcabar;
    delete[] contDisp;
    delete[] desalocouChave;
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
    
    quantArquivos = part_ordena(entrada, cap_memoria, cont1, cont2);

    intercala(quantArquivos, cap_memoria);

  

    return 0;
}
