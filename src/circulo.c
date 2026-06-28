
#include <stdlib.h>
#include <string.h>

#include "circulo.h"

#define PI 3.14159265358979323846

struct circulo {
    int    id;
    double x;     /* x do centro (âncora) */
    double y;     /* y do centro (âncora) */
    double r;     /* raio */
    char  *corb;  /* cor da borda */
    char  *corp;  /* cor de preenchimento*/
};

/* Duplica uma string em memória própria. Substitui strdup() para não depender
   de macros POSIX sob -std=c99. Retorna NULL se s for NULL ou em falha de
   alocação. */
static char *dup_str(const char *s)
{
    size_t n;
    char  *novo;

    if (s == NULL) {
        return NULL;
    }
    n = strlen(s) + 1;
    novo = malloc(n);
    if (novo != NULL) {
        memcpy(novo, s, n);
    }
    return novo;
}


Circulo circulo_criar(int id, double x, double y, double r,
                      const char *corb, const char *corp)
{
    Circulo c = malloc(sizeof(struct circulo));
    if (c == NULL) {
        return NULL;
    }

    c->id = id;
    c->x  = x;
    c->y  = y;
    c->r  = r;
    c->corb = dup_str(corb);
    c->corp = dup_str(corp);

    /* Se qualquer cópia de cor falhou, desfaz tudo para não vazar memória. */
    if (c->corb == NULL || c->corp == NULL) {
        free(c->corb);
        free(c->corp);
        free(c);
        return NULL;
    }
    return c;
}

Circulo circulo_clonar(Circulo c, int novo_id)
{
    if (c == NULL) {
        return NULL;
    }
    return circulo_criar(novo_id, c->x, c->y, c->r, c->corb, c->corp);
}

void circulo_destruir(Circulo c)
{
    if (c == NULL) {
        return;
    }
    free(c->corb);
    free(c->corp);
    free(c);
}

int circulo_get_id(Circulo c)
{
    return c->id;
}

double circulo_get_x(Circulo c)
{
    return c->x;
}

double circulo_get_y(Circulo c)
{
    return c->y;
}

double circulo_get_raio(Circulo c)
{
    return c->r;
}

const char *circulo_get_cor_borda(Circulo c)
{
    return c->corb;
}

const char *circulo_get_cor_preenchimento(Circulo c)
{
    return c->corp;
}

/* -------------------------------------------------------------------------- */
/*  Atributos derivados                                                       */
/* -------------------------------------------------------------------------- */

double circulo_area(Circulo c)
{
    return PI * c->r * c->r;
}

double circulo_diametro(Circulo c)
{
    return 2.0 * c->r;  
}



void circulo_mover(Circulo c, double dx, double dy)
{
    c->x += dx;
    c->y += dy;
}

void circulo_set_cores(Circulo c, const char *corb, const char *corp)
{
    char *nova_borda    = dup_str(corb);
    char *nova_preench  = dup_str(corp);

    /* Só troca se ambas as cópias deram certo; caso contrário, mantém o
       estado anterior intacto e não vaza as novas alocações. */
    if (nova_borda == NULL || nova_preench == NULL) {
        free(nova_borda);
        free(nova_preench);
        return;
    }

    free(c->corb);
    c->corb = nova_borda;

    free(c->corp);
    c->corp = nova_preench;
}