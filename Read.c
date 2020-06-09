#include<stdio.h>
#include <stdlib.h>
#include<math.h>
#include<string.h>

int *RevisarMatriz(char path[]){         //Funcion que lee la matriz para conocer las dimensiones que tiene
    FILE *archivo;                                      //Devuelve un puntero a entero con las dimensiones
    char str[100];
    archivo=fopen(path,"r");
    int balance=0;
    int fil=0,col=0;
    if(archivo!=NULL){
        while(!feof(archivo)){
            fgets(str,100,archivo);
            for(int i=0;i<strlen(str);i++){
                if(str[i]=='{')
                    balance++;
                if(str[i]=='}')
                    balance--;
                if(str[i]==48||str[i]==49)
                    col++;
            }
            fil++;
        }
        col=col/fil;
        if(balance==0)
            printf("Balanceado\n %d Filas\n %d Columnas\n",fil,col);
        else printf("No Balanceado\n");
    }
    fclose(archivo);
    int *dimensiones = (int *)malloc(2*(sizeof(int)));
    dimensiones[0]=fil;
    dimensiones[1]=col;

    return dimensiones;
}

int ***CrearMatriz(char path[]){                     //Funcion que crea la matriz a partir del archivo leido
    FILE *archivo;
    char str[100];
    int *dimensiones = RevisarMatriz(path);
    int fil=dimensiones[0];
    int col=dimensiones[1];

    int ***Mundo=(int ***)malloc(3*(sizeof(int**)));               //Reservo Memoria para llenar mi matriz de enteros con los 
    for(int i=0;i<fil;i++){                                                 //parametros encontrados
        Mundo[i]=(int **)malloc(fil*(sizeof(int*)));
        for(int j=0;j<col;j++)
            Mundo[i][j]=(int *)malloc(col*(sizeof(int)));
    }

    int j=0,k=0;                                                            
    archivo=fopen(path,"r");                            
    if (archivo!=NULL){                                                     //Leo nuevamente mi archivo para guardar los caracteres 1 y 0
        while(!feof(archivo)){
            fgets(str,100,archivo);
            printf("%s\n",str);
            for(int i=0;i<strlen(str);i++){
                if(str[i]==48||str[i]==49){
                    Mundo[0][j][k]=str[i]-'0';
                    k++;
                }
            }
            j++;
            k=0;
        }
    }
    fclose(archivo);
    printf("\n");

    for(int i=0;i<fil;i++){                                             //Imprimo mi matriz para comprobar
        for(j=0;j<col;j++)
            printf("%d ",Mundo[0][i][j]);
        printf("\n");                                                   //Falta verificar que las , esten balanceadas y que los numeros no sean 
    }                                                                   //diferentes de 1 y 0.
    return Mundo;
}

int ***MatrizAux(char path[]){
    FILE *archivo;
    char str[100];
    int *dimensiones = RevisarMatriz(path);
    int fil=dimensiones[0];
    int col=dimensiones[1];

    int ***Aux=(int ***)malloc(3*(sizeof(int**)));               //Reservo Memoria para llenar mi matriz de enteros con los 
    for(int i=0;i<fil;i++){                                                 //parametros encontrados
        Aux[i]=(int **)malloc(fil*(sizeof(int*)));
        for(int j=0;j<col;j++)
            Aux[i][j]=(int *)malloc(col*(sizeof(int)));
    }
    return Aux;
}

void SigGeneration(int ***Mundo, int ***Auxiliar,int *dimensiones){
    int i,j,alive,dead,fil=dimensiones[0],col=dimensiones[1];
    for(i=0;i<fil;i++)
        for(j=0;j<col;j++){
            alive=0;
            if(Mundo[0][i-1][j-1]==1)
                alive++;
            if(Mundo[0][i-1][j]==1)
                alive++;
            if(Mundo[0][i+1][j+1]==1)
                alive++;
            if(Mundo[0][i][j-1]==1)
                alive++;
            if(Mundo[0][i][j+1]==1)
                alive++;
            if(Mundo[0][i+1][j-1]==1)
                alive++;
            if(Mundo[0][i+1][j]==1)
                alive++;
            if(Mundo[0][i+1][j+1]==1)
                alive++;
            if(Mundo[0][i][j]==1 && (alive<2 || alive>3))
                Auxiliar[0][i][j]=0;
            else if(Mundo[0][i][j]==0 && alive==3)
                Auxiliar[0][i][j]=1;  
            else Auxiliar[0][i][j]=Mundo[0][i][j];     
        }
    printf("Generacion 2\n");
    for(i=0;i<fil;i++){
        for(j=0;j<col;j++)
            printf("%d ",Auxiliar[0][i][j]);
        printf("\n");
    }
}

int main(){
    int ***Mundo;
    int ***Auxiliar;
    int *dimensiones=RevisarMatriz("prueba.txt");
    Mundo=CrearMatriz("prueba.txt");
    Auxiliar=MatrizAux("prueba.txt");
    SigGeneration(Mundo,Auxiliar,dimensiones);
}