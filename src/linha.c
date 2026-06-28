/**
 * @file linha.c
 * @brief Implementação do TAD Linha (contrato em linha.h).
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "linha.h"

/* Definição concreta do tipo. Fica no .c (proibido struct no .h). */
struct linha {
    int    id;
    double x1;
    double y1;
    double x2;
    double y2;
    char  *cor;   /* cor própria (string alocada) */
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

/* -------------------------------------------------------------------------- */
/*  Criação / clonagem / destruição                                           */
/* -------------------------------------------------------------------------- */

Linha linha_criar(int id, double x1, double y1, double x2, double y2,
                  const char *cor)
{
    Linha l = malloc(sizeof(struct linha));
    if (l == NULL) {
        return NULL;
    }

    l->id = id;
    l->x1 = x1;
    l->y1 = y1;
    l->x2 = x2;
    l->y2 = y2;
    l->cor = dup_str(cor);

    if (l->cor == NULL) {
        free(l);
        return NULL;
    }
    return l;
}

Linha linha_clonar(Linha l, int novo_id)
{
    if (l == NULL) {
        return NULL;
    }
    return linha_criar(novo_id, l->x1, l->y1, l->x2, l->y2, l->cor);
}

void linha_destruir(Linha l)
{
    if (l == NULL) {
        return;
    }
    free(l->cor);
    free(l);
}

/* -------------------------------------------------------------------------- */
/*  Consultas (getters)                                                       */
/* -------------------------------------------------------------------------- */

int linha_get_id(Linha l)
{
    return l->id;
}

double linha_get_x1(Linha l)
{
    return l->x1;
}

double linha_get_y1(Linha l)
{
    return l->y1;
}

double linha_get_x2(Linha l)
{
    return l->x2;
}

double linha_get_y2(Linha l)
{
    return l->y2;
}

const char *linha_get_cor(Linha l)
{
    return l->cor;
}

/* -------------------------------------------------------------------------- */
/*  Atributos derivados                                                       */
/* -------------------------------------------------------------------------- */

double linha_comprimento(Linha l)
{
    double dx = l->x2 - l->x1;
    double dy = l->y2 - l->y1;
    return sqrt(dx * dx + dy * dy);
}

double linha_largura(Linha l)
{
    return linha_comprimento(l);
}

double linha_altura(Linha l)
{
    (void) l;            /* altura é constante; parâmetro mantido por contrato */
    return 1.5;
}

double linha_area(Linha l)
{
    return 1.5 * linha_comprimento(l);
}

/* -------------------------------------------------------------------------- */
/*  Modificações                                                              */
/* -------------------------------------------------------------------------- */

void linha_mover(Linha l, double dx, double dy)
{
    l->x1 += dx;
    l->y1 += dy;
    l->x2 += dx;
    l->y2 += dy;
}

void linha_set_cor(Linha l, const char *cor)
{
    char *nova = dup_str(cor);

    /* Só troca se a cópia deu certo; caso contrário, mantém o estado anterior. */
    if (nova == NULL) {
        return;
    }
    free(l->cor);
    l->cor = nova;
}