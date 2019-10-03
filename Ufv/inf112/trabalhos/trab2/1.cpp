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
    int n = 0, m = 0;
    
    char temp1[100], temp2[100];

    bool a = true , b = true;

    while (a==true || b==true)
    {
        entrada.getline(temp1,100, ',');
        
        if (strcmp(temp1, argv[2]) == 0){
            cout << temp1 << " ";
            a = false;        
        }    
        if (strcmp(temp1, argv[3]) == 0){
            b = false;    
            cout << temp1 << " ";
        }
        if (a == true)
            n++;
        if (b == true)
            m++;
    }
    
  
    cout << n << " " << m << endl;
  
    entrada.close();

    return 0;
}
