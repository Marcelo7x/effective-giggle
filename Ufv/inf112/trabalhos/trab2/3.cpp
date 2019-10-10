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

void imprimeChaves(/*char **argv,*/ Dispositivos *disp1, int linhas){
    //cout << argv[3] << " " << argv[4] << endl;
    for (int i = 0; i < linhas; i++)
        cout << disp1[i].chave << " " << disp1[i].valor << endl;
}

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

Dispositivos leituraChave(ifstream& entrada, int cont1, int cont2, int igno){
    Dispositivos disp1;
    string str;
    char *temp,*aux;
    int pos1, pos2, j;
        
    for (int i = 0; i < igno; i++)
        getline(entrada, str);
    

    getline(entrada, str);
    cout << "get = " << str << endl;
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
            disp1.chave[j] = temp[i];
            disp1.chave[j+1] = '\0'; 
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
    disp1.valor = atoi(aux);
    delete[] aux;
    return disp1;
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

            disp1[i] =leituraChave(entrada, cont1, cont2, 0);

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
            saida << disp1[i].chave << "," << disp1[i].valor << endl;
        
        cont_disp++;
        saida.close();
    }
    return cont_disp;
}

void part_ordena2(int num_disp, int cap_memoria, int metadinha, int metade){
    Dispositivos *disp1= new Dispositivos[cap_memoria], disp_temp, menor;
    char disp[20], *c, *aux, *aux2;
    string str;
    int num_disp2 = num_disp, l = 0,linha = 0, linhaax;
    int num_disp3 = (num_disp)/2, pos, posaux;
    int igno = 0, menorint;
    int a[num_disp]{0};
    bool linhaca[num_disp] = {false}, acabou = false;

    for (size_t i = 0; i < num_disp; i++)
    linhaca[i] = false;
    for (int j = 0; j < cap_memoria; j++)
    {
        sprintf(disp, "disp%d.txt", linha+metade);
            ifstream saida(disp);
            if (!saida.is_open()){
                cerr << " Erro ao abrir arquivo de ifstream.\n";
                exit(1);
            }
        disp1[j] = leituraChave(saida,0,1,l);
        cout << "1 "<< disp1[j].chave << " " << disp1[j].valor << endl;
        saida.close();    
        linha++;
        a[j]++;
    }
    linha-=cap_memoria;
    for (int j = 0; j < cap_memoria*2; j++)
    {
        menor = disp1[0];
        pos = 0;
        for(int i = 1; i < cap_memoria; i++)
        {
            if (strcmp(disp1[i].chave, menor.chave) < 0)
            {
                menor = disp1[i];
                pos = i;
            }
        }
            sprintf(disp, "disp%d.txt", num_disp2+metadinha);
            fstream saida(disp, std::fstream::out | std::fstream::app);
            if (!saida.is_open()){
            cerr << " Erro ao abrir arquivo de ifstream.\n";
            exit(1);
            }
            cout <<"2"<<  menor.chave << " " << menor.valor << endl;
            saida << menor.chave << "," << menor.valor << endl;
            saida.close();
            
            cout << "a"<< pos << " " << a[pos];
            if (a[pos] == cap_memoria)
            {
                    linhaca[pos] = true;
                    //linhaax = linha;
                    cout << "lin " << pos;
                    posaux = pos;
                for (int k = 0; k < cap_memoria; k++)
                {
                    if (linhaca[k] == false){
                        pos = k;
                        cout << "lina " << pos;
                        sprintf(disp, "disp%d.txt", pos+metade);
                        ifstream saida2(disp);
                        if (!saida2.is_open()){
                            cerr << " Erro ao abrir arquivo de ifstream.\n";
                            exit(1);
                        }
                        
                        disp1[posaux] = leituraChave(saida2, 0, 1,a[pos]);
                        a[pos]++;
                        saida.close();
                        break;
                        }
                    if (k == cap_memoria-1)
                    {
                        sprintf(disp, "disp%d.txt", num_disp2+metadinha);
                        fstream saida(disp, std::fstream::out | std::fstream::app);
                        if (!saida.is_open()){
                        cerr << " Erro ao abrir arquivo de ifstream.\n";
                        exit(1);
                        }
                        for (int i = 0; i < cap_memoria; i++){
                            if (i != pos){
                            cout << ",," << pos << endl;
                                saida << disp1[i].chave << "," << disp1[i].valor << endl;
                                cout <<"2~"<<  disp1[i].chave << " " << disp1[i].valor << endl;
                            }
                        }
                        saida.close();
                        acabou = true;
                    }
                    
                }
                if (acabou)
                    break;

                continue;
            }

            sprintf(disp, "disp%d.txt", pos+metade);
            ifstream saida2(disp);
            if (!saida2.is_open()){
                cerr << " Erro ao abrir arquivo de ifstream.\n";
                exit(1);
            }
            
            
            disp1[pos] = leituraChave(saida2, 0, 1,a[pos]);
            a[pos]++;
            saida.close();

    } 
}

