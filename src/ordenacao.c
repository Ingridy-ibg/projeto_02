/**
 * @file ordenacao.c
 * @brief Implementação dos algoritmos de ordenação (contrato em ordenacao.h).
 */

#include <stdlib.h>
#include <string.h>

#include "ordenacao.h"

/* Troca dois itens do vetor. */
static void troca(Item *vet, int i, int j)
{
    Item tmp = vet[i];
    vet[i]   = vet[j];
    vet[j]   = tmp;
}

/* Dispara o snapshot, se houver. */
static void snapshot(FnSnapshot snap, Item *vet, int n, void *ctx)
{
    if (snap != NULL) {
        snap(vet, n, ctx);
    }
}

/* -------------------------------------------------------------------------- */
/*  Selection Sort                                                            */
/* -------------------------------------------------------------------------- */

void ordenacao_selection(Item *vet, int n, Comparador cmp,
                         FnSnapshot snap, void *ctx)
{
    int i, j, menor;
    for (i = 0; i < n - 1; i++) {
        menor = i;
        for (j = i + 1; j < n; j++) {
            if (cmp(vet[j], vet[menor]) < 0) {
                menor = j;
            }
        }
        if (menor != i) {
            troca(vet, i, menor);
        }
        snapshot(snap, vet, n, ctx);   /* uma iteração = um elemento fixado */
    }
}

/* -------------------------------------------------------------------------- */
/*  Bubble Sort                                                               */
/* -------------------------------------------------------------------------- */

void ordenacao_bubble(Item *vet, int n, Comparador cmp,
                      FnSnapshot snap, void *ctx)
{
    int i, j, trocou;
    for (i = 0; i < n - 1; i++) {
        trocou = 0;
        for (j = 0; j < n - 1 - i; j++) {
            if (cmp(vet[j + 1], vet[j]) < 0) {
                troca(vet, j, j + 1);
                trocou = 1;
            }
        }
        snapshot(snap, vet, n, ctx);
        if (!trocou) {          /* já ordenado: encerra cedo */
            break;
        }
    }
}

/* -------------------------------------------------------------------------- */
/*  Insertion Sort                                                            */
/* -------------------------------------------------------------------------- */

void ordenacao_insertion(Item *vet, int n, Comparador cmp,
                         FnSnapshot snap, void *ctx)
{
    int  i, j;
    Item chave;
    for (i = 1; i < n; i++) {
        chave = vet[i];
        j = i - 1;
        while (j >= 0 && cmp(chave, vet[j]) < 0) {
            vet[j + 1] = vet[j];
            j--;
        }
        vet[j + 1] = chave;
        snapshot(snap, vet, n, ctx);
    }
}

/* -------------------------------------------------------------------------- */
/*  Shell Sort                                                                */
/* -------------------------------------------------------------------------- */

void ordenacao_shell(Item *vet, int n, Comparador cmp,
                     FnSnapshot snap, void *ctx)
{
    int  gap, i, j;
    Item tmp;
    for (gap = n / 2; gap > 0; gap /= 2) {
        for (i = gap; i < n; i++) {
            tmp = vet[i];
            j = i;
            while (j >= gap && cmp(tmp, vet[j - gap]) < 0) {
                vet[j] = vet[j - gap];
                j -= gap;
            }
            vet[j] = tmp;
        }
        snapshot(snap, vet, n, ctx);   /* uma iteração = um gap concluído */
    }
}

/* -------------------------------------------------------------------------- */
/*  Quick Sort (pivô mediana-de-três, partição de Lomuto)                     */
/* -------------------------------------------------------------------------- */

/* Coloca a mediana de vet[lo], vet[mid], vet[hi] em vet[hi] (pivô). */
static void mediana_de_tres(Item *vet, int lo, int hi, Comparador cmp)
{
    int mid = lo + (hi - lo) / 2;
    if (cmp(vet[mid], vet[lo]) < 0) troca(vet, lo, mid);
    if (cmp(vet[hi], vet[lo]) < 0) troca(vet, lo, hi);
    if (cmp(vet[hi], vet[mid]) < 0) troca(vet, mid, hi);
    /* agora vet[lo] <= vet[mid] <= vet[hi]; move a mediana para o fim */
    troca(vet, mid, hi);
}

static int particiona(Item *vet, int lo, int hi, Comparador cmp)
{
    Item pivo = vet[hi];
    int  i = lo - 1;
    int  j;
    for (j = lo; j < hi; j++) {
        if (cmp(vet[j], pivo) <= 0) {
            i++;
            troca(vet, i, j);
        }
    }
    troca(vet, i + 1, hi);
    return i + 1;
}

static void quick_rec(Item *vet, int lo, int hi, Comparador cmp,
                      FnSnapshot snap, int n, void *ctx)
{
    if (lo < hi) {
        int p;
        if (hi - lo >= 2) {
            mediana_de_tres(vet, lo, hi, cmp);
        }
        p = particiona(vet, lo, hi, cmp);
        snapshot(snap, vet, n, ctx);   /* uma iteração = uma partição */
        quick_rec(vet, lo, p - 1, cmp, snap, n, ctx);
        quick_rec(vet, p + 1, hi, cmp, snap, n, ctx);
    }
}

void ordenacao_quick(Item *vet, int n, Comparador cmp,
                     FnSnapshot snap, void *ctx)
{
    quick_rec(vet, 0, n - 1, cmp, snap, n, ctx);
}

/* -------------------------------------------------------------------------- */
/*  Merge Sort                                                                */
/* -------------------------------------------------------------------------- */

/* Intercala vet[lo..mid] e vet[mid+1..hi] usando o buffer aux. */
static void intercala(Item *vet, Item *aux, int lo, int mid, int hi,
                      Comparador cmp)
{
    int i = lo, j = mid + 1, k;

    for (k = lo; k <= hi; k++) {   /* copia a faixa para aux */
        aux[k] = vet[k];
    }
    for (k = lo; k <= hi; k++) {
        if (i > mid) {
            vet[k] = aux[j++];
        } else if (j > hi) {
            vet[k] = aux[i++];
        } else if (cmp(aux[j], aux[i]) < 0) {
            vet[k] = aux[j++];
        } else {
            vet[k] = aux[i++];
        }
    }
}

static void merge_rec(Item *vet, Item *aux, int lo, int hi, Comparador cmp,
                      FnSnapshot snap, int n, void *ctx)
{
    int mid;
    if (lo >= hi) {
        return;
    }
    mid = lo + (hi - lo) / 2;
    merge_rec(vet, aux, lo, mid, cmp, snap, n, ctx);
    merge_rec(vet, aux, mid + 1, hi, cmp, snap, n, ctx);
    intercala(vet, aux, lo, mid, hi, cmp);
    snapshot(snap, vet, n, ctx);   /* uma iteração = uma intercalação */
}

void ordenacao_merge(Item *vet, int n, Comparador cmp,
                     FnSnapshot snap, void *ctx)
{
    Item *aux;
    if (n < 2) {
        return;
    }
    aux = malloc((size_t) n * sizeof(Item));
    if (aux == NULL) {
        return;   /* sem memória: não ordena (vetor permanece intacto) */
    }
    merge_rec(vet, aux, 0, n - 1, cmp, snap, n, ctx);
    free(aux);
}

/* -------------------------------------------------------------------------- */
/*  Despachante                                                               */
/* -------------------------------------------------------------------------- */

int ordenar(const char *alg, Item *vet, int n, Comparador cmp,
            FnSnapshot snap, void *ctx)
{
    if      (strcmp(alg, "ss")  == 0) ordenacao_selection(vet, n, cmp, snap, ctx);
    else if (strcmp(alg, "bs")  == 0) ordenacao_bubble(vet, n, cmp, snap, ctx);
    else if (strcmp(alg, "is")  == 0) ordenacao_insertion(vet, n, cmp, snap, ctx);
    else if (strcmp(alg, "shs") == 0) ordenacao_shell(vet, n, cmp, snap, ctx);
    else if (strcmp(alg, "qs")  == 0) ordenacao_quick(vet, n, cmp, snap, ctx);
    else if (strcmp(alg, "ms")  == 0) ordenacao_merge(vet, n, cmp, snap, ctx);
    else return 0;   /* código inválido */
    return 1;
}