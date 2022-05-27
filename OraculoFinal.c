#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>

#define BUFF_SIZE 1024


typedef struct Carta {
    int id;
    char *nombre;
    char *desc;
    char *tag;
} Carta;


char **cargarPreguntas(FILE *fichero, int nPreguntas,  char **preguntas);
void mostrarPreguntas (int cont, char **preguntas);
int construirMazo(FILE *fichero, Carta *cartas);
Carta *cargarMazo(FILE *fichero);
int numCartasMazo(Carta *mazo);
int showCartasMazo(Carta *mazo);
void baraja (Carta *mazo);
void intercambiarCartas(int i, int n, Carta *cartas);
int actualizaKarma(int seleccion, Carta *mazo);
int obtenerKarma(int seleccion, Carta *mazo);
void quitarCarta(int n, int max, Carta *cartas);
int numeroPreguntas(FILE *fichero);
void iniciarPreguntas(int nPreguntas, Carta *cartas, char **preguntas );
int hacerPreguntas(int nPreguntas, Carta *cartas, char **preguntas );
float resultadoKarma(int karma, int numPreguntas);
void leerDescripciones(float reskar);
void guardaLog(char *nombre,int karma);
void oraculo();

int main(int argc, char const *argv[])
{
    int n, opcion;

    do
    {
        printf( "\n\n\n   1. Oraculo.", 163 );
//        printf( "\n   2. Ver estadísticas.", 163 );
//        printf( "\n   3. Graficos.", 163 );
        printf( "\n   2. Salir." );
        printf( "\n\n   Introduzca opci%cn (1-2): ", 162 );

        scanf( "%d", &opcion );

        switch ( opcion )
        {
            case 1: oraculo();
                    break;

            case 3:
    break;
         }


    } while ( opcion != 2 );

    return 0;
}

void oraculo()
{

    setlocale(LC_ALL,""); // Conf. acentos

    Carta *cartas;
    int nCartas;
    char **preguntas;

    int karma = 0;
    int nPreguntas;
    //INTRODUCCION - experiancia oraculo o_0
    char *nombre;
    int sexo;

    int numeroCartasMazo;

    printf("\nEl hombre lleva preguntandole al oraculo desde tiempos inmemorables, tratando obtener respuestas a cerca del futuro.\n\n");
    printf("        Este oraculo basado en la lectura de las cartas del Tarot,\n        le ayudara a encontrar respuesta a sus problemas,\n        tanto amorosos, laborales como familiares.\n\n");
    printf("!!!A CONTINUACION, CON LA AYUDA DE LOS 22 ARCANOS MAYORES, SU FUTURO SERA REVELADO ANTE USTED!!!\n\n\n");
    printf("Introduzca Su Nombre:\n");
    scanf("%s", &nombre);

    FILE *fichCartas;

    if(fichCartas = fopen("arcanosMayores.csv", "r")){
        cartas = cargarMazo(fichCartas);

    }else{
        perror("Error al abrir el archivo");
    }

    numeroCartasMazo = numCartasMazo(cartas);
    nCartas = numeroCartasMazo;
    showCartasMazo(cartas);

    printf("\n");
    baraja(cartas);
    showCartasMazo(cartas);

    FILE *fichPreguntas;
    char linea[BUFF_SIZE];

    if(fichPreguntas = fopen("preguntas.txt", "r")){
        nPreguntas = numeroPreguntas(fichPreguntas);
    }else{
        perror("Error al abrir el archivo preguntas");
    }

    preguntas = (char**) malloc(nPreguntas*sizeof(linea));
    if(preguntas == NULL){
        perror("No se pudo reservar memoria suficiente");
    }

//    printf("\nEl num de nPreguntas es: %d", nPreguntas);
    preguntas = cargarPreguntas(fichPreguntas, nPreguntas,  preguntas);

    karma = hacerPreguntas(nPreguntas, cartas, preguntas);
    printf("\nEl Karma final es: %d", karma);

    float reskar = resultadoKarma(karma, nPreguntas);
    leerDescripciones(reskar);

    guardaLog(&nombre,karma);

    return 0;
}

char **cargarPreguntas(FILE *fichero,int nPreguntas, char **preguntas){

    char linea[BUFF_SIZE];

    rewind(fichero);
    int i;
    for ( i = 0; i < nPreguntas-1; i++)
    {
        fgets(linea, BUFF_SIZE, fichero);
        linea[strlen(linea)-1]='\0';
        //printf("\nLA LINEA ES %s", linea);

        preguntas[i] = strdup(linea);
    }
    return preguntas;
}

int numeroPreguntas(FILE *fichero){
    char **preguntas;
    char linea[BUFF_SIZE];

    int cont = 0;

    while(!feof(fichero)){
        fgets(linea, BUFF_SIZE, fichero);
        cont++;
    }

    return cont;
}


