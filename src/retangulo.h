#ifndef RETANGULO_H
#define RETANGULO_H

/**
 * @file retangulo.h
 * @brief TAD Retângulo.
 *
 * Representa um retângulo do plano cartesiano, conforme o comando .geo:
 *
 *     r i x y w h corb corp
 *
 * onde:
 *   - i        : identificador inteiro (>= 1) da forma;
 *   - (x, y)   : coordenada âncora, que é o CANTO INFERIOR ESQUERDO;
 *   - w        : largura (> 0);
 *   - h        : altura (> 0);
 *   - corb     : cor da borda    (string no padrão de cores SVG);
 *   - corp     : cor de preenchimento (string no padrão de cores SVG).
 *
 * Atributos de ordenação:
 *   - largura : w;
 *   - altura  : h;
 *   - área    : w * h.
 *
 * Este é um TAD OPACO: a struct é definida apenas em retangulo.c.
 *
 * Propriedade (ownership):
 *   - as strings de cor passadas são COPIADAS internamente; o chamador
 *     continua dono dos buffers originais;
 *   - as strings devolvidas pelos getters pertencem ao retângulo; não liberar
 *     nem modificar;
 *   - todo Retangulo criado deve ser liberado com retangulo_destruir().
 */

/**
 * @brief Ponteiro opaco para um retângulo.
 *
 * A struct subjacente é definida apenas em retangulo.c (contrato: nenhuma
 * struct é definida neste cabeçalho).
 */
typedef struct retangulo *Retangulo;

/**
 * @brief Cria um retângulo.
 *
 * @param id    Identificador inteiro da forma (>= 1).
 * @param x     x do canto inferior esquerdo (âncora).
 * @param y     y do canto inferior esquerdo (âncora).
 * @param w     Largura (deve ser > 0).
 * @param h     Altura (deve ser > 0).
 * @param corb  Cor da borda (string não nula, padrão SVG). É copiada.
 * @param corp  Cor de preenchimento (string não nula, padrão SVG). É copiada.
 *
 * @return Novo Retangulo alocado dinamicamente, ou NULL em falha de alocação.
 *         O chamador deve liberá-lo com retangulo_destruir().
 *
 * @pre w > 0; h > 0; corb != NULL; corp != NULL.
 */
Retangulo retangulo_criar(int id, double x, double y, double w, double h,
                          const char *corb, const char *corp);

/**
 * @brief Cria uma cópia independente de um retângulo, com novo identificador.
 *
 * Útil para o comando de clonagem (cm). A cópia possui suas próprias strings
 * de cor; alterações em uma não afetam a outra.
 *
 * @param r        Retângulo a clonar (não nulo).
 * @param novo_id  Identificador do clone (>= 1).
 * @return Novo Retangulo idêntico a @p r (exceto pelo id), ou NULL em falha.
 * @pre r != NULL.
 */
Retangulo retangulo_clonar(Retangulo r, int novo_id);

/**
 * @brief Libera toda a memória associada a um retângulo.
 *
 * Após a chamada, @p r não deve mais ser usado. Chamar com NULL é seguro.
 *
 * @param r Retângulo a destruir (pode ser NULL).
 */
void retangulo_destruir(Retangulo r);

/* -------------------------------------------------------------------------- */
/*  Consultas (getters)                                                       */
/* -------------------------------------------------------------------------- */

/** @brief Identificador do retângulo. @pre r != NULL. */
int retangulo_get_id(Retangulo r);

/** @brief x do canto inferior esquerdo (âncora). @pre r != NULL. */
double retangulo_get_x(Retangulo r);

/** @brief y do canto inferior esquerdo (âncora). @pre r != NULL. */
double retangulo_get_y(Retangulo r);

/**
 * @brief Largura (w). Também é o atributo do critério de ordenação por largura.
 * @pre r != NULL.
 */
double retangulo_get_largura(Retangulo r);

/**
 * @brief Altura (h). Também é o atributo do critério de ordenação por altura.
 * @pre r != NULL.
 */
double retangulo_get_altura(Retangulo r);

/**
 * @brief Retorna a cor da borda.
 *
 * A string retornada pertence ao retângulo; não liberar nem modificar.
 * @pre r != NULL.
 */
const char *retangulo_get_cor_borda(Retangulo r);

/**
 * @brief Retorna a cor de preenchimento.
 *
 * A string retornada pertence ao retângulo; não liberar nem modificar.
 * @pre r != NULL.
 */
const char *retangulo_get_cor_preenchimento(Retangulo r);

/* -------------------------------------------------------------------------- */
/*  Atributos derivados                                                       */
/* -------------------------------------------------------------------------- */

/**
 * @brief Área do retângulo: w * h.
 * @param r Retângulo (não nulo).
 * @pre r != NULL.
 */
double retangulo_area(Retangulo r);

/* -------------------------------------------------------------------------- */
/*  Modificações                                                              */
/* -------------------------------------------------------------------------- */

/**
 * @brief Desloca o canto âncora do retângulo por (dx, dy).
 *
 * Usado na clonagem com movimento (cm) e no reposicionamento de find/findrm.
 *
 * @param r   Retângulo (não nulo).
 * @param dx  Deslocamento no eixo x.
 * @param dy  Deslocamento no eixo y.
 * @pre r != NULL.
 */
void retangulo_mover(Retangulo r, double dx, double dy);

/**
 * @brief Altera as cores do retângulo (comando mc).
 *
 * As novas strings são copiadas; as cores anteriores são liberadas.
 *
 * @param r     Retângulo (não nulo).
 * @param corb  Nova cor da borda (não nula). É copiada.
 * @param corp  Nova cor de preenchimento (não nula). É copiada.
 * @pre r != NULL; corb != NULL; corp != NULL.
 */
void retangulo_set_cores(Retangulo r, const char *corb, const char *corp);

#endif /* RETANGULO_H */