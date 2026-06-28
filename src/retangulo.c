#include <stdlib.h>
#include <string.h>

#include "retangulo.h"

struct retangulo {
    int    id;
    double x;     /* x do canto inferior esquerdo (âncora) */
    double y;     /* y do canto inferior esquerdo (âncora) */
    double w;     /* largura */
    double h;     /* altura  */
    char  *corb;  /* cor da borda */
    char  *corp;  /* cor de preenchimento */
};

/* Duplica uma string em memória própria. Substitui strdup() para não depender
   de macros POSIX sob -std=c99. Retorna NULL se s for NULL ou em falha. */
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

Retangulo retangulo_criar(int id, double x, double y, double w, double h,
                          const char *corb, const char *corp)
{
    Retangulo r = malloc(sizeof(struct retangulo));
    if (r == NULL) {
        return NULL;
    }

    r->id = id;
    r->x  = x;
    r->y  = y;
    r->w  = w;
    r->h  = h;
    r->corb = dup_str(corb);
    r->corp = dup_str(corp);

    /* Se qualquer cópia de cor falhou, desfaz tudo para não vazar memória. */
    if (r->corb == NULL || r->corp == NULL) {
        free(r->corb);
        free(r->corp);
        free(r);
        return NULL;
    }
    return r;
}

Retangulo retangulo_clonar(Retangulo r, int novo_id)
{
    if (r == NULL) {
        return NULL;
    }
    return retangulo_criar(novo_id, r->x, r->y, r->w, r->h, r->corb, r->corp);
}

void retangulo_destruir(Retangulo r)
{
    if (r == NULL) {
        return;
    }
    free(r->corb);
    free(r->corp);
    free(r);
}


int retangulo_get_id(Retangulo r)
{
    return r->id;
}

double retangulo_get_x(Retangulo r)
{
    return r->x;
}

double retangulo_get_y(Retangulo r)
{
    return r->y;
}

double retangulo_get_largura(Retangulo r)
{
    return r->w;
}

double retangulo_get_altura(Retangulo r)
{
    return r->h;
}

const char *retangulo_get_cor_borda(Retangulo r)
{
    return r->corb;
}

const char *retangulo_get_cor_preenchimento(Retangulo r)
{
    return r->corp;
}


double retangulo_area(Retangulo r)
{
    return r->w * r->h;
}

void retangulo_mover(Retangulo r, double dx, double dy)
{
    r->x += dx;
    r->y += dy;
}

void retangulo_set_cores(Retangulo r, const char *corb, const char *corp)
{
    char *nova_borda   = dup_str(corb);
    char *nova_preench = dup_str(corp);

    /* Só troca se ambas as cópias deram certo; caso contrário, mantém o
       estado anterior intacto e não vaza as novas alocações. */
    if (nova_borda == NULL || nova_preench == NULL) {
        free(nova_borda);
        free(nova_preench);
        return;
    }

    free(r->corb);
    r->corb = nova_borda;

    free(r->corp);
    r->corp = nova_preench;
}