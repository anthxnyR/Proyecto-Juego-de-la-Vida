#include<stdio.h>
#include <stdlib.h>
#include<math.h>
#include<string.h>

int *RevisarMatriz(char path[]){         //Funcion que lee la matriz para conocer las dimensiones que tiene
    FILE *archivo=NULL;                                      //Devuelve un puntero a entero con las dimensiones
    char str[100];
    archivo=fopen(path,"r");
    if(archivo==NULL);
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
    for(int i=0;i<fil+2;i++){                                                 //parametros encontrados
        Mundo[i]=(int **)malloc((fil+2)*(sizeof(int*)));
        for(int j=0;j<col+2;j++)
            Mundo[i][j]=(int *)malloc((col+2)*(sizeof(int)));
    }

    for(int i=0;i<3;i++)
        for(int j=0;j<(fil+2);j++)
            for(int k=0;k<(col+2);k++)
                Mundo[i][j][k]=9;

    int j=1,k=1;
    archivo=fopen(path,"r");
    if (archivo!=NULL){                                                     //Leo nuevamente mi archivo para guardar los caracteres 1 y 0
        while(!feof(archivo)){
            fgets(str,100,archivo);
            printf("%s\n",str);
            for(int i=0;i<strlen(str);i++){
                if(str[i]==48||str[i]==49){
                    Mundo[0][j][k]=str[i]-'0';
                    Mundo[1][j][k]=str[i]-'0';
                    Mundo[2][j][k]=str[i]-'0';
                    k++;
                }
            }
            j++;
            k=1;
        }
    }
    fclose(archivo);
    printf("\n");
    printf("Matriz logica\n");

    for(int i=0;i<fil+2;i++){                                             //Imprimo mi matriz para comprobar
        for(j=0;j<col+2;j++)
            printf("%d ",Mundo[0][i][j]);
        printf("\n");                                                   //Falta verificar que las , esten balanceadas y que los numeros no sean
    }

    CreateRecords(Mundo,0,dimensiones);
    CreateRecords(Mundo,1,dimensiones);
    CreateRecords(Mundo,2,dimensiones);
                                                                   //diferentes de 1 y 0.
    return Mundo;
}


int ***MatrizAux(char path[]){
    int *dimensiones = RevisarMatriz(path);
    int fil=dimensiones[0];
    int col=dimensiones[1];

    int ***Aux=(int ***)malloc(3*(sizeof(int**)));               //Reservo Memoria para llenar mi matriz de enteros con los
    for(int i=0;i<fil+2;i++){                                                 //parametros encontrados
        Aux[i]=(int **)malloc((fil+2)*(sizeof(int*)));
        for(int j=0;j<col+2;j++)
            Aux[i][j]=(int *)malloc((col+2)*(sizeof(int)));
    }

    for(int i=0;i<3;i++)
        for(int j=0;j<(fil+2);j++)
            for(int k=0;k<(col+2);k++)
                Aux[i][j][k]=9;

    return Aux;
}

int ***NextGenClassic(int ***Mundo, int ***Auxiliar,int *dimensiones,int idx){
    int i,j,alive,fil=dimensiones[0],col=dimensiones[1];
    for(i=1;i<fil+1;i++)
        for(j=1;j<col+1;j++){
            alive=0;
            if(Mundo[idx][i-1][j-1]==1)
                alive++;
            if(Mundo[idx][i-1][j]==1)
                alive++;
            if(Mundo[idx][i-1][j+1]==1)
                alive++;
            if(Mundo[idx][i][j-1]==1)
                alive++;
            if(Mundo[idx][i][j+1]==1)
                alive++;
            if(Mundo[idx][i+1][j-1]==1)
                alive++;
            if(Mundo[idx][i+1][j]==1)
                alive++;
            if(Mundo[idx][i+1][j+1]==1)
                alive++;
            if(Mundo[idx][i][j]==1 && (alive<2 || alive>3))
                Auxiliar[idx][i][j]=0;
            else if(Mundo[idx][i][j]==0 && alive==3)
                Auxiliar[idx][i][j]=1;
                else Auxiliar[idx][i][j]=Mundo[idx][i][j];
        }
    return Auxiliar;
}

