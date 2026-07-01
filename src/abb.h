#ifndef ABB_H
#define ABB_H

/* "abb" é arvore binaria de busca*/

/** @brief Ponteiro opaco para a árvore. */
typedef struct abb *Abb;

/** @brief Tipo do elemento armazenado. */
typedef void *Item;

/**
 * @brief Comparador total entre dois itens.
 */
typedef int (*Comparador)(Item a, Item b);

/**
 * @brief Cria uma ABB vazia com o comparador dado.
 * @param cmp Comparador de chave.
 * @return Nova Abb.
 * @pre cmp != NULL.
 */
Abb abb_criar(Comparador cmp);

/**
 * @brief Libera a árvore.
 *
 * @param a            Árvore.
 * @param libera_item  Destrutor por item, ou NULL.
 */
void abb_destruir(Abb a, void (*libera_item)(Item));

/**
 * @brief Insere um item, posicionando-o pela chave.
 *
 * @param a   Árvore.
 * @param it  Item a inserir.
 * @return 1 em sucesso, 0 em falha de alocação.
 * @pre a != NULL.
 */
int abb_inserir(Abb a, Item it);

/**
 * @brief Busca o item cuja chave é igual à de chave.
 * @param a      Árvore.
 * @param chave  Item usado apenas como chave de comparação.
 * @return O item armazenado equivalente, ou NULL se não existir.
 * @pre a != NULL.
 */
Item abb_buscar(Abb a, Item chave);

/**
 * @brief Remove da árvore o item cuja chave é igual a da chave.
 *
 * @param a      Árvore (não nula).
 * @param chave  Item usado apenas como chave de comparação.
 * @return O item removido, ou NULL se não encontrado.
 * @pre a != NULL.
 */
Item abb_remover(Abb a, Item chave);

/**
 * @brief Percorre a árvore em-ordem, aplicando visita a cada item.
 *
 * @param a       Árvore.
 * @param visita  Função chamada como visita(item, ctx).
 * @param ctx     Contexto opaco repassado a @p visita.
 * @pre a != NULL; visita != NULL.
 */
void abb_percorrer_em_ordem(Abb a, void (*visita)(Item, void *), void *ctx);

/**
 * @brief Número de itens na árvore.
 * @param a Árvore.
 * @pre a != NULL.
 */
int abb_tamanho(Abb a);

/**
 * @brief Indica se a árvore está vazia.
 * @param a Árvore.
 * @return 1 se vazia, 0 caso contrário.
 * @pre a != NULL.
 */
int abb_vazia(Abb a);

#endif /* ABB_H */