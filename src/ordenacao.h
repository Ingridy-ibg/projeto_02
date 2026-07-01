#ifndef ORDENACAO_H
#define ORDENACAO_H

/** @brief Tipo do elemento ordenado. */
typedef void *Item;

/**
 * @brief Comparador total: < 0 se a precede b; 0 se equivalentes; > 0 caso contrário.
 */
typedef int (*Comparador)(Item a, Item b);

/**
 * @brief Callback de snapshot chamado a cada iteração do algoritmo.
 * @param vet  Vetor no estado atual.
 * @param n    Número de elementos.
 * @param ctx  Contexto opaco do chamador.
 */

 //anotacao pessoal: essa funcao aqui é importante pra animação.
typedef void (*FnSnapshot)(Item *vet, int n, void *ctx);

/*Selection Sort*/
void ordenacao_selection(Item *vet, int n, Comparador cmp, FnSnapshot snap, void *ctx);

/*Bubble Sort*/
void ordenacao_bubble(Item *vet, int n, Comparador cmp, FnSnapshot snap, void *ctx);

/* insertion sort*/
void ordenacao_insertion(Item *vet, int n, Comparador cmp, FnSnapshot snap, void *ctx);

/*shell sort*/
void ordenacao_shell(Item *vet, int n, Comparador cmp,  FnSnapshot snap, void *ctx);

/*quick sort*/void ordenacao_quick(Item *vet, int n, Comparador cmp, FnSnapshot snap, void *ctx);

/*merge sort*/
void ordenacao_merge(Item *vet, int n, Comparador cmp, FnSnapshot snap, void *ctx);

/**
 * @brief Ordena usando o algoritmo indicado pelo código.
 *
 * @param alg   Código: "ss", "bs", "is", "shs", "qs" ou "ms".
 * @param vet   Vetor a ordenar.
 * @param n     Número de elementos.
 * @param cmp   Comparador.
 * @param snap  Callback de snapshot, ou NULL.
 * @param ctx   Contexto opaco do snapshot.
 * @return 1 se o código foi reconhecido e a ordenação executada; 0 caso o
 *         código seja inválido (nesse caso o vetor não é alterado).
 * @pre cmp != NULL.
 */
int ordenar(const char *alg, Item *vet, int n, Comparador cmp,
            FnSnapshot snap, void *ctx);

#endif /* ORDENACAO_H */