int ***NextGenHor(int ***Mundo, int ***Auxiliar, int *dimensiones,int idx){     //Recibe idx que indicara el indice de la matriz que trabaja
    int i,j,alive,fil=dimensiones[0],col=dimensiones[1];
    for(i=1;i<fil+1;i++)
        for(j=1;j<col+1;j++){
            alive=0;
            if(Mundo[idx][i-1][j-1]==1)
                alive++;
                else if(Mundo[idx][i-1][j-1]==9 && Mundo[idx][fil][j-1]==1)
                    alive++;
            if(Mundo[idx][i-1][j]==1)
                alive++;
                else if(Mundo[idx][i-1][j]==9 && Mundo[idx][fil][j]==1)
                    alive++;
            if(Mundo[idx][i-1][j+1]==1)
                alive++;
                else if(Mundo[idx][i-1][j+1]==9 && Mundo[idx][fil][j+1]==1)
                    alive++;
            if(Mundo[idx][i][j-1]==1)
                alive++;
            if(Mundo[idx][i][j+1]==1)
                alive++;
            if(Mundo[idx][i+1][j-1]==1)
                alive++;
                else if(Mundo[idx][i+1][j-1]==9 && Mundo[idx][1][j-1]==1)
                    alive++;
            if(Mundo[0][i+1][j]==1)
                alive++;
                else if(Mundo[0][i+1][j]==9 && Mundo[0][1][j]==1)
                    alive++;
            if(Mundo[idx][i+1][j+1]==1)
                alive++;
                else if(Mundo[idx][i+1][j+1]==9 && Mundo[idx][1][j+1]==1)
                    alive++;
            if(Mundo[idx][i][j]==1 && (alive<2 || alive>3))
                Auxiliar[idx][i][j]=0;
                else if(Mundo[idx][i][j]==0 && alive==3)
                    Auxiliar[idx][i][j]=1;
                    else
                        Auxiliar[idx][i][j]=Mundo[idx][i][j];
        }
    return Auxiliar;
}

int ***NextGenVert(int ***Mundo, int ***Auxiliar, int *dimensiones,int idx){
    int i,j,alive,fil=dimensiones[0],col=dimensiones[1];
    for(i=1;i<fil+1;i++)
        for(j=1;j<col+1;j++){
            alive=0;
            if(Mundo[idx][i-1][j-1]==1)
                alive++;
                else if(Mundo[idx][i-1][j-1]==9 && Mundo[idx][i-1][col]==1)
                    alive++;
            if(Mundo[idx][i-1][j]==1)
                alive++;
            if(Mundo[idx][i-1][j+1]==1)
                alive++;
                else if(Mundo[idx][i-1][j+1]==9 && Mundo[idx][i-1][1]==1)
                    alive++;
            if(Mundo[idx][i][j-1]==1)
                alive++;
                else if(Mundo[idx][i][j-1]==9 && Mundo[idx][i][col]==1)
                    alive++;
            if(Mundo[idx][i][j+1]==1)
                alive++;
                else if(Mundo[idx][i][j+1]==9 && Mundo[idx][i][1]==1)
                    alive++;
            if(Mundo[0][i+1][j-1]==1)
                alive++;
                else if(Mundo[idx][i+1][j-1]==9 && Mundo[idx][i+1][col]==1)
                    alive++;
            if(Mundo[idx][i+1][j]==1)
                alive++;
            if(Mundo[idx][i+1][j+1]==1)
                alive++;
                else if(Mundo[idx][i+1][j+1]==9 && Mundo[idx][i+1][1]==1)
                    alive++;
            if(Mundo[idx][i][j]==1 && (alive<2 || alive>3))
                Auxiliar[idx][i][j]=0;
                else if(Mundo[idx][i][j]==0 && alive==3)
                    Auxiliar[idx][i][j]=1;
                    else
                        Auxiliar[idx][i][j]=Mundo[idx][i][j];
        }
    return Auxiliar;
}

