#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

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
            for(int i=1;i<dimensiones[1]+1;i++){
                for(int j=1;j<dimensiones[2]+1;j++){
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
        for(int i=1;i<dimensiones[1]+1;i++){
            for(int j=1;j<dimensiones[2]+1;j++)
                fprintf(fp,"%d ",Matriz[idx][i][j]);
            fprintf(fp, "\n");
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}

void RemoveBlanks(char *str){
    char straux[strlen(str)];
    int i=0,j=0;
    while (str[i]!='\0'){
        if(!(str[i]==' ') && (!(str[i]=='\n'))){
            straux[j] = str[i];
            j++;
        }
        i++;
    }
    straux[j] = '\0';
    strcpy(str,straux);
}

int *RevisarMatriz(char path[]){         //Funcion que lee la matriz para conocer las dimensiones que tiene
    FILE *archivo=NULL;
    char str[100];
    char array_str[1000]={" "};
    int i=0,j=0;
    int ContKeys=0,col=0,fil=0,flag=0,auxcont=0;
    int *dimensiones = (int *)malloc(3*(sizeof(int)));
    archivo=fopen(path,"r");
    if(archivo==NULL){
        printf("Archivo no encontrado!\n");
        EXIT_FAILURE;
    }

    //***********************DEJAMOS MATRIZ LINEAL****************************

    fgets(str,100,archivo);
    while(!feof(archivo)){
        strcat(array_str,str);
        fgets(str,100,archivo);
    }
    strcat(array_str,str);
    fclose(archivo);
    RemoveBlanks(array_str);
    printf("%s\n",array_str);


    //***********************CONDICIONES DE VALIDACIONES**********************

    for(i=0;array_str[i]!='\0';i++){
        if(array_str[i]!='{' && array_str[i]!=',' && array_str[i]!='}' && array_str[i]!='1' && array_str[i]!='0'){
            printf("ERROR EN MATRIZ");
            dimensiones[0]=-1;
            return dimensiones;
        }
        if(array_str[i]=='{'){
            ContKeys++;
            if(i!=0){
                col=0;
                for(j=i;array_str[j]!='}';j++){
                    if(array_str[j]=='1' || array_str[j]=='0'){
                        col++;
                        if(array_str[j+1]!=','&& array_str[j+1]!='}'){
                            printf("ERROR EN MATRIZ\n");
                            dimensiones[0]=-1;
                            return dimensiones;
                        }
                    }
                    if(array_str[j]>=50 && array_str[j]<=58){
                        printf("ERROR EN MATRIZ\n");
                        dimensiones[0]=-1;
                        return dimensiones;
                    }
                }
            }
            if(flag==0 && i!=0){
                auxcont=col;
                flag=1;
            }
            if(flag==1 && col!=auxcont){
                printf("ERROR EN MATRIZ\n");
                dimensiones[0]=-1;
                return dimensiones;
            }
            if(ContKeys>=3){
                printf("ERROR EN MATRIZ\n");
                dimensiones[0]=-1;
                return dimensiones;
            }


        }
        if(array_str[i]=='}'){
            ContKeys--;
            if(ContKeys==1)
                fil++;
            if(array_str[i+1]!=',' && array_str[i+1]!='}' && array_str[i+1]!='\0'){
                printf("ERROR EN MATRIZ\n");
                dimensiones[0]=-1;
                return dimensiones;
            }

        }

        if(array_str[i]==','){
            if((array_str[i-1]!='1' && array_str[i-1]!='0')||(array_str[i+1]!='1' && array_str[i+1]!='0'))
                if(array_str[i-1]!='}' || array_str[i+1]!='{'){
                    printf("ERROR EN MATRIZ\n");
                    dimensiones[0]=-1;
                    return dimensiones;
                }
        }

    }
        if(ContKeys!=0){
            printf("ERROR EN MATRIZ\n");
            dimensiones[0]=-1;
            return dimensiones;
        }

    printf("Filas: %d\nColumnas: %d\n",fil,col);

    dimensiones[0]=1;
    dimensiones[1]=fil;
    dimensiones[2]=col;

    return dimensiones;
}

int ***CrearMatriz(char path[]){                     //Funcion que crea la matriz a partir del archivo leido
    FILE *archivo;
    char str[100];
    int *dimensiones = RevisarMatriz(path);
    if(dimensiones[0]==-1)
        EXIT_FAILURE;
    int fil=dimensiones[1];
    int col=dimensiones[2];

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
    int fil=dimensiones[1];
    int col=dimensiones[2];

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
    int i,j,alive,fil=dimensiones[1],col=dimensiones[2];
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
    int i,j,alive,fil=dimensiones[1],col=dimensiones[2];
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
    int i,j,alive,fil=dimensiones[1],col=dimensiones[2];
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
            for(int j=1;j<dimensiones[1]+1;j++){

                for(int k=1;k<dimensiones[2]+1;k++)
                    printf("%d ",Auxiliar[0][j][k]);
                printf("   ");
                for(int k=1;k<dimensiones[2]+1;k++)
                    printf("%d ",Auxiliar[1][j][k]);
                printf("   ");
                for(int k=1;k<dimensiones[2]+1;k++)
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
            for(int j=1;j<dimensiones[1]+1;j++){
                for(int k=1;k<dimensiones[2]+1;k++)
                    printf("%d ",Mundo[0][j][k]);
                printf("   ");
                for(int k=1;k<dimensiones[2]+1;k++)
                    printf("%d ",Mundo[1][j][k]);
                printf("   ");
                for(int k=1;k<dimensiones[2]+1;k++)
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
    FILE *fp=NULL;
    fp=fopen(path,"r");
    if(fp==NULL){
        printf("Archivo no encontrado.\n");
        return 0;
    }else fclose(fp);
    printf("Ingrese el tiempo de espera en milisegundos\n");
    scanf("%f",&milseg);
    milseg=milseg/1000;
    printf("%f\n",milseg);
    int *dimensiones=RevisarMatriz(path);
    StartGame(path,gen,milseg);
    return 0;
}
