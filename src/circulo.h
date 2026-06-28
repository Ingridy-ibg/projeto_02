#ifndef CIRCULO_H
#define CIRCULO_H

/**
 * @file circulo.h
 * @brief TAD Círculo.
 *
 * Representa um círculo do plano cartesiano, conforme o comando .geo:
 *
 *     c i x y r corb corp
 *
 * onde:
 *   - i        : identificador inteiro (>= 1) da forma;
 *   - (x, y)   : coordenada âncora do círculo, que é o seu CENTRO;
 *   - r        : raio (> 0);
 *   - corb     : cor da borda    (string no padrão de cores SVG);
 *   - corp     : cor de preenchimento (string no padrão de cores SVG).
 *
 * Este é um Tipo Abstrato de Dados (TAD) OPACO: a definição da struct fica
 * no arquivo de implementação (circulo.c). O usuário do módulo manipula
 * apenas o ponteiro Circulo e nunca acessa os campos diretamente.
 *
 * Convenção de propriedade (ownership):
 *   - as strings de cor passadas em circulo_criar() são COPIADAS internamente;
 *     o chamador continua dono dos buffers que passou;
 *   - as strings devolvidas pelos getters de cor pertencem ao próprio círculo
 *     e são válidas enquanto o círculo existir; NÃO devem ser liberadas nem
 *     modificadas pelo chamador;
 *   - todo Circulo criado deve ser liberado com circulo_destruir().
 */

/**
 * @brief Ponteiro opaco para um círculo.
 *
 * A struct subjacente é definida apenas em circulo.c (contrato: nenhuma
 * struct é definida neste cabeçalho).
 */
typedef struct circulo *Circulo;

/**
 * @brief Cria um círculo.
 *
 * @param id    Identificador inteiro da forma (>= 1).
 * @param x     Coordenada x do centro.
 * @param y     Coordenada y do centro.
 * @param r     Raio do círculo (deve ser > 0).
 * @param corb  Cor da borda (string não nula, padrão SVG). É copiada.
 * @param corp  Cor de preenchimento (string não nula, padrão SVG). É copiada.
 *
 * @return Novo Circulo alocado dinamicamente, ou NULL em caso de falha de
 *         alocação. O chamador é responsável por liberá-lo com
 *         circulo_destruir().
 *
 * @pre  r > 0; corb != NULL; corp != NULL.
 */
Circulo circulo_criar(int id, double x, double y, double r,
                      const char *corb, const char *corp);

/**
 * @brief Cria uma cópia independente de um círculo, com um novo identificador.
 *
 * Útil para o comando de clonagem (cm). A cópia possui suas próprias strings
 * de cor; alterações em uma não afetam a outra.
 *
 * @param c        Círculo a ser clonado (não nulo).
 * @param novo_id  Identificador a ser atribuído ao clone (>= 1).
 *
 * @return Novo Circulo idêntico a @p c (exceto pelo id), ou NULL em falha.
 *
 * @pre c != NULL.
 */
Circulo circulo_clonar(Circulo c, int novo_id);

/**
 * @brief Libera toda a memória associada a um círculo.
 *
 * Após a chamada, o ponteiro @p c não deve mais ser usado. Chamar com NULL
 * é seguro (no-op).
 *
 * @param c Círculo a ser destruído (pode ser NULL).
 */
void circulo_destruir(Circulo c);

/* -------------------------------------------------------------------------- */
/*  Consultas (getters)                                                       */
/* -------------------------------------------------------------------------- */

/**
 * @brief Retorna o identificador do círculo.
 * @param c Círculo (não nulo).
 * @return Identificador inteiro.
 * @pre c != NULL.
 */
int circulo_get_id(Circulo c);

/**
 * @brief Retorna a coordenada x do centro (âncora).
 * @param c Círculo (não nulo).
 * @pre c != NULL.
 */
double circulo_get_x(Circulo c);

/**
 * @brief Retorna a coordenada y do centro (âncora).
 * @param c Círculo (não nulo).
 * @pre c != NULL.
 */
double circulo_get_y(Circulo c);

/**
 * @brief Retorna o raio do círculo.
 * @param c Círculo (não nulo).
 * @pre c != NULL.
 */
double circulo_get_raio(Circulo c);

/**
 * @brief Retorna a cor da borda.
 *
 * A string retornada pertence ao círculo; não liberar nem modificar.
 *
 * @param c Círculo (não nulo).
 * @return Ponteiro para a string da cor da borda.
 * @pre c != NULL.
 */
const char *circulo_get_cor_borda(Circulo c);

/**
 * @brief Retorna a cor de preenchimento.
 *
 * A string retornada pertence ao círculo; não liberar nem modificar.
 *
 * @param c Círculo (não nulo).
 * @return Ponteiro para a string da cor de preenchimento.
 * @pre c != NULL.
 */
const char *circulo_get_cor_preenchimento(Circulo c);

/* -------------------------------------------------------------------------- */
/*  Atributos derivados (para os critérios de ordenação)                      */
/* -------------------------------------------------------------------------- */

/**
 * @brief Área do círculo: pi * r^2.
 * @param c Círculo (não nulo).
 * @pre c != NULL.
 */
double circulo_area(Circulo c);

/**
 * @brief Largura do círculo, definida como o diâmetro (2 * r).
 *
 * Corresponde à largura da caixa delimitadora (bounding box), usada pelo
 * critério de ordenação por largura (w).
 *
 * @param c Círculo (não nulo).
 * @pre c != NULL.
 */
double circulo_largura(Circulo c);

/**
 * @brief Altura do círculo, definida como o diâmetro (2 * r).
 *
 * Corresponde à altura da caixa delimitadora (bounding box), usada pelo
 * critério de ordenação por altura (h).
 *
 * @param c Círculo (não nulo).
 * @pre c != NULL.
 */
double circulo_altura(Circulo c);

/* -------------------------------------------------------------------------- */
/*  Modificações (setters)                                                    */
/* -------------------------------------------------------------------------- */

/**
 * @brief Desloca o centro do círculo por (dx, dy).
 *
 * Usado, por exemplo, na clonagem com movimento (comando cm).
 *
 * @param c   Círculo (não nulo).
 * @param dx  Deslocamento no eixo x.
 * @param dy  Deslocamento no eixo y.
 * @pre c != NULL.
 */
void circulo_mover(Circulo c, double dx, double dy);

/**
 * @brief Altera as cores do círculo (comando mc).
 *
 * As novas strings são copiadas internamente; o chamador continua dono dos
 * buffers passados. As cores anteriores são liberadas.
 *
 * @param c     Círculo (não nulo).
 * @param corb  Nova cor da borda (não nula). É copiada.
 * @param corp  Nova cor de preenchimento (não nula). É copiada.
 * @pre c != NULL; corb != NULL; corp != NULL.
 */
void circulo_set_cores(Circulo c, const char *corb, const char *corp);

#endif /* CIRCULO_H */