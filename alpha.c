#include <stdio.h>

int main()
{
    int num;
    char letra;
    printf("Seleccione una de las sigientes opciones:\n a. Salud\n b.Dinero\n c.Amor\n e.Salir\n");
    scanf ("%c", &letra);
    
    switch(letra)
    {
    case 'a':
    case 'A':
         printf("HA SELECCIONADO LA OPCION SALUD.\n");
         printf("Pulse un numero si desea volver al menu principal. \n");
         scanf("%d", &num);
         break;
    
    case 'b':
    case 'B':
         printf("HA SELECCIONADO LA OPCION DINERO.\n");
         printf("Pulse un numero si desea volver al menu principal. \n");
         scanf("%d", &num);
         break;
    
    case 'c':
    case 'C':
         printf("HA SELECCIONADO LA OPCION AMOR.\n");
         printf("Pulse un numero si desea volver al menu principal. \n");
         scanf("%d", &num);
         break;
    
    case 'e':
    case 'E':
         printf("SALIR\n");
         break;
         
    default:
        printf("La opcion seleccionada no existe");
        printf("Pulse un numero si desea volver al menu principal. \n");
        scanf("%d", &num);
        break;
    }
    return 0;
}
