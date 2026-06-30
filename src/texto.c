/**
 * @file texto.c
 * @brief Implementação do TAD Texto (contrato em texto.h).
 */

#include <stdlib.h>
#include <string.h>

#include "texto.h"

/* Definição concreta do tipo. Fica no .c (proibido struct no .h). */
struct texto {
    int    id;
    double x;          /* x da âncora */
    double y;          /* y da âncora */
    char   ancora;     /* 'i', 'm' ou 'f' */
    char  *corb;       /* cor da borda */
    char  *corp;       /* cor de preenchimento */
    char  *conteudo;   /* texto exibido */
    char  *familia;    /* família da fonte */
    char  *peso;       /* peso da fonte */
    char  *tamanho;    /* tamanho da fonte (string) */
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

Texto texto_criar(int id, double x, double y,
                  const char *corb, const char *corp,
                  char ancora, const char *conteudo,
                  const char *familia, const char *peso, const char *tamanho)
{
    Texto t = malloc(sizeof(struct texto));
    if (t == NULL) {
        return NULL;
    }

    t->id     = id;
    t->x      = x;
    t->y      = y;
    t->ancora = ancora;

    /* Inicializa os ponteiros antes de duplicar, para que texto_destruir
       possa limpar com segurança (free(NULL)) em caso de falha parcial. */
    t->corb     = dup_str(corb);
    t->corp     = dup_str(corp);
    t->conteudo = dup_str(conteudo);
    t->familia  = dup_str(familia);
    t->peso     = dup_str(peso);
    t->tamanho  = dup_str(tamanho);

    if (t->corb == NULL || t->corp == NULL || t->conteudo == NULL ||
        t->familia == NULL || t->peso == NULL || t->tamanho == NULL) {
        texto_destruir(t);   /* libera o que tiver sido alocado */
        return NULL;
    }
    return t;
}

Texto texto_clonar(Texto t, int novo_id)
{
    if (t == NULL) {
        return NULL;
    }
    return texto_criar(novo_id, t->x, t->y, t->corb, t->corp, t->ancora,
                       t->conteudo, t->familia, t->peso, t->tamanho);
}

void texto_destruir(Texto t)
{
    if (t == NULL) {
        return;
    }
    free(t->corb);
    free(t->corp);
    free(t->conteudo);
    free(t->familia);
    free(t->peso);
    free(t->tamanho);
    free(t);
}

/* -------------------------------------------------------------------------- */
/*  Consultas (getters)                                                       */
/* -------------------------------------------------------------------------- */

int texto_get_id(Texto t)
{
    return t->id;
}

double texto_get_x(Texto t)
{
    return t->x;
}

double texto_get_y(Texto t)
{
    return t->y;
}

const char *texto_get_cor_borda(Texto t)
{
    return t->corb;
}

const char *texto_get_cor_preenchimento(Texto t)
{
    return t->corp;
}

char texto_get_ancora(Texto t)
{
    return t->ancora;
}

const char *texto_get_conteudo(Texto t)
{
    return t->conteudo;
}

const char *texto_get_familia(Texto t)
{
    return t->familia;
}

const char *texto_get_peso(Texto t)
{
    return t->peso;
}

const char *texto_get_tamanho(Texto t)
{
    return t->tamanho;
}

/* -------------------------------------------------------------------------- */
/*  Atributos derivados                                                       */
/* -------------------------------------------------------------------------- */

int texto_num_caracteres(Texto t)
{
    return (int) strlen(t->conteudo);
}

double texto_largura(Texto t)
{
    return 1.0 * texto_num_caracteres(t);
}

double texto_altura(Texto t)
{
    (void) t;            /* altura é constante; parâmetro mantido por contrato */
    return 10.0;
}

double texto_area(Texto t)
{
    return 10.0 * texto_num_caracteres(t);
}

/* -------------------------------------------------------------------------- */
/*  Modificações                                                              */
/* -------------------------------------------------------------------------- */

void texto_mover(Texto t, double dx, double dy)
{
    t->x += dx;
    t->y += dy;
}

void texto_set_cores(Texto t, const char *corb, const char *corp)
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

    free(t->corb);
    t->corb = nova_borda;

    free(t->corp);
    t->corp = nova_preench;
}