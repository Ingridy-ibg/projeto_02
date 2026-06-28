#ifndef LINHA_H
#define LINHA_H

/**
 * @file linha.h
 * @brief TAD Linha.
 *
 * Representa um segmento de reta do plano cartesiano, conforme o comando .geo:
 *
 *     l i x1 y1 x2 y2 cor
 *
 * onde:
 *   - i              : identificador inteiro (>= 1) da forma;
 *   - (x1, y1)       : primeira extremidade;
 *   - (x2, y2)       : segunda extremidade;
 *   - cor            : cor da linha (string no padrão de cores SVG).
 *
 * Diferenças importantes em relação a círculo/retângulo:
 *   - a linha tem APENAS UMA cor (não há borda e preenchimento separados);
 *   - a linha é definida por DUAS âncoras (suas extremidades). Para fins de
 *     ordenação default e de posicionamento (comandos find/findrm), adota-se a
 *     PRIMEIRA extremidade (x1, y1) como âncora de referência.
 *
 * Convenções de ordenação (definidas no enunciado):
 *   - altura da linha     : constante 1.5;
 *   - largura da linha    : igual ao seu comprimento (ver linha_largura);
 *   - área da linha       : 1.5 * comprimento  (= altura * largura);
 *   - na ordenação por cor (c), usa-se a cor da linha.
 *
 * Este é um TAD OPACO: a struct é definida apenas em linha.c.
 *
 * Propriedade (ownership):
 *   - a string de cor passada é COPIADA internamente; o chamador continua dono
 *     do buffer original;
 *   - a string devolvida por linha_get_cor pertence à linha; não liberar nem
 *     modificar;
 *   - toda Linha criada deve ser liberada com linha_destruir().
 */

/**
 * @brief Ponteiro opaco para uma linha.
 *
 * A struct subjacente é definida apenas em linha.c (contrato: nenhuma struct
 * é definida neste cabeçalho).
 */
typedef struct linha *Linha;

/**
 * @brief Cria uma linha.
 *
 * @param id   Identificador inteiro da forma (>= 1).
 * @param x1   x da primeira extremidade.
 * @param y1   y da primeira extremidade.
 * @param x2   x da segunda extremidade.
 * @param y2   y da segunda extremidade.
 * @param cor  Cor da linha (string não nula, padrão SVG). É copiada.
 *
 * @return Nova Linha alocada dinamicamente, ou NULL em falha de alocação.
 *         O chamador deve liberá-la com linha_destruir().
 *
 * @pre cor != NULL.
 */
Linha linha_criar(int id, double x1, double y1, double x2, double y2,
                  const char *cor);

/**
 * @brief Cria uma cópia independente de uma linha, com novo identificador.
 *
 * Útil para o comando de clonagem (cm). A cópia possui sua própria string de
 * cor; alterações em uma não afetam a outra.
 *
 * @param l        Linha a clonar (não nula).
 * @param novo_id  Identificador do clone (>= 1).
 * @return Nova Linha idêntica a @p l (exceto pelo id), ou NULL em falha.
 * @pre l != NULL.
 */
Linha linha_clonar(Linha l, int novo_id);

/**
 * @brief Libera toda a memória associada a uma linha.
 *
 * Após a chamada, @p l não deve mais ser usado. Chamar com NULL é seguro.
 *
 * @param l Linha a destruir (pode ser NULL).
 */
void linha_destruir(Linha l);

/* -------------------------------------------------------------------------- */
/*  Consultas (getters)                                                       */
/* -------------------------------------------------------------------------- */

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
 * A string retornada pertence à linha; não liberar nem modificar.
 *
 * @param l Linha (não nula).
 * @pre l != NULL.
 */
const char *linha_get_cor(Linha l);

/* -------------------------------------------------------------------------- */
/*  Atributos derivados (para os critérios de ordenação)                      */
/* -------------------------------------------------------------------------- */

/**
 * @brief Comprimento euclidiano da linha: sqrt((x2-x1)^2 + (y2-y1)^2).
 * @param l Linha (não nula).
 * @pre l != NULL.
 */
double linha_comprimento(Linha l);

/**
 * @brief Largura da linha, definida como o seu comprimento.
 *
 * Modela a linha como uma faixa fina de altura 1.5: assim
 * area = largura * altura = comprimento * 1.5, coerente com a definição
 * de área do enunciado. Usada pelo critério de ordenação por largura (w).
 *
 * @param l Linha (não nula).
 * @pre l != NULL.
 */
double linha_largura(Linha l);

/**
 * @brief Altura da linha: constante 1.5 (definição do enunciado).
 * @param l Linha (não nula).
 * @pre l != NULL.
 */
double linha_altura(Linha l);

/**
 * @brief Área da linha: 1.5 * comprimento (definição do enunciado).
 * @param l Linha (não nula).
 * @pre l != NULL.
 */
double linha_area(Linha l);

/* -------------------------------------------------------------------------- */
/*  Modificações                                                              */
/* -------------------------------------------------------------------------- */

/**
 * @brief Desloca a linha por (dx, dy), transladando AMBAS as extremidades.
 *
 * O comprimento é invariante ao deslocamento. Usado na clonagem com movimento
 * (cm) e no reposicionamento de find/findrm.
 *
 * @param l   Linha (não nula).
 * @param dx  Deslocamento no eixo x.
 * @param dy  Deslocamento no eixo y.
 * @pre l != NULL.
 */
void linha_mover(Linha l, double dx, double dy);

/**
 * @brief Altera a cor da linha (comando mc).
 *
 * A nova string é copiada; a cor anterior é liberada. Como a linha tem uma
 * única cor, no comando mc deve-se passar a cor de borda (corb).
 *
 * @param l    Linha (não nula).
 * @param cor  Nova cor (não nula). É copiada.
 * @pre l != NULL; cor != NULL.
 */
void linha_set_cor(Linha l, const char *cor);

#endif /* LINHA_H */