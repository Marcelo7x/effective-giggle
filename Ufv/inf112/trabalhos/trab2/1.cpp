#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

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

int leituraChaves(Dispositivos *disp1, ifstream& entrada, int n, int cont1, int cont2){
    char aux[n], temp[n];
    int pos1, pos2, j, linhas = 0;

    while (true)
    {
        if (entrada.eof())
            break;

        entrada.getline(temp,100);

        j=0; pos1 = 0; pos2 = 0;
        for (int i = 0; i < 100; i++)
        {
            if (temp[i] == ',' ){
                pos1++;
                i++;
            }
            if (pos1 >cont1)
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
        for (int i = 0; i < 100; i++)
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
    }
    return linhas;
}

void imprimeChaves(char **argv, Dispositivos *disp1, int linhas){
    cout << argv[3] << " " << argv[4] << endl;
    for (int i = 0; i < linhas; i++)
        cout << disp1[i].chave << " " << disp1[i].valor << endl;
}

int main(int argc, char **argv){

    ifstream entrada(argv[1]);
    if (!entrada.is_open()){
        cerr << " Erro ao abrir arquivo de entrada.\n";
        exit(1);
    }
    int n = 100, linhas;
    int cont1 = 0, cont2 = 0;
    
    char colunas[n];
    
    entrada.getline(colunas,100);
    posicaoChaves(colunas, argv, n, cont1, cont2);
    
    Dispositivos *disp1 = new Dispositivos[n];

    linhas = leituraChaves(disp1, entrada, n, cont1, cont2);

    imprimeChaves(argv, disp1, linhas);
    
    entrada.close();

    return 0;
}
