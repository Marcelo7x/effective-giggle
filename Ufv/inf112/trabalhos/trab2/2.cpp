#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
using namespace std;

char* converte(string str){
    char *c = new char [str.length()+1];
    strcpy (c, str.c_str());
    return c;
}

struct Dispositivos
    {
        char chave[100];
        int valor;
    };

void posicaoChaves(char *colunas, char **argv , int n, int &cont1, int &cont2){
    char temp[n];
    bool aux1 = false, aux2 = false;
    for (int i = 0, j = 0; i < 100; i++)
    {
        temp[j] = colunas[i];
        temp[j+1] = '\0';
        j++;
      
        if (strcmp(temp, argv[3]) == 0)
           aux1 = true; 

        if (strcmp(temp, argv[4]) == 0)
            aux2 = true;
       
        if ((aux1 == true)&&(aux2 == true))
            break;
        
        if (colunas[i+1] == ',')
        {
            i++;
            j = 0;
            
            if (aux1 == false)
                cont1++;
            if (aux2 == false)
                cont2++;
        }
    }
}

int leituraChave(Dispositivos *disp1, ifstream& entrada, int cont1, int cont2){
    string str;
    char *temp,*aux;
    int pos1, pos2, j, linhas = 0;

    getline(entrada, str);
    temp = converte(str);


    j=0; pos1 = 0; pos2 = 0;
    for (int i = 0; temp[i] != '\0'; i++)
    {
        if (temp[i] == ',' ){
            pos1++;
            i++;
        }
        if (pos1 > cont1)
        {
            break;
        }
        if ((cont1 == pos1) && (temp[i]!='\n') ){
            disp1[linhas].chave[j] = temp[i];
            disp1[linhas].chave[j+1] = '\0'; 
            j++;  
        }
    }
    j=0;
    aux = new char[sizeof(temp)];
    for (int i = 0; temp[i] != '\0'; i++)
    {
        if (temp[i] == ',' ){
            pos2++;
            i++;
        }
        if (pos2 > cont2)
        {
            break;
        }
        if ((cont2 == pos2) && (temp[i]!='\n') ){
            aux[j] = temp[i];
            aux[j+1] = '\0';
            j++;
        }
    }
    disp1[linhas].valor = atoi(aux);
    linhas++;
    delete[] aux;
    
    return linhas;
}

int part_ordena(ifstream& entrada, int cap_memoria, int cont1, int cont2){
    char disp[20];
    int j;

    Dispositivos disp1[cap_memoria], disp_temp;
    int cont_disp = 0;
   
    while(true){
        if (entrada.eof())
            break;

        sprintf(disp, "disp%d.txt", cont_disp);
        fstream saida(disp, std::fstream::in | std::fstream::out | std::fstream::app);
        if (!saida.is_open()){
            cerr << " Erro ao abrir arquivo de fstream.\n";
            exit(1);
            }

        for (int i = 0; i < cap_memoria; i++)
        {
            if (entrada.eof())
                break;

            leituraChave((disp1+i), entrada, cont1, cont2);

            if (i > 0)
            {
                j = i;
                while(j>0)
                {
                    
                    if (strcmp(disp1[j].chave, disp1[j-1].chave) < 0)
                    {
                        disp_temp = disp1[j-1];
                        disp1[j-1] = disp1[j];
                        disp1[j] = disp_temp;
                        j--;
                    }
                    else
                        break;
                }
            }
        }
        for (int i = 0; i < cap_memoria; i++)  // BUG
            saida << disp1[i].chave << " " << disp1[i].valor << endl;
        
        cont_disp++;
        saida.close();
    }
}

void imprimeChaves(char **argv, Dispositivos *disp1, int linhas){
    cout << argv[3] << " " << argv[4] << endl;
    for (int i = 0; i < linhas; i++)
        cout << disp1[i].chave << " " << disp1[i].valor << endl;
}

int main(int argc, char **argv){

    int j;
    ifstream entrada(argv[1]);
    if (!entrada.is_open()){
        cerr << " Erro ao abrir arquivo de entrada.\n";
        exit(1);
    }
    int n = 100, linhas = 0;
    int cont1 = 0, cont2 = 0;
    int cap_memoria = atoi(argv[2]);
    
    string str;
    //char colunas[n];
    
    getline(entrada, str);
    char * colunas = converte(str);

    posicaoChaves(colunas, argv, n, cont1, cont2);

    int cont_disp = part_ordena(entrada, cap_memoria,cont1, cont2);
    
    
    entrada.close();
    
    delete[] colunas;

    return 0;
}