void StartGame(char path[],int generation, int milseg){
    FILE *archivo;
    int i=1,flag=0;
    int ***Mundo, ***Auxiliar;
    int *dimensiones=RevisarMatriz(path);
    Mundo=CrearMatriz(path);
    Auxiliar=MatrizAux(path);
    while(i<=generation){
        if(flag==0){
            NextGenClassic(Mundo,Auxiliar,dimensiones,0);
            NextGenHor(Mundo,Auxiliar,dimensiones,1);
            NextGenVert(Mundo,Auxiliar,dimensiones,2);

            if(i!=1)
                sleep(milseg);
            printf("Generacion %d\n\n",i);
            printf("  Classic          Horizontal        Vertical\n");
            for(int j=1;j<dimensiones[0]+1;j++){

                for(int k=1;k<dimensiones[1]+1;k++)
                    printf("%d ",Auxiliar[0][j][k]);
                printf("   ");
                for(int k=1;k<dimensiones[1]+1;k++)
                    printf("%d ",Auxiliar[1][j][k]);
                printf("   ");
                for(int k=1;k<dimensiones[1]+1;k++)
                    printf("%d ",Auxiliar[2][j][k]);
                printf("\n");
            }
            flag=1;
            SaveRecord(Auxiliar,i,dimensiones,0);
            SaveRecord(Auxiliar,i,dimensiones,1);
            SaveRecord(Auxiliar,i,dimensiones,2);

        }else if(flag==1){
            NextGenClassic(Auxiliar,Mundo,dimensiones,0);
            NextGenHor(Auxiliar,Mundo,dimensiones,1);
            NextGenVert(Auxiliar,Mundo,dimensiones,2);

            sleep(milseg);
            printf("Generacion %d\n\n",i);
            printf("  Classic          Horizontal        Vertical\n");
            for(int j=1;j<dimensiones[0]+1;j++){
                for(int k=1;k<dimensiones[1]+1;k++)
                    printf("%d ",Mundo[0][j][k]);
                printf("   ");
                for(int k=1;k<dimensiones[1]+1;k++)
                    printf("%d ",Mundo[1][j][k]);
                printf("   ");
                for(int k=1;k<dimensiones[1]+1;k++)
                    printf("%d ",Mundo[2][j][k]);
                printf("\n");
            }
            flag=0;
            SaveRecord(Mundo,i,dimensiones,0);
            SaveRecord(Mundo,i,dimensiones,1);
            SaveRecord(Mundo,i,dimensiones,2);
        }
        i++;
        printf("\n");
    }
}

void CreateRecords(int ***Matriz,int idx,int *dimensiones){
    int i,j;
    FILE *archivo;
    switch(idx){
        case 0:
            archivo=fopen("RecordClassic.txt","w");
            break;
        case 1:
            archivo=fopen("RecordHorizontal.txt","w");
            break;
        case 2:
            archivo=fopen("RecordVertical.txt","w");
            break;
    }
    if(archivo){
        fprintf(archivo,"%s\n","Generacion 0");
            for(int i=1;i<dimensiones[0]+1;i++){
                for(int j=1;j<dimensiones[1]+1;j++){
                    fprintf(archivo,"%d ",Matriz[idx][i][j]);
                }
                fprintf(archivo,"\n");
            }
        fprintf(archivo,"\n");
    }
    fclose(archivo);

}

void SaveRecord(int ***Matriz, int gen, int *dimensiones,int idx){
    FILE *fp;
    switch(idx){
        case 0:
            fp=fopen("RecordClassic.txt","a+");
            break;
        case 1:
            fp=fopen("RecordHorizontal.txt","a+");
            break;
        case 2:
            fp=fopen("RecordVertical.txt","a+");
            break;
    }

    if(fp!=NULL){
        fprintf(fp,"%s %d\n","Generacion",gen);
        for(int i=1;i<dimensiones[0]+1;i++){
            for(int j=1;j<dimensiones[1]+1;j++)
                fprintf(fp,"%d ",Matriz[idx][i][j]);
            fprintf(fp, "\n");
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}


int main(){
    int ***Mundo;
    int ***Auxiliar;
    int gen,i=0,flag=0;
    float milseg;
    char path[100];
    printf("Ingrese el numero de generaciones\n");
    scanf("%d",&gen);
    printf("Ingrese el documento a agregar\n");
    scanf("%s",path);
    if(!strstr(path,".txt"))
        strcat(path,".txt");
    printf("%s\n",path);
    printf("Ingrese el tiempo de espera en milisegundos\n");
    scanf("%f",&milseg);
    milseg=milseg/1000;
    printf("%f\n",milseg);
    int *dimensiones=RevisarMatriz(path);
    StartGame(path,gen,milseg);
    return 0;
}
