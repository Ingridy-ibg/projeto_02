#include <stdlib.h>

#include "abb.h"

struct no {
    Item       item;
    struct no *esq;
    struct no *dir;
};

struct abb {
    struct no *raiz;
    Comparador cmp;
    int        n;
};

static struct no *criar_no(Item it)
{
    struct no *novo = malloc(sizeof(struct no));
    if (novo != NULL) {
        novo->item = it;
        novo->esq  = NULL;
        novo->dir  = NULL;
    }
    return novo;
}

static struct no *inserir_no(struct no *raiz, Item it, Comparador cmp, int *ok)
{
    if (raiz == NULL) {
        struct no *novo = criar_no(it);
        *ok = (novo != NULL);
        return novo;
    }
    if (cmp(it, raiz->item) < 0) {
        raiz->esq = inserir_no(raiz->esq, it, cmp, ok);
    } else {                       /* igual ou maior vai para a direita */
        raiz->dir = inserir_no(raiz->dir, it, cmp, ok);
    }
    return raiz;
}

/* Menor nó de uma subárvore (mais à esquerda). */
static struct no *min_no(struct no *raiz)
{
    while (raiz->esq != NULL) {
        raiz = raiz->esq;
    }
    return raiz;
}
static struct no *remover_no(struct no *raiz, Item chave, Comparador cmp,
                             Item *removido)
{
    int c;

    if (raiz == NULL) {
        return NULL;                       /* não encontrado */
    }

    c = cmp(chave, raiz->item);
    if (c < 0) {
        raiz->esq = remover_no(raiz->esq, chave, cmp, removido);
        return raiz;
    }
    if (c > 0) {
        raiz->dir = remover_no(raiz->dir, chave, cmp, removido);
        return raiz;
    }

    /* c == 0: este é o nó a remover. */
    *removido = raiz->item;

    if (raiz->esq == NULL) {               /* 0 ou 1 filho (direita) */
        struct no *d = raiz->dir;
        free(raiz);
        return d;
    }
    if (raiz->dir == NULL) {               /* 1 filho (esquerda) */
        struct no *e = raiz->esq;
        free(raiz);
        return e;
    }

    /* 2 filhos: copia o item do sucessor em-ordem (menor da subárvore direita)
       para este nó e remove o sucessor da subárvore direita. */
    {
        struct no *suc = min_no(raiz->dir);
        Item       lixo;                   /* item do sucessor: já copiado */
        raiz->item = suc->item;
        raiz->dir  = remover_no(raiz->dir, suc->item, cmp, &lixo);
    }
    return raiz;
}

static void destruir_no(struct no *raiz, void (*libera_item)(Item))
{
    if (raiz == NULL) {
        return;
    }
    destruir_no(raiz->esq, libera_item);
    destruir_no(raiz->dir, libera_item);
    if (libera_item != NULL) {
        libera_item(raiz->item);
    }
    free(raiz);
}

static void em_ordem_no(struct no *raiz, void (*visita)(Item, void *), void *ctx)
{
    if (raiz == NULL) {
        return;
    }
    em_ordem_no(raiz->esq, visita, ctx);
    visita(raiz->item, ctx);
    em_ordem_no(raiz->dir, visita, ctx);
}

Abb abb_criar(Comparador cmp)
{
    Abb a = malloc(sizeof(struct abb));
    if (a == NULL) {
        return NULL;
    }
    a->raiz = NULL;
    a->cmp  = cmp;
    a->n    = 0;
    return a;
}

void abb_destruir(Abb a, void (*libera_item)(Item))
{
    if (a == NULL) {
        return;
    }
    destruir_no(a->raiz, libera_item);
    free(a);
}

int abb_inserir(Abb a, Item it)
{
    int ok = 0;
    a->raiz = inserir_no(a->raiz, it, a->cmp, &ok);
    if (ok) {
        a->n++;
    }
    return ok;
}

Item abb_buscar(Abb a, Item chave)
{
    struct no *atual = a->raiz;
    while (atual != NULL) {
        int c = a->cmp(chave, atual->item);
        if (c == 0) {
            return atual->item;
        }
        atual = (c < 0) ? atual->esq : atual->dir;
    }
    return NULL;
}

Item abb_remover(Abb a, Item chave)
{
    Item removido = NULL;
    a->raiz = remover_no(a->raiz, chave, a->cmp, &removido);
    if (removido != NULL) {
        a->n--;
    }
    return removido;
}

void abb_percorrer_em_ordem(Abb a, void (*visita)(Item, void *), void *ctx)
{
    em_ordem_no(a->raiz, visita, ctx);
}

int abb_tamanho(Abb a)
{
    return a->n;
}

int abb_vazia(Abb a)
{
    return a->n == 0;
}