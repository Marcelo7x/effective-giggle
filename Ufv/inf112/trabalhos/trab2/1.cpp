#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;


int main(int argc, char **argv){

    ifstream entrada(argv[1]);
    if (!entrada.is_open()){
        cerr << " Erro ao abrir arquivo de entrada.\n";
        exit(1);
    }
    int cont1 = 0, cont2 = 0;
    bool a = false, b = false;
    
    char classes[100], temp[100];
    
    entrada.getline(classes,100);
    //cout << classes << endl;

    for (int i = 0, j = 0; i < 100; i++)
    {
        temp[j] = classes[i];
        temp[j+1] = '\0';
        j++;
        //cout << temp << endl;
        
        /*if (a == false)
            cont1++;
        if (b == false)
            cont2++;*/

        if (strcmp(temp, argv[3]) == 0)
            a = true; 

        if (strcmp(temp, argv[4]) == 0)
            b = true;
       
        if ((a == true)&&(b == true))
            break;
        
        if (classes[i+1] == ',')
        {
            i++;
            j = 0;
            
            if (a == false)
                cont1++;
            if (b == false)
                cont2++;
        }
        //cout << a << b << endl;
    }
    cout << cont1 << " " << cont2 << endl;

    struct ordena
    {
        char chave[100];
        int valor;
    };
    
    ordena *disp1 = new ordena[100];
    char aux[100];
    
    int pos1 = 0, pos2 = 0, j = 0, k = 0;
    while (true)
    {
        if (entrada.eof())
            break;
        //entrada.ignore();
        entrada.getline(temp,100);
        cout << temp << endl;
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
                disp1[k].chave[j] = temp[i];
                disp1[k].chave[j+1] = '\0'; 
                j++;  
            }
            cout << pos1 <<" " <<cont1 <<disp1[k].chave << endl;
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
             cout << pos2 <<" " <<cont2 <<aux << endl;
        }
        disp1[k].valor = atoi(aux);
        cout << disp1[k].valor << endl;
        k++;
    }
    
    for (int i = 0; i < k; i++)
    {
        cout << disp1[i].chave << " " << disp1[i].valor << endl;
    }
    
    
    
    entrada.close();

    return 0;
}