void part_ordena2(int num_disp, int cap_memoria, int cap_memoria2, int metadinha, int metade){
    Dispositivos *disp1= new Dispositivos[cap_memoria], disp_temp, menor;
    char disp[20], *c, *aux, *aux2;
    string str;
    int num_disp2 = num_disp, l = 0,linha = 0, linhaax;
    int num_disp3 = (num_disp)/2, pos, posaux;
    int igno = 0, menorint;
    int a[num_disp]{0};
    bool linhaca[num_disp] = {false}, acabou = false;

    for (size_t i = 0; i < num_disp; i++)
    linhaca[i] = false;
    for (int j = 0; j < cap_memoria; j++)
    {
        sprintf(disp, "disp%d.txt", linha+metade);
            ifstream saida(disp);
            if (!saida.is_open()){
                cerr << " Erro ao abrir arquivo de ifstream.\n";
                exit(1);
            }
        disp1[j] = leituraChave(saida,0,1,l);
        cout << "1 "<< disp1[j].chave << " " << disp1[j].valor << endl;
        saida.close();    
        linha++;
        a[j]++;
    }
    linha-=cap_memoria;
    for (int j = 0; j < cap_memoria2*2; j++)
    {
        menor = disp1[0];
        pos = 0;
        for(int i = 1; i < cap_memoria; i++)
        {
            if (strcmp(disp1[i].chave, menor.chave) < 0)
            {
                menor = disp1[i];
                pos = i;
            }
        }
            sprintf(disp, "disp%d.txt", num_disp2+metadinha);
            fstream saida(disp, std::fstream::out | std::fstream::app);
            if (!saida.is_open()){
            cerr << " Erro ao abrir arquivo de ifstream.\n";
            exit(1);
            }
            cout <<"2"<<  menor.chave << " " << menor.valor << endl;
            saida << menor.chave << "," << menor.valor << endl;
            saida.close();
            
            cout << "a"<< pos << " " << a[pos];
            if (a[pos] == cap_memoria2)
            {
                    linhaca[pos] = true;
                    //linhaax = linha;
                    cout << "lin " << pos;
                    posaux = pos;
                for (int k = 0; k < cap_memoria; k++)
                {
                    if (linhaca[k] == false){
                        pos = k;
                        cout << "lina " << pos;
                        sprintf(disp, "disp%d.txt", pos+metade);
                        ifstream saida2(disp);
                        if (!saida2.is_open()){
                            cerr << " Erro ao abrir arquivo de ifstream.\n";
                            exit(1);
                        }
                        
                        disp1[posaux] = leituraChave(saida2, 0, 1,a[pos]);
                        a[pos]++;
                        saida.close();
                        break;
                        }
                    if (k == cap_memoria-1)
                    {
                        sprintf(disp, "disp%d.txt", num_disp2+metadinha);
                        fstream saida(disp, std::fstream::out | std::fstream::app);
                        if (!saida.is_open()){
                        cerr << " Erro ao abrir arquivo de ifstream.\n";
                        exit(1);
                        }
                        for (int i = 0; i < cap_memoria; i++){
                            if (i != pos){
                            cout << ",," << pos << endl;
                                saida << disp1[i].chave << "," << disp1[i].valor << endl;
                                cout <<"2~"<<  disp1[i].chave << " " << disp1[i].valor << endl;
                            }
                        }
                        saida.close();
                        acabou = true;
                    }
                    
                }
                if (acabou)
                    break;

                continue;
            }

            sprintf(disp, "disp%d.txt", pos+metade);
            ifstream saida2(disp);
            if (!saida2.is_open()){
                cerr << " Erro ao abrir arquivo de ifstream.\n";
                exit(1);
            }
            
            
            disp1[pos] = leituraChave(saida2, 0, 1,a[pos]);
            a[pos]++;
            saida.close();

    } 
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

    int k = 0;
    int cont_disp = part_ordena(entrada, cap_memoria,cont1, cont2);
    for (int i = 0; i < cap_memoria; i++){
        if (cont_disp/cap_memoria < cap_memoria)
            break;
        part_ordena2(cont_disp, cap_memoria, i, k);
        k+=cont_disp/cap_memoria;
    }
    //num_disp+= cap_memoria;
    part_ordena2(cont_disp+cap_memoria, cap_memoria, cap_memoria*2, 0, cap_memoria*2);
    
    entrada.close();
    
    delete[] colunas;

    return 0;
}
