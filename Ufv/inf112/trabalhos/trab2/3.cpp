#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
using namespace std;

char* converte(string str){     //converte uma string para um vertor de char alocado dinamicamente
    char *c = new char [str.length()+1];
    strcpy (c, str.c_str());
    return c;
}

struct Dispositivos     //struct para facilitar a ordenacao dos registros
    {
        char chave[100];
        int valor;
    };

void imprimeChaves(/*char **argv,*/ Dispositivos *disp1, int linhas){
    //cout << argv[3] << " " << argv[4] << endl;
    for (int i = 0; i < linhas; i++)
        cout << disp1[i].chave << " " << disp1[i].valor << endl;
}

void posicaoChaves(ifstream &entrada, char **argv ,int &cont1, int &cont2){ //encontra a posicao das chaves a serem utilizadas 
                                                                            //para a aordenacao
    
    string str;             //string auxiliar
    getline(entrada, str);  
    char * colunas = converte(str); //coverte a string auxiliar par aum vetor de char

    char *temp = new char [sizeof(colunas)];    //vertor de char auxiliar
    bool achou1 = false, achou2 = false;            //indica se encontrou as chaves

    for (int i = 0, j = 0; colunas[i] != '\0'; i++)     
    {
        temp[j] = colunas[i];
        temp[j+1] = '\0';
        j++;
      
        if (strcmp(temp, argv[3]) == 0)     //verica se encontrou a primeira chave
           achou1 = true; 

        if (strcmp(temp, argv[4]) == 0)     //verifica se encotrou a segunda chave
            achou2 = true;
       
        if ((achou1 == true)&&(achou2 == true))
            break;
        
        if (colunas[i+1] == ',')
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

Dispositivos leituraChave(ifstream& entrada, int cont1, int cont2, int igno){   //le uma linha d arquivo de entrada e devolve 
    Dispositivos disp1;                                                         //os regitros das chaves desejadas em struct
    string str;         //string e ponteios para char nescessarios
    char *temp,*aux;
    int pos1 = 0, pos2 = 0, j = 0;
        
    for (int i = 0; i < igno; i++)  //ignora linhas ja lidas
        getline(entrada, str);
    

    getline(entrada, str);          // le a linha desejadas e converte
    cout << "get = " << str << endl;
    temp = converte(str);


    for (int i = 0; temp[i] != '\0'; i++)
    {
        if (temp[i] == ',' ){   //procura a posicao da primeira chave
            pos1++;
            i++;
        }
        if (pos1 > cont1)
        {
            break;
        }
        if ((cont1 == pos1) && (temp[i]!='\n') ){   //apos encontrar a posicao da chave copia o registro pra um struct 
            disp1.chave[j] = temp[i];
            disp1.chave[j+1] = '\0'; 
            j++;  
        }
    }
    j = 0;
    aux = new char[sizeof(temp)];

    for (int i = 0; temp[i] != '\0'; i++)
    {
        if (temp[i] == ',' ){   //procura a posicao da primeira chave
            pos2++;
            i++;
        }
        if (pos2 > cont2)
        {
            break;
        }
        if ((cont2 == pos2) && (temp[i]!='\n') ){   //apos encontrar a posicao da chave copia o registro pra um struct
            aux[j] = temp[i];
            aux[j+1] = '\0';
            j++;
        }
    }
    
    disp1.valor = atoi(aux);
    
    delete[] aux;
    return disp1;
}

int part_ordena(ifstream& entrada, int cap_memoria, int cont1, int cont2){   //le registros, ordena-os e grava em um dispositivo auxiliar
    char disp[20];
    Dispositivos *disp1 = new Dispositivos [cap_memoria], disp_temp;
    int cont_disp = 0, cont, j;
    bool passou = false;
   
    while(true){
        if (entrada.eof())
            break;

        sprintf(disp, "disp%d.txt", cont_disp); //abre arquivo auxiliar para a gravacao 
        fstream saida(disp, std::fstream::in | std::fstream::out | std::fstream::app);
        if (!saida.is_open()){  //verifica se arquivo foi aberto
            cerr << " Erro ao abrir arquivo de fstream.\n";
            exit(1);
            }
        
        cont = 0;
        
        for (int i = 0; i < cap_memoria; i++)
        {
            if (entrada.eof()){
                passou = true;
                break;
            }

            disp1[i] =leituraChave(entrada, cont1, cont2, 0);   //extrai registros
            cont++;

            if (i > 0)
            {
                j = i;
                while(j>0)
                {
                    
                    if (strcmp(disp1[j].chave, disp1[j-1].chave) < 0)   //ordena os registros 
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
        
        for (int i = 0; i < cont; i++)      //grava os registros ordenados nos arquivos
            saida << disp1[i].chave << "," << disp1[i].valor << endl;
        
        cont_disp++;
        saida.close();  //fecha os arquivos
    }
    
    delete[] disp1;
    return cont_disp;
}

void part_ordena2(int num_disp, int cap_memoria, int metadinha, int metade){
    Dispositivos *disp1= new Dispositivos[cap_memoria], disp_temp, menor;
    char disp[20], *c, *aux, *aux2;
    string str;
    int num_disp2 = num_disp, l = 0,linha = 0, linhaax, o;
    int num_disp3 = (num_disp)/2, pos, posaux;
    int igno = 0, menorint, acabouint;
    int a[num_disp]{0};
    bool linhaca[num_disp] = {false}, acabou = false, acabou2 = false;

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
        if(acabou2 == false)
            menor = disp1[0];
        if((acabou2) && acabouint == 0 )
            menor = disp1[1];
        if((acabou2) && acabouint == 1 )
            menor = disp1[0];
        
        pos = 0;
        for(int i = 1; i < cap_memoria; i++)
        {
           if((acabou2) && i == acabouint)
                continue;

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
            o = j;
            for (size_t i = 0; i < num_disp; i++)
            {
                cout << " a[i] = "<< a[i] <<" ";
            }
            cout << endl;

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
                        cout << " teste ";
                        leituraChave(saida2, 0, 1,a[pos]-1);
                        
                        if (saida2.eof() || o == ((cap_memoria*2)-1) ){
                            cout << "acabou \n";
                            acabou2 = true;
                            acabouint = posaux;
                            break; 
                        }

                        cout << " m "; 
                        disp1[posaux] = leituraChave(saida2, 0, 1,0);
                        

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

void part_ordena3(int num_disp, int cap_memoria, int cap_memoria2, int metadinha, int metade){
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
            cerr << " Erro ao abrir arquivo de fstream.\n";
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
    
    ifstream entrada(argv[1]);      //abre o arquivo de entrada base 
    if (!entrada.is_open()){        //verifica se o arquivo foi aberto
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
    int cont_disp = part_ordena(entrada, cap_memoria, cont1, cont2);//faz leitura das linhas do arquivo base e guarda 
                                                                    /// em n/m ou n/m+1 arquivos auxiliares
    for (int i = 0; i < cap_memoria; i++){      //ordena os regitros dos arquivos auxiliares e grava em  (n/m)/2 arquivos
        if (cont_disp/cap_memoria < cap_memoria)
            break;
        part_ordena2(cont_disp, cap_memoria, i, k);
        k+=cont_disp/cap_memoria;
        passou =true;
    }
    //num_disp+= cap_memoria;
    if (passou)
        part_ordena3(cont_disp+cap_memoria, cap_memoria, cap_memoria*2, 0, cap_memoria*2);
    else
        part_ordena3(cont_disp, cap_memoria, cap_memoria+1, 0, 0);
    
    entrada.close();


    return 0;
}