int construirMazo(FILE *fichero, Carta *cartas){
    char linea[BUFF_SIZE];

    int cont = 0;

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

    fgets(linea, BUFF_SIZE, fichero); //ignora la primera linea
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


int numeroCartas(FILE *fichero){
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
    return cont;
}


Carta *cargarMazo(FILE *fichero){

	Carta *mazo;

    //DEFINIMOS LA LINEA
    char linea[BUFF_SIZE];

    int cont = 0;

    cont = numeroCartas(fichero);

    //REINICIAMOS EL PUNTERO AL PRINCIPIO
    rewind(fichero);

    //RESERVAMOS MEMORIA PARA CADA CARTA
    mazo = (Carta*)malloc(cont*sizeof(Carta)+1);
    if(mazo == NULL){
        perror("No se pudo reservar memoria suficiente");
    }

    int i;
    char *tok;

    //CARGAMOS LAS MAZO
    //ignora la primera linea
    fgets(linea, BUFF_SIZE, fichero);
    for (i = 0; i<cont - 1; i++)
    {
        fgets(linea, BUFF_SIZE, fichero);
        //if (i != 0){
            //LEEMOS EL ID

            tok = strtok(linea,";");
            mazo[i].id = atoi(tok);

            //LEEMOS EL NOMBRE
            tok = strtok(NULL,";");
            mazo[i].nombre = strdup(tok);

            //LEEMOS EL TAG
            tok = strtok(NULL,";");
            mazo[i].tag = strdup(tok);;

            //LEEMOS LA DESCRIPCION
            tok = strtok(NULL,";");
            mazo[i].desc = strdup(tok);;
        //}
    }
    mazo[cont-1].id = -1;
    mazo[cont-1].nombre = "";
    mazo[cont-1].tag = "";
    mazo[cont-1].desc = "";
    return mazo;
}

int numCartasMazo(Carta *mazo){

    Carta *cartas_ptr = mazo;
    int i = 0;

    while (cartas_ptr->id != -1){
        cartas_ptr++;
        i++;
    }

    return i;
}

int showCartasMazo(Carta *mazo){

    Carta *cartas_ptr = mazo;
    int i = 0;

    while ((cartas_ptr->id != -1) && (i<28)){
        //printf("\n%i %s - %s - %s", cartas_ptr->id,  cartas_ptr->nombre, cartas_ptr->tag,  cartas_ptr->desc );
//        printf("\n%i %s", cartas_ptr->id,  cartas_ptr->nombre);
        cartas_ptr++;
        i++;
    }

    return i;
}


void mostrarPreguntas (int cont, char **preguntas)
{
    int i;
    printf("\nEl num de pregs es: %d", cont);

    for (i=0; i<cont-1; i++)
    {
        printf("\n%s", preguntas[i]);
    }
}


void baraja (Carta *mazo)
{
    int i, n, cont;
    int aux;
    char auxtxt;

    Carta *auxPtrCarta;

    srand(time(NULL));
    cont = numCartasMazo(mazo);
    //printf ("El contador es %i", cont);

    for (i = 0; i<cont-1 ; i++)
    {
        n = rand() % cont;
        intercambiarCartas(i, n, mazo);
//        printf("\nIntercambiamos cartas %i - %i", i, n);
    }

    printf("\n");
    printf("***** barajando el mazo ******\n\n");
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

int obtenerKarma(int seleccion, Carta *mazo)
{
	int auxKarma =0;
    if(strcmp(mazo[seleccion].tag, "Bueno")==0){
        auxKarma =1;}
return auxKarma;
}

void iniciarPreguntas(int nPreguntas, Carta *cartas, char **preguntas ){
    int i;
    int seleccion;
    int karma=0;
    int nCartas;
   // printf("el num de preguntas es: %d", nPreguntas);
    nCartas = numCartasMazo(cartas);
    for (i = 0; i < nPreguntas; i++)
    {
        printf("\n%s\n\n", preguntas[i]);
        seleccion = pedirSeleccion((nCartas));

        if(strcmp(cartas[seleccion].tag, "Bueno")==0)
            {karma++; }
            //else {karma = 0;};

        //printf("\nHa seleccionado: %d", cartas[seleccion].id);
        printf("\nSu carta es: %s           +%d karma", cartas[seleccion].nombre, karma);
        printf("\n\n%s", cartas[seleccion].desc);
        //printf("\n                                                karma total: %d", actualizaKarma(seleccion,cartas));
        printf("\n                                                karma total: %d", karma);
        quitarCarta(seleccion, nCartas, cartas);
        nCartas--;
    }
}

int hacerPreguntas(int nPreguntas, Carta *cartas, char **preguntas ){
    int i;
    int seleccion;
    int karma=0;
    int nCartas;
   // printf("el num de preguntas es: %d", nPreguntas);
    nCartas = numCartasMazo(cartas);
    for (i = 0; i < nPreguntas; i++)
    {
        printf("\n%s\n\n", preguntas[i]);
        seleccion = pedirSeleccion((nCartas));

        if(strcmp(cartas[seleccion].tag, "Bueno")==0)
            {karma++; }
            //else {karma = 0;};

        //printf("\nHa seleccionado: %d", cartas[seleccion].id);
        printf("\nSu carta es: %s           +%d karma", cartas[seleccion].nombre, karma);
        printf("\n\n%s", cartas[seleccion].desc);
        //printf("\n                                                karma total: %d", actualizaKarma(seleccion,cartas));
        printf("\n                                                karma total: %d", karma);
        quitarCarta(seleccion, nCartas, cartas);
        nCartas--;
    }
    return karma;
}

void quitarCarta(int n, int max,  Carta *cartas)
{
    printf("\nQuitamos carta : %s\n\n", cartas[n].nombre);

    for(int i = n; i < max; i++)
    {
      intercambiarCartas(i, (i+1),cartas);
    }
}

void intercambiarCartas(int i, int n, Carta *cartas)
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

float resultadoKarma(int karma, int numPreguntas)
{
    float resultado;
    float karma1, numpreguntas1;
    karma1 = karma;
    numpreguntas1 = numPreguntas;

    resultado = karma1 / (numpreguntas1-2);
//    printf("Tu karma division es: %f", resultado);
    return resultado;
}

void leerDescripciones(float reskar)
{
    char *parrafo, *d1, *d2, *d3, *d4, c;
    char **texto;
    FILE *descripciones;
    descripciones = fopen("DESCRIPCIONES.txt", "r");
    int iChar = 0, nParrafos = 0, nCaracteresParrafo = 0, nCaracteresmax = 0, nCaracteres;
    int parrafoLargo;

    while(fscanf(descripciones, "%c", &c) != EOF)
    {
        iChar++;
        if(c == '\n')
        {
                nParrafos++;
                if(iChar > nCaracteresmax)
                {
                    parrafoLargo = nParrafos;
                    nCaracteresmax = iChar;
                }
            iChar = 0;
        }
        else if (c != '\0') {nCaracteresParrafo++;}
    }

//    printf("Caracteres max %d", nCaracteresmax);
//    printf("\nEl numero de parrafos es: %d", nParrafos);
//    printf("\nEl num de caracteres es: %d\n", nCaracteresParrafo);
//    printf("\nEl parrafoLargo es: %d\n", parrafoLargo);

    nCaracteresmax++;
//    printf("\nEl num de caracteresmax es: %d\n", nCaracteresmax);

    texto = (char **) malloc(nParrafos * sizeof(char) + 1);
    if (texto == NULL){
        printf("Error1: memoria no disponible.\n");
        exit(-1);
    }
    for(int n=0; n < nParrafos; n++)
    {
        texto[n] = (char *) malloc(nCaracteresmax * sizeof(char) );
        if (texto[n] == NULL){
        printf("Error %d: memoria no disponible.\n", n);
        exit(-1);
    }
    parrafo = (char *) malloc(nCaracteresmax * sizeof(char));

    }
    //printf("\nEl num de caracteresmax es: %d\n", nCaracteresmax);

    int i=0;
    int j=0;
    fseek(descripciones, 0, SEEK_SET);
    while(fscanf(descripciones, "%c", &c) != EOF)
    {
        parrafo[i]=c;
        //printf("El caracter %i - %i es: %c", j, i, c);
        if(c == '\n')
        {
            parrafo[i]= '\0';
            //printf("\nEl parrafo antes de copiar es: %s\n", parrafo);
            strcpy(texto[j], parrafo);
//            printf("\nEl parrafo %i - %i es : %s", j, i, texto[j]);
//            printf("\n\n%s", texto[j]);
            j++;
            i=0;
        }
        i++;
    }
    texto[j]='\0';
    fseek(descripciones, 0, SEEK_SET);

    if(reskar > 0 && reskar < 0.25)
    {
        printf("\n%s", texto[3]);
    }
    else if(reskar > 0.25 && reskar < 0.5)
    {
        printf("\n%s", texto[2]);
    }
    else if(reskar > 0.5 && reskar < 0.75)
    {
        printf("\n%s", texto[1]);
    }
    else if(reskar > 0.75 && reskar < 1)
    {
        printf("\n%s", texto[0]);
    }


}


void guardaLog(char *nombre,int karma){
    FILE *fichLogCartas;

    if(fichLogCartas = fopen("fichLogCartas.csv", "a")){
        fprintf(fichLogCartas,"%s;%i\n",nombre,karma);
        //printf("El nombre es");

    }else{
        perror("Error al abrir el archivo");
    }


}













