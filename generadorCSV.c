#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <math.h>

typedef struct
{
    char *nombre;
    int id;
    int max_encuestas;
    int Industria;
    int Servicios;
    int Consumidores;
    int Comercio;
    int Construccion;
} Pais;

static const Pais PAISES[] = {
    {"FR", 1, 6850, 1760, 1430, 2020, 1080, 290},
    {"ES", 2, 36640, 3850, 4240, 1670, 2860, 24020},
    {"IT", 3, 13100, 5440, 3100, 2000, 1560, 1000}};

const Pais *buscar_pais(const char *nombre)
{
    for (size_t i = 0; i < sizeof(PAISES) / sizeof(PAISES[0]); i++)
    {
        if (strcmp(PAISES[i].nombre, nombre) == 0)
            return &PAISES[i];
    }
    return NULL;
}

void generar_CSV(char *nombrep, float porcentaje)
{

    printf("Porcentaje %f\n", porcentaje);
    const Pais *pais = buscar_pais(nombrep);

    printf("Prueba pais: %s %d %d %d %d \n", pais->nombre, pais->id, pais->max_encuestas, pais->Industria, pais->Comercio);
    printf("Prueba pocentaje: %f\n", porcentaje);

    if (pais == NULL)
    {
        fprintf(stderr, "Error al buscar el pais");
    }

    int maxIn = round(pais->Industria * porcentaje);
    int maxSe = round(pais->Servicios * porcentaje);
    int maxConsu = round(pais->Consumidores * porcentaje);
    int maxCom = round(pais->Comercio * porcentaje);
    int maxConst = round(pais->Construccion * porcentaje);

    int total = maxIn + maxSe + maxConsu + maxCom + maxConst;

    printf("Generando fichero encuestas_%s.csv para pais %s con %f de encuestas\n", pais->nombre, pais->nombre, porcentaje);
    printf("Maximos por sector: \n");

    printf("Industria: %d/%d encuestas (%f del maximo) \n", maxIn, pais->Industria, porcentaje);
    printf("Servicios: %d/%d encuestas (%f del maximo) \n", maxSe, pais->Servicios, porcentaje);
    printf("Consumidores: %d/%d encuestas (%f del maximo) \n", maxConsu, pais->Consumidores, porcentaje);
    printf("Comercio: %d/%d encuestas (%f del maximo) \n", maxCom, pais->Comercio, porcentaje);
    printf("Construccion: %d/%d encuestas (%f del maximo) \n", maxConst, pais->Construccion, porcentaje);

    printf("Total de encuestas: %d\n", total);

    FILE *f = fopen("encuestas_FR.csv", "w");

    if (f == NULL)
    {
        perror("fopen");
        return;
    }

    printf("Escribiendo cabecera\n");

    fprintf(f, "ID_encuesta,ID_pais,Pais,Sector,ID_sector,Sentimiento\n");

    int contador = 1;

    for (int i = 0; i < maxIn; i++)
    {
        int sentimiento = rand() % 100 + 0;
        fprintf(f, "%d,%d,%s,Industria,0,%d\n", contador, pais->id, pais->nombre, sentimiento);
        contador++;
    }

    for (int i = 0; i < maxSe; i++)
    {
        int sentimiento = rand() % 100 + 0;
        fprintf(f, "%d,%d,%s,Servicios,0,%d\n", contador, pais->id, pais->nombre, sentimiento);
        contador++;
    }

    for (int i = 0; i < maxConsu; i++)
    {
        int sentimiento = rand() % 100 + 0;
        fprintf(f, "%d,%d,%s,Consumidores,0,%d\n", contador, pais->id, pais->nombre, sentimiento);
        contador++;
    }

    for (int i = 0; i < maxCom; i++)
    {
        int sentimiento = rand() % 100 + 0;
        fprintf(f, "%d,%d,%s,Comercio,0,%d\n", contador, pais->id, pais->nombre, sentimiento);
        contador++;
    }

    for (int i = 0; i < maxConst; i++)
    {
        int sentimiento = rand() % 100 + 0;
        fprintf(f, "%d,%d,%s,Construccion,0,%d\n", contador, pais->id, pais->nombre, sentimiento);
        contador++;
    }
}

int main(int argc, char *argv[])

{

    srand(time(NULL));

    if (argc != 5)
    {
        fprintf(stderr, "Uso: %s -p <pais> -e porcentaje \n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-p") != 0)
    {
        fprintf(stderr, "Error primer argumento. Uso: %s -p <pais> -e porcentaje \n", argv[0]);
        return 1;
    }

    if (strcmp(argv[2], "ES") != 0 && strcmp(argv[2], "IT") != 0 && strcmp(argv[2], "FR") != 0)
    {
        fprintf(stderr, "Error segundo argumento. Uso: ES - IT - FR \n");
        return 1;
    }

    if (strcmp(argv[3], "-e") != 0)
    {
        fprintf(stderr, "Error tercer argumento. Uso: %s -p <pais> -e porcentaje \n", argv[0]);
        return 1;
    }

    char *fin;
    errno = 0;
    long n = strtol(argv[4], &fin, 10); // 10 = base decimal

    if (errno != 0 || fin == argv[4] || *fin != '\0')
    {
        fprintf(stderr, "'%s' no es un número válido\n", argv[1]);
        return 1;
    }

    if (n < 0 || n > 100)
    {
        fprintf(stderr, "Error cuarto argumento. Uso: porcentaje (0-100)");
        return 1;
    }

    // Verificacion pais

    printf("Porcentaje puro: %d\n", n);
    float porcentaje = n / 100.0;

    printf("Calculo porcentaje: %f\n", porcentaje);
    printf("Nombre pais: %s\n", argv[2]);
    generar_CSV(argv[2], porcentaje);
}