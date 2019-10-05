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
    cout << classes << endl;

    for (int i = 0, j = 0; i < 100; i++)
    {
        temp[j] = classes[i];
        temp[j+1] = '\0';
        j++;
        cout << temp << endl;
        
        if (a == false)
            cont1++;
        if (b == false)
            cont2++;
            
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
        cout << a << b << endl;
    }
    
    cout << cont1 << " " << cont2 << endl;

    entrada.close();

    return 0;
}
