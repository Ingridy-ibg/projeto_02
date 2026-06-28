#ifndef CIRCULO_H
#define CIRCULO_H

/**
 * @brief Ponteiro opaco para um círculo.
 */
typedef struct circulo *Circulo;

/**
 * @brief Cria um círculo.
 *
 * @param id    Identificador inteiro da forma.
 * @param x     Coordenada x do centro.
 * @param y     Coordenada y do centro.
 * @param r     Raio do círculo.
 * @param corb  Cor da borda.
 * @param corp  Cor de preenchimento.
 *
 * @return Novo Circulo alocado dinamicamente, ou NULL em caso de falha de
 *         alocação. O chamador é responsável por liberá-lo com
 *         circulo_destruir().
 *
 * @pre  id maior que 1; r maior que 0; cores nao nulas.
 * 
 */
Circulo circulo_criar(int id, double x, double y, double r,
                      const char *corb, const char *corp);

/**
 * @brief Cria uma cópia independente de um círculo, com um novo identificador.
 *
 * @param c        Círculo a ser clonado.
 * @param novo_id  Identificador a ser atribuído ao clone.
 *
 * @return Novo Circulo idêntico a c, ou NULL em falha.
 *
 * @pre c != NULL.
 */
Circulo circulo_clonar(Circulo c, int novo_id);

/**
 * @brief Libera toda a memória associada a um círculo.
 *
 * @param c Círculo a ser destruído.
 */
void circulo_destruir(Circulo c);

/**
 * @brief Retorna o id do círculo.
 * @param c Círculo.
 * @pre c != NULL.
 */
int circulo_get_id(Circulo c);

/**
 * @brief Retorna a coordenada x do centro (âncora).
 * @param c Círculo.
 * @pre c != NULL.
 */
double circulo_get_x(Circulo c);

/**
 * @brief Retorna a coordenada y do centro (âncora).
 * @param c Círculo.
 * @pre c != NULL.
 */
double circulo_get_y(Circulo c);

/**
 * @brief Retorna o raio do círculo.
 * @param c Círculo.
 * @pre c != NULL.
 */
double circulo_get_raio(Circulo c);

/**
 * @brief Retorna a cor da borda.
 *
 * @param c Círculo.
 * @return Ponteiro para a string da cor da borda.
 * @pre c != NULL.
 */
const char *circulo_get_cor_borda(Circulo c);

/**
 * @brief Retorna a cor de preenchimento.
 *
 * @param c Círculo.
 * @return Ponteiro para a string da cor de preenchimento.
 * @pre c != NULL.
 */
const char *circulo_get_cor_preenchimento(Circulo c);

/**
 * @brief Área do círculo: pi * r^2.
 * @param c Círculo.
 * @pre c != NULL.
 */
double circulo_area(Circulo c);

/**
 * @brief Diâmetro do círculo, definido como 2 * r.
 *
 * @param c Círculo (não nulo).
 * @pre c != NULL.
 */
double circulo_diametro(Circulo c);

/**
 * @brief Desloca o centro do círculo por (dx, dy).
 *
 * @param c   Círculo.
 * @param dx  Deslocamento no eixo x.
 * @param dy  Deslocamento no eixo y.
 * @pre c != NULL.
 */
void circulo_mover(Circulo c, double dx, double dy);

/**
 * @brief Altera as cores do círculo.
 *
 * @param c     Círculo.
 * @param corb  Nova cor da borda.
 * @param corp  Nova cor de preenchimento.
 * @pre c != NULL; corb != NULL; corp != NULL.
 */
void circulo_set_cores(Circulo c, const char *corb, const char *corp);

#endif /* CIRCULO_H */