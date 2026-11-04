#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>

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
    {"ES", 1, 6850, 1760, 1430, 2020, 1080, 290},
    {"FR", 2, 36640, 3850, 4240, 1670, 2860, 24020},
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

    if (pais == NULL)
    {
        fprintf(stderr, "Error al buscar el pais");
        return;
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

    char archivo[20];

    sprintf(archivo, "encuestas_%s.csv", nombrep);

    FILE *f = fopen(archivo, "w");

    if (f == NULL)
    {
        perror("fopen");
        return;
    }

    fprintf(f, "ID_encuesta,ID_pais,Pais,Sector,ID_sector,Sentimiento\n");

    int contador = 1;

    for (int i = 0; i < maxIn; i++)
    {
        double sentimiento = ((float)rand() / (float)RAND_MAX) * 100;
        fprintf(f, "%d,%d,%s,Industria,0,%.2f\n", contador, pais->id, pais->nombre, sentimiento);
        contador++;
    }

    for (int i = 0; i < maxSe; i++)
    {
        double sentimiento = ((float)rand() / (float)RAND_MAX) * 100;
        fprintf(f, "%d,%d,%s,Servicios,1,%.2f\n", contador, pais->id, pais->nombre, sentimiento);
        contador++;
    }

    for (int i = 0; i < maxConsu; i++)
    {
        double sentimiento = ((float)rand() / (float)RAND_MAX) * 100;
        fprintf(f, "%d,%d,%s,Consumidores,2,%.2f\n", contador, pais->id, pais->nombre, sentimiento);
        contador++;
    }

    for (int i = 0; i < maxCom; i++)
    {
        double sentimiento = ((float)rand() / (float)RAND_MAX) * 100;
        fprintf(f, "%d,%d,%s,Comercio,3,%.2f\n", contador, pais->id, pais->nombre, sentimiento);
        contador++;
    }

    for (int i = 0; i < maxConst; i++)
    {
        double sentimiento = ((float)rand() / (float)RAND_MAX) * 100;
        fprintf(f, "%d,%d,%s,Construccion,4,%.2f\n", contador, pais->id, pais->nombre, sentimiento);
        contador++;
    }

    fclose(f);
}

int main(int argc, char *argv[])

{

    srand(time(NULL));

    if (argc != 5)
    {
        fprintf(stderr, "Uso: %s -p <pais> -e porcentaje \n", argv[0]);
        return 1;
    }

    int opt;

    // Variables para guardar los resultados
    double e_value = 0;
    char *p_value = NULL;

    // "e:p:" es el string de formato.
    // Indica que buscamos las flags 'e' y 'p'.
    // Los dos puntos (:) después de la letra indican que ESA flag requiere un argumento.
    while ((opt = getopt(argc, argv, "e:p:")) != -1)
    {
        switch (opt)
        {
        case 'e':
            // optarg es una variable global de getopt que contiene el valor de la flag actual
            char *fin;
            errno = 0;
            e_value = strtod(optarg, &fin); // 10 = base decimal

            if (errno != 0 || fin == optarg || *fin != '\0')
            {
                fprintf(stderr, "'%s' no es un número válido\n", argv[1]);
                return 1;
            }

            break;
        case 'p':
            p_value = optarg;
            break;
        case '?':
            // getopt devuelve '?' si falta un argumento o si se usa una flag desconocida
            fprintf(stderr, "Uso correcto: %s -e <numero> -p <texto>\n", argv[0]);
            return 1;
        default:
            abort();
        }
    }

    // Imprimimos para verificar que se leyeron correctamente
    printf("Valor de -e: %f\n", e_value);

    if (p_value != NULL)
    {
        printf("Valor de -p: %s\n", p_value);
    }
    else
    {
        printf("La flag -p no fue proporcionada.\n");
    }

    if (e_value < 0 || e_value > 100)
    {
        fprintf(stderr, "Error cuarto argumento. Uso: porcentaje (0-100)\n");
        return 1;
    }

    float porcentaje = e_value / 100.0;
    generar_CSV(p_value, porcentaje);
}