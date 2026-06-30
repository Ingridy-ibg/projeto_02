#ifndef LISTA_H
#define LISTA_H

/** @brief Ponteiro opaco para a lista. */
typedef struct lista *Lista;

/** @brief Tipo do elemento armazenado. */
typedef void *Item;

/**
 * @brief Cria uma lista vazia.
 * @return Nova Lista, ou NULL em falha de alocação.
 */
Lista lista_criar(void);

/**
 * @brief Libera a lista.
 *
 * @param l            Lista a destruir.
 * @param libera_item  Destrutor por item, ou NULL para não liberar os itens.
 */
void lista_destruir(Lista l, void (*libera_item)(Item));

/**
 * @brief Insere um item no início da lista.
 * @param l   Lista
 * @param it  Item a inserir.
 * @return 1 em sucesso, 0 em falha de alocação do nó.
 * @pre l != NULL.
 */
int lista_inserir_inicio(Lista l, Item it);

/**
 * @brief Insere um item no fim da lista. 
 * @param l   Lista.
 * @param it  Item a inserir.
 * @return 1 em sucesso, 0 em falha de alocação do nó.
 * @pre l != NULL.
 */
int lista_inserir_fim(Lista l, Item it);

/**
 * @brief Remove a primeira ocorrência de um item.
 * @param l   Lista (não nula).
 * @param it  Item a remover.
 * @return 1 se removeu, 0 se não encontrou.
 * @pre l != NULL.
 */
int lista_remover(Lista l, Item it);

/**
 * @brief Número de itens na lista. 
 * @param l Lista.
 * @pre l != NULL.
 */
int lista_tamanho(Lista l);

/**
 * @brief Indica se a lista está vazia.
 * @param l Lista (não nula).
 * @return 1 se vazia, 0 caso contrário.
 * @pre l != NULL.
 */
int lista_vazia(Lista l);

/**
 * @brief Copia os itens para um vetor recém-alocado, na ordem da lista.
 *
 * @param l      Lista (não nula).
 * @param n_out  Saída: número de itens (= tamanho).
 * @return Vetor de Item (tamanho *n_out), ou NULL se a lista estiver vazia ou
 *         em falha de alocação (nesse caso *n_out = 0).
 * @pre l != NULL; n_out != NULL.
 */
Item *lista_para_vetor(Lista l, int *n_out);

/**
 * @brief Aplica uma função a cada item, na ordem da lista.
 *
 * @param l       Lista (não nula).
 * @param visita  Função chamada como visita(item, ctx) para cada item.
 * @param ctx     Contexto opaco repassado a @p visita (pode ser NULL).
 * @pre l != NULL; visita != NULL.
 */
void lista_percorrer(Lista l, void (*visita)(Item, void *), void *ctx);

#endif /* LISTA_H */