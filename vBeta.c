#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFF_SIZE 1024
#define numCartas 22


typedef struct Carta {
    int id;
    char *nombre;
    char *desc;
    char *tag;
} Carta;

//variables globales (rectificar)
Carta *cartas;
int karma;
char **preguntas;
int nPreguntas;
int nCartas;
int *mazoMezclado;


//funciones
int cargarPreguntas(FILE *fichero);
int construirMazo(FILE *fichero);
int indexOf(int *indices, int cont, int num);
void mezclarCartas (int *indices, int cont);
void barajar (int cont);
void mostrarCartas (int cont);
void intercambiarCartas(int i, int n);
int pedirSeleccion();
void retirarCarta(int *mazo, int cont,int seleccion);
int actualizarKarma(int seleccion);
void iniciarPreguntas();



int main(int argc, char const *argv[])
{
    karma = 0;
    //INTRODUCCION - experiancia oraculo o_0
    char nombre;
    int sexo;

        printf("\nEl hombre lleva preguntandole al oraculo desde tiempos inmemorables, tratando obtener respuestas a cerca del futuro.\n\n");
        printf("        Este oraculo basado en la lectura de las cartas del Tarot,\n        le ayudara a encontrar respuesta a sus problemas,\n        tanto amorosos, laborales como familiares.\n\n");
        printf("!!!A CONTINUACION, CON LA AYUDA DE LOS 22 ARCANOS MAYORES, SU FUTURO SERA REVELADO ANTE USTED!!!\n\n\n");
        printf("Introduzca Su Nombre:\n");
        scanf("%s", &nombre);
        //printf("comprobacion");

    //CARGAR CARTAS DEL CSV - memoria dinamica

    FILE *fichCartas;


    if(fichCartas = fopen("arcanosMayores.csv", "r")){
        nCartas = construirMazo(fichCartas);

    }else{
        perror("Error al abrir el archivo");
    }


    //PREGUNTAS - selecciona cartas sin sustitucion
    //mostrarCartas(nCartas);
    printf("\n");
    barajar(nCartas-1);
    printf("\n");
    //mostrarCartas(nCartas-1);
    printf("\n");

    //CARGAMOS LAS PREGUNTAS
    FILE *fichPreguntas;

    if(fichPreguntas = fopen("preguntas.txt", "r")){
        nPreguntas = cargarPreguntas(fichPreguntas);
    }else{
        perror("Error al abrir el archivo");
    }

    iniciarPreguntas();

    printf("TU KARMA ES %i\n", karma);
    printf("Npreguntas %i\n", nPreguntas);
    float karmaPorcentaje;
    karmaPorcentaje = karma/nPreguntas;
    //no funciona
    printf("TU KARMA ES %f \n", karmaPorcentaje);

    //RESULTADOS - analisis de respuestas y resultados

    return 0;
}

//FUNCIONES

int cargarPreguntas(FILE *fichero){
    char linea[BUFF_SIZE];

    int cont = 0;

    while(!feof(fichero)){
        fgets(linea, BUFF_SIZE, fichero);
        cont++;
    }

    rewind(fichero);

    preguntas = (char**) malloc(cont*sizeof(linea));
    if(preguntas == NULL){
        perror("No se pudo reservar memoria suficiente");
    }

    int i;
    for ( i = 0; i < cont; i++)
    {
        fgets(linea, BUFF_SIZE, fichero);
        //quitamos salto de linea '\n'
        linea[strlen(linea)-1]=0;
        preguntas[i] = strdup(linea);
    }
    return cont;
}


int construirMazo(FILE *fichero){
    //DEFINIMOS LA LINEA
    char linea[BUFF_SIZE];

    int cont = 0;

    //LEEMOS LA CANTIDAD DE LINEAS
    do{
        fgets(linea, BUFF_SIZE, fichero);
        cont++;
    }while(!feof(fichero));

    cont = cont-1; // 1 cab - 2 primera nula

    //REINICIAMOS EL PUNTERO AL PRINCIPIO
    rewind(fichero);

    //RESERVAMOS MEMORIA PARA CADA CARTA
    cartas = (Carta*)malloc(cont*sizeof(Carta));
    if(cartas == NULL){
        perror("No se pudo reservar memoria suficiente");
    }

    int i;
    char *tok;

    //CARGAMOS LAS CARTAS
    //ignora la primera linea
    fgets(linea, BUFF_SIZE, fichero);
    for (i = 0; i<cont - 1; i++)
    {
        fgets(linea, BUFF_SIZE, fichero);
        //if (i != 0){
            //LEEMOS EL ID

            tok = strtok(linea,";");
            cartas[i].id = atoi(tok);

            //LEEMOS EL NOMBRE
            tok = strtok(NULL,";");
            cartas[i].nombre = strdup(tok);

            //LEEMOS EL TAG
            tok = strtok(NULL,";");
            cartas[i].tag = strdup(tok);;

            //LEEMOS LA DESCRIPCION
            tok = strtok(NULL,";");
            cartas[i].desc = strdup(tok);;
        //}
    }
    return cont;
}

int indexOf(int *indices, int cont, int num){
    int i;
    for (i = 0; i < cont; i++)
    {
        if(indices[i]==num){
            return i;
        }
    }
    return -1;
}

void mostrarCartas (int cont)
{
    int i;
    printf("\nEl num de cartas es: %d", cont);

    for (i=0; i<cont-1; i++)
    {
        printf("\n%d - %s", cartas[i].id, cartas[i].nombre);
    }
}

void mezclarCartas (int *indices, int cont)
{
    int i, n;
    int aux;
    char auxtxt;
            srand(time(NULL));

    for (i = 0; i<numCartas ; i++)
    {
            n = rand() % numCartas;

            intercambiarCartas(i, n);
    }
    /*for (i = 0; i<numCartas ; i++)
    {
        printf("%d, ", cartas[i].id);
    }*/
}

void barajar (int cont)
{
    int i, n;
    int aux;
    char auxtxt;
    srand(time(NULL));

    for (i = 0; i<cont-1 ; i++)
    {
        n = rand() % cont;
        intercambiarCartas(i, n);
    }

    printf("\n");
    printf("*barajando*");

    /* for (i = 0; i<cont-1 ; i++)
    {
        printf("%d, ", cartas[i].id);
    }*/
}

int pedirSeleccion(int maxim)
{
    int seleccion;
    do
    {
         printf("Selecciona una carta del [1,%d]: ", maxim);
         scanf("%d", &seleccion);
         seleccion = seleccion - 1;
         //printf("Ha seleccionado la carta");
    }
    while (seleccion > maxim-1 || seleccion < 0 );
return seleccion;
}

int actualizarKarma(int seleccion)
{
    if(strcmp(cartas[seleccion].tag, "Bueno")==0){
        karma++;}
return karma;
}

void iniciarPreguntas(){
    int i;
    int seleccion;
    int karma;
   // printf("el num de preguntas es: %d", nPreguntas);
   nCartas = numCartas;
    for (i = 0; i < nPreguntas; i++)
    {
        printf("\n%s\n\n", preguntas[i]);
        seleccion = pedirSeleccion((nCartas));

        if(strcmp(cartas[seleccion].tag, "Bueno")==0)
            {karma = 1; }
            else {karma = 0;};

        //printf("\nHa seleccionado: %d", cartas[seleccion].id);
        printf("\nSu carta es: %s           +%d karma", cartas[seleccion].nombre, karma);
        printf("\n\n%s", cartas[seleccion].desc);
        printf("\n                                                karma total: %d", actualizarKarma(seleccion));
        quitarCarta(seleccion, nCartas);
        //mostrarCartas(nCartas);
        //actualizarKarma(mazoMezclado[seleccion]);
        //retirarCarta(mazoMezclado,nCartas,seleccion);
        nCartas--;
    }
}

void quitarCarta(int n, int max)
{
    printf("\nQuitamos carta : %s\n\n", cartas[n].nombre);

    for(int i = n; i < max; i++)
    {
      intercambiarCartas(i, (i+1));
    }
}

void intercambiarCartas(int i, int n)
{
    int aux;
    char *auxtxt;

    aux = cartas[i].id;
    cartas[i].id = cartas[n].id;
    cartas[n].id = aux;

    auxtxt = cartas[i].nombre;
    cartas[i].nombre = cartas[n].nombre;
    cartas[n].nombre = auxtxt;

    auxtxt = cartas[i].tag;
    cartas[i].tag = cartas[n].tag;
    cartas[n].tag = auxtxt;

    auxtxt = cartas[i].desc;
    cartas[i].desc = cartas[n].desc;
    cartas[n].desc = auxtxt;

}
