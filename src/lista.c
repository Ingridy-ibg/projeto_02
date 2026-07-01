/**
 * @file lista.c
 * @brief Implementação do TAD Lista encadeada genérica (contrato em lista.h).
 */

#include <stdlib.h>

#include "lista.h"

/* Nó da lista. Definições de struct ficam no .c (proibido no .h). */
struct no {
    Item       item;
    struct no *prox;
};

struct lista {
    struct no *inicio;
    struct no *fim;
    int        n;
};

/* -------------------------------------------------------------------------- */
/*  Criação / destruição                                                      */
/* -------------------------------------------------------------------------- */

Lista lista_criar(void)
{
    Lista l = malloc(sizeof(struct lista));
    if (l == NULL) {
        return NULL;
    }
    l->inicio = NULL;
    l->fim    = NULL;
    l->n      = 0;
    return l;
}

void lista_destruir(Lista l, void (*libera_item)(Item))
{
    struct no *atual;
    struct no *prox;

    if (l == NULL) {
        return;
    }
    atual = l->inicio;
    while (atual != NULL) {
        prox = atual->prox;
        if (libera_item != NULL) {
            libera_item(atual->item);
        }
        free(atual);
        atual = prox;
    }
    free(l);
}

/* -------------------------------------------------------------------------- */
/*  Inserção                                                                  */
/* -------------------------------------------------------------------------- */

int lista_inserir_inicio(Lista l, Item it)
{
    struct no *novo = malloc(sizeof(struct no));
    if (novo == NULL) {
        return 0;
    }
    novo->item = it;
    novo->prox = l->inicio;
    l->inicio  = novo;
    if (l->fim == NULL) {     /* lista estava vazia */
        l->fim = novo;
    }
    l->n++;
    return 1;
}

int lista_inserir_fim(Lista l, Item it)
{
    struct no *novo = malloc(sizeof(struct no));
    if (novo == NULL) {
        return 0;
    }
    novo->item = it;
    novo->prox = NULL;
    if (l->fim == NULL) {     /* lista vazia */
        l->inicio = novo;
    } else {
        l->fim->prox = novo;
    }
    l->fim = novo;
    l->n++;
    return 1;
}

/* -------------------------------------------------------------------------- */
/*  Remoção                                                                   */
/* -------------------------------------------------------------------------- */

int lista_remover(Lista l, Item it)
{
    struct no *atual = l->inicio;
    struct no *ant   = NULL;

    while (atual != NULL) {
        if (atual->item == it) {
            /* desencadeia o nó */
            if (ant == NULL) {
                l->inicio = atual->prox;     /* removendo o primeiro */
            } else {
                ant->prox = atual->prox;
            }
            if (atual == l->fim) {
                l->fim = ant;                /* removendo o último */
            }
            free(atual);
            l->n--;
            return 1;
        }
        ant   = atual;
        atual = atual->prox;
    }
    return 0;   /* não encontrado */
}

/* -------------------------------------------------------------------------- */
/*  Consultas                                                                 */
/* -------------------------------------------------------------------------- */

int lista_tamanho(Lista l)
{
    return l->n;
}

int lista_vazia(Lista l)
{
    return l->n == 0;
}

Item *lista_para_vetor(Lista l, int *n_out)
{
    Item      *vet;
    struct no *atual;
    int        i;

    if (l->n == 0) {
        *n_out = 0;
        return NULL;
    }

    vet = malloc((size_t) l->n * sizeof(Item));
    if (vet == NULL) {
        *n_out = 0;
        return NULL;
    }

    i = 0;
    for (atual = l->inicio; atual != NULL; atual = atual->prox) {
        vet[i++] = atual->item;
    }
    *n_out = l->n;
    return vet;
}

void lista_percorrer(Lista l, void (*visita)(Item, void *), void *ctx)
{
    struct no *atual;
    for (atual = l->inicio; atual != NULL; atual = atual->prox) {
        visita(atual->item, ctx);
    }
}