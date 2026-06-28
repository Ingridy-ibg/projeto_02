#ifndef LINHA_H
#define LINHA_H

/**
 * @brief Ponteiro opaco para uma linha.
 */
typedef struct linha *Linha;

/**
 * @brief Cria uma linha.
 *
 * @param id   Identificador inteiro da forma.
 * @param x1   x da primeira extremidade.
 * @param y1   y da primeira extremidade.
 * @param x2   x da segunda extremidade.
 * @param y2   y da segunda extremidade.
 * @param cor  Cor da linha.
 *
 * @return Nova Linha alocada dinamicamente.
 *
 * @pre cor != NULL; id >=0;
 */
Linha linha_criar(int id, double x1, double y1, double x2, double y2,
                  const char *cor);

/**
 * @brief Cria uma cópia independente de uma linha, com novo identificador.
 *
 * @param l        Linha a clonar.
 * @param novo_id  Identificador do clone.
 * @return Nova Linha idêntica a @p l (exceto pelo id), ou NULL em falha.
 * @pre l != NULL.
 */
Linha linha_clonar(Linha l, int novo_id);

/**
 * @brief Libera toda a memória associada a uma linha.
 *
 * @param l Linha a destruir (pode ser NULL).
 */
void linha_destruir(Linha l);


/** @brief Identificador da linha. @pre l != NULL. */
int linha_get_id(Linha l);

/** @brief x da primeira extremidade (âncora de referência). @pre l != NULL. */
double linha_get_x1(Linha l);

/** @brief y da primeira extremidade (âncora de referência). @pre l != NULL. */
double linha_get_y1(Linha l);

/** @brief x da segunda extremidade. @pre l != NULL. */
double linha_get_x2(Linha l);

/** @brief y da segunda extremidade. @pre l != NULL. */
double linha_get_y2(Linha l);

/**
 * @brief Retorna a cor da linha.
 *
 * @param l Linha.
 * @pre l != NULL.
 */
const char *linha_get_cor(Linha l);

/**
 * @brief Comprimento euclidiano da linha: sqrt((x2-x1)^2 + (y2-y1)^2).
 * @param l Linha (não nula).
 * @pre l != NULL.
 */
double linha_comprimento(Linha l);

/**
 * @brief Largura da linha, definida como o seu comprimento.
 *
 * @param l Linha (não nula).
 * @pre l != NULL.
 */
double linha_largura(Linha l);

/**
 * @brief Altura da linha: constante 1.5 (definida na descrição do projeto).
 * @param l Linha.
 * @pre l != NULL.
 */
double linha_altura(Linha l);

/**
 * @brief Área da linha: 1.5 * comprimento.
 * @param l Linha.
 * @pre l != NULL.
 */
double linha_area(Linha l);

/**
 * @brief Desloca a linha por (dx, dy), transladando ambas as extremidades.
 *
 * @param l   Linha.
 * @param dx  Deslocamento no eixo x.
 * @param dy  Deslocamento no eixo y.
 * @pre l != NULL.
 */
void linha_mover(Linha l, double dx, double dy);

/**
 * @brief Altera a cor da linha.
 *
 * @param l    Linha.
 * @param cor  Nova cor
 * @pre l != NULL; cor != NULL.
 */
void linha_set_cor(Linha l, const char *cor);

#endif /* LINHA_H */