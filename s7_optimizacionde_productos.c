/*
 * ISWZ1102 - Programación 1
 * Optimización de Producción en una Fábrica de Componentes Electrónicos
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 5          /* cinco productos distintos */
#define LARGO 40       /* largo maximo del nombre */

/* Opcion 1: ingresar un producto */
void ingresar(char nombres[][LARGO], double *tiempos, double *recursos,
              int *demandas, int *cantidad) {
    if (*cantidad >= MAX) {
        printf("Ya se ingresaron los %d productos permitidos.\n", MAX);
        return;
    }
    printf("Nombre del producto: ");
    scanf(" %39[^\n]", nombres[*cantidad]);

    printf("Tiempo de fabricacion por unidad (min): ");
    while (scanf("%lf", tiempos + *cantidad) != 1 || *(tiempos + *cantidad) <= 0) {
        while (getchar() != '\n');
        printf("Valor invalido. Ingrese un numero mayor a 0: ");
    }
    printf("Recursos requeridos por unidad: ");
    while (scanf("%lf", recursos + *cantidad) != 1 || *(recursos + *cantidad) <= 0) {
        while (getchar() != '\n');
        printf("Valor invalido. Ingrese un numero mayor a 0: ");
    }
    printf("Cantidad demandada (unidades): ");
    while (scanf("%d", demandas + *cantidad) != 1 || *(demandas + *cantidad) < 0) {
        while (getchar() != '\n');
        printf("Valor invalido. Ingrese un entero mayor o igual a 0: ");
    }
    (*cantidad)++;
    printf("Producto registrado.\n");
}

/* Opcion 2: listar los productos */
void listar(char nombres[][LARGO], double *tiempos, double *recursos,
            int *demandas, int cantidad) {
    int i;
    if (cantidad == 0) {
        printf("No hay productos registrados.\n");
        return;
    }
    printf("\n%-3s %-25s %12s %12s %10s\n", "#", "Producto", "Tiempo/u", "Recursos/u", "Demanda");
    for (i = 0; i < cantidad; i++) {
        printf("%-3d %-25s %12.2f %12.2f %10d\n", i + 1, nombres[i],
               *(tiempos + i), *(recursos + i), *(demandas + i));
    }
}

/* Opcion 3: editar un producto (se busca por nombre) */
void editar(char nombres[][LARGO], double *tiempos, double *recursos,
            int *demandas, int cantidad) {
    char clave[LARGO];
    int i, j, pos = -1, opcion;

    if (cantidad == 0) {
        printf("No hay productos registrados.\n");
        return;
    }
    printf("Nombre del producto a editar: ");
    scanf(" %39[^\n]", clave);

    /* Busqueda lineal propia */
    for (i = 0; i < cantidad && pos == -1; i++) {
        j = 0;
        while (nombres[i][j] != '\0' && nombres[i][j] == clave[j]) j++;
        if (nombres[i][j] == clave[j]) pos = i;
    }
    if (pos == -1) {
        printf("Producto no encontrado.\n");
        return;
    }

    do {
        printf("\nEditando '%s'\n", nombres[pos]);
        printf("1. Nombre\n2. Tiempo por unidad\n3. Recursos por unidad\n");
        printf("4. Cantidad demandada\n0. Terminar\nOpcion: ");
        while (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            printf("Opcion invalida: ");
        }
        switch (opcion) {
            case 1:
                printf("Nuevo nombre: ");
                scanf(" %39[^\n]", nombres[pos]);
                break;
            case 2:
                printf("Nuevo tiempo por unidad: ");
                while (scanf("%lf", tiempos + pos) != 1 || *(tiempos + pos) <= 0) {
                    while (getchar() != '\n');
                    printf("Valor invalido: ");
                }
                break;
            case 3:
                printf("Nuevos recursos por unidad: ");
                while (scanf("%lf", recursos + pos) != 1 || *(recursos + pos) <= 0) {
                    while (getchar() != '\n');
                    printf("Valor invalido: ");
                }
                break;
            case 4:
                printf("Nueva demanda: ");
                while (scanf("%d", demandas + pos) != 1 || *(demandas + pos) < 0) {
                    while (getchar() != '\n');
                    printf("Valor invalido: ");
                }
                break;
            case 0:
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 0);
}

/* Opcion 4: eliminar un producto (se busca por nombre) */
void eliminar(char nombres[][LARGO], double *tiempos, double *recursos,
              int *demandas, int *cantidad) {
    char clave[LARGO];
    int i, j, pos = -1;

    if (*cantidad == 0) {
        printf("No hay productos registrados.\n");
        return;
    }
    printf("Nombre del producto a eliminar: ");
    scanf(" %39[^\n]", clave);

    /* Busqueda lineal propia */
    for (i = 0; i < *cantidad && pos == -1; i++) {
        j = 0;
        while (nombres[i][j] != '\0' && nombres[i][j] == clave[j]) j++;
        if (nombres[i][j] == clave[j]) pos = i;
    }
    if (pos == -1) {
        printf("Producto no encontrado.\n");
        return;
    }

    /* Se corren los datos una posicion a la izquierda */
    for (i = pos; i < *cantidad - 1; i++) {
        strcpy(nombres[i], nombres[i + 1]);
        *(tiempos + i)  = *(tiempos + i + 1);
        *(recursos + i) = *(recursos + i + 1);
        *(demandas + i) = *(demandas + i + 1);
    }
    (*cantidad)--;
    printf("Producto eliminado.\n");
}

/* Opcion 5: calcular tiempo total, recursos totales y factibilidad */
void analizar(char nombres[][LARGO], double *tiempos, double *recursos,
              int *demandas, int cantidad) {
    double tiempo_total = 0, recursos_total = 0;
    double tiempo_disp, recursos_disp;
    int i;

    if (cantidad == 0) {
        printf("No hay productos registrados.\n");
        return;
    }
    printf("Tiempo de produccion disponible (min): ");
    while (scanf("%lf", &tiempo_disp) != 1 || tiempo_disp <= 0) {
        while (getchar() != '\n');
        printf("Valor invalido: ");
    }
    printf("Recursos disponibles: ");
    while (scanf("%lf", &recursos_disp) != 1 || recursos_disp <= 0) {
        while (getchar() != '\n');
        printf("Valor invalido: ");
    }

    printf("\n%-25s %14s %14s\n", "Producto", "Tiempo (min)", "Recursos");
    for (i = 0; i < cantidad; i++) {
        printf("%-25s %14.2f %14.2f\n", nombres[i],
               *(tiempos + i) * *(demandas + i),
               *(recursos + i) * *(demandas + i));
        tiempo_total   += *(tiempos + i)  * *(demandas + i);
        recursos_total += *(recursos + i) * *(demandas + i);
    }
    printf("\nTiempo total de fabricacion: %.2f min (%.2f horas)\n", tiempo_total, tiempo_total / 60);
    printf("Recursos totales necesarios: %.2f\n", recursos_total);

    if (tiempo_total <= tiempo_disp && recursos_total <= recursos_disp) {
        printf("\nSI se puede cumplir con la demanda.\n");
        printf("Sobran %.2f min y %.2f de recursos.\n",
               tiempo_disp - tiempo_total, recursos_disp - recursos_total);
    } else {
        printf("\nNO se puede cumplir con la demanda.\n");
        if (tiempo_total > tiempo_disp)
            printf("Faltan %.2f min de tiempo.\n", tiempo_total - tiempo_disp);
        if (recursos_total > recursos_disp)
            printf("Faltan %.2f de recursos.\n", recursos_total - recursos_disp);
    }
}

int main(void) {
    char nombres[MAX][LARGO];
    double tiempos[MAX];     /* minutos por unidad */
    double recursos[MAX];    /* recursos por unidad */
    int demandas[MAX];       /* unidades demandadas */
    int cantidad = 0;
    int opcion;

    do {
        system("cls");
        printf("===== FABRICA DE COMPONENTES ELECTRONICOS =====\n");
        printf("1. Ingresar producto\n");
        printf("2. Listar productos\n");
        printf("3. Editar producto\n");
        printf("4. Eliminar producto\n");
        printf("5. Calcular produccion y factibilidad\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        while (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            printf("Opcion invalida: ");
        }

        switch (opcion) {
            case 1: ingresar(nombres, tiempos, recursos, demandas, &cantidad); break;
            case 2: listar(nombres, tiempos, recursos, demandas, cantidad); break;
            case 3: editar(nombres, tiempos, recursos, demandas, cantidad); break;
            case 4: eliminar(nombres, tiempos, recursos, demandas, &cantidad); break;
            case 5: analizar(nombres, tiempos, recursos, demandas, cantidad); break;
            case 0: printf("Hasta luego.\n"); break;
            default: printf("Opcion invalida.\n");
        }
        if (opcion != 0) system("pause");
    } while (opcion != 0);

    return 0;
}
