#include<stdio.h>
#include <stdlib.h>
#include<math.h>
#include<string.h>

int *RevisarMatriz(char path[],int generation){         //Funcion que lee la matriz para conocer las dimensiones que tiene
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
    int *dimensiones = (int *)malloc(col*(sizeof(int)));
    dimensiones[0]=generation;
    dimensiones[1]=fil;
    dimensiones[2]=col;

    return dimensiones;
}

void CrearMatriz(char path[],int generation){                     //Funcion que crea la matriz a partir del archivo leido
    FILE *archivo;
    char str[100];
    int *dimensiones = RevisarMatriz(path,generation);
    int fil=dimensiones[1];
    int col=dimensiones[2];

    int ***Mundo=(int ***)malloc(generation*(sizeof(int**)));               //Reservo Memoria para llenar mi matriz de enteros con los 
    for(int i=0;i<fil;i++){                                                 //parametros encontrados
        Mundo[i]=(int **)malloc(fil*(sizeof(int*)));
        for(int j=0;j<col;j++)
            Mundo[i][j]=(int *)malloc(col*(sizeof(int)));
    }

    int ***MundoAux=(int ***)malloc(generation*(sizeof(int***)));
    for(int i=0;i<fil;i++){                                                 //Reservo Memoria para la matriz auxiliar.
        MundoAux[i]=(int **)malloc(fil*(sizeof(int*)));
        for(int j=0;j<col;j++)
            MundoAux[i][j]=(int *)malloc(col*(sizeof(int)));
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
}



void SigGeneration(int ***Mundo);

int main(){
    CrearMatriz("prueba.txt",5);
}