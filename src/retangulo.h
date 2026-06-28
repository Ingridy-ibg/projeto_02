#ifndef RETANGULO_H
#define RETANGULO_H

/**
 * @brief Ponteiro opaco para um retângulo.
 */
typedef struct retangulo *Retangulo;

/**
 * @brief Cria um retângulo.
 *
 * @param id    Identificador inteiro da forma.
 * @param x     x do canto inferior esquerdo (âncora).
 * @param y     y do canto inferior esquerdo (âncora).
 * @param w     Largura.
 * @param h     Altura.
 * @param corb  Cor da borda
 * @param corp  Cor de preenchimento
 *
 * @return Novo Retangulo alocado dinamicamente, ou NULL em falha de alocação.
 *
 * @pre w > 0; h > 0; corb != NULL; corp != NULL.
 */
Retangulo retangulo_criar(int id, double x, double y, double w, double h,
                          const char *corb, const char *corp);

/**
 * @brief Cria uma cópia independente de um retângulo, com novo identificador.
 *
 * @param r        Retângulo a clonar.
 * @param novo_id  Identificador do clone.
 * @return Novo Retangulo idêntico a @p r, ou NULL em falha.
 * @pre r != NULL.
 */
Retangulo retangulo_clonar(Retangulo r, int novo_id);

/**
 * @brief Libera toda a memória associada a um retângulo.
 * @param r Retângulo a destruir.
 */
void retangulo_destruir(Retangulo r);


/** @brief Identificador do retângulo. @pre r != NULL. */
int retangulo_get_id(Retangulo r);

/** @brief x do canto inferior esquerdo (âncora). @pre r != NULL. */
double retangulo_get_x(Retangulo r);

/** @brief y do canto inferior esquerdo (âncora). @pre r != NULL. */
double retangulo_get_y(Retangulo r);

/**
 * @brief Largura.
 * @pre r != NULL.
 */
double retangulo_get_largura(Retangulo r);

/**
 * @brief Altura.
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
 * @pre r != NULL.
 */
const char *retangulo_get_cor_preenchimento(Retangulo r);


/**
 * @brief Área do retângulo: altura * largura.
 * @param r Retângulo.
 * @pre r != NULL.
 */
double retangulo_area(Retangulo r);


/**
 * @brief Desloca o canto âncora do retângulo por (dx, dy).
 * @param r   Retângulo.
 * @param dx  Deslocamento no eixo x.
 * @param dy  Deslocamento no eixo y.
 * @pre r != NULL.
 */
void retangulo_mover(Retangulo r, double dx, double dy);

/**
 * @brief Altera as cores do retângulo.
 * @param r     Retângulo.
 * @param corb  Nova cor da borda. 
 * @param corp  Nova cor de preenchimento.
 * @pre r != NULL; corb != NULL; corp != NULL.
 */
void retangulo_set_cores(Retangulo r, const char *corb, const char *corp);

#endif /* RETANGULO_H */