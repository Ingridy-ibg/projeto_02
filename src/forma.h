#ifndef FORMA_H
#define FORMA_H

/** @brief Ponteiro opaco para uma forma genérica. */
typedef struct forma *Forma;

/**
 * Etiqueta do tipo concreto de uma forma.
 */
typedef enum {
    TIPO_CIRCULO,
    TIPO_RETANGULO,
    TIPO_LINHA,
    TIPO_TEXTO
} TipoForma;

/**
 * @brief trata uma forma concreta como uma Forma genérica.
 *
 * @param tipo        Tipo da forma concreta.
 * @param especifica  Ponteiro para a forma concreta (Circulo, Retangulo,
 *                    Linha ou Texto), não nulo. A Forma passa a ser dona dele.
 *
 * @return Nova Forma, ou NULL em falha de alocação (neste caso, @p especifica
 *         NÃO é liberada; a posse permanece com o chamador).
 *
 * @pre especifica != NULL e compatível com @p tipo.
 */
Forma forma_envolver(TipoForma tipo, void *especifica);

/**
 * @brief Cria uma cópia independente da forma, com novo identificador.
 *
 * @param f        Forma a clonar.
 * @param novo_id  Identificador do clone.
 * @return Nova Forma independente, ou NULL em falha.
 * @pre f != NULL.
 */
Forma forma_clonar(Forma f, int novo_id);

/**
 * @brief Libera a Forma e a forma concreta interna.
 * @param f Forma a destruir.
 */
void forma_destruir(Forma f);

/** @brief Tipo concreto da forma. @pre f != NULL. */
TipoForma forma_get_tipo(Forma f);

/**
 * @brief Ponteiro para a forma concreta interna.
 * @param f Forma (não nula).
 * @pre f != NULL.
 */
void *forma_get_especifica(Forma f);

/** @brief Identificador da forma. @pre f != NULL. */
int forma_get_id(Forma f);

/**
 * @brief x da âncora de referência da forma.
 * @pre f != NULL.
 */
double forma_get_x(Forma f);

/** @brief y da âncora de referência.
 *  @pre f != NULL. */
double forma_get_y(Forma f);

/**
 * @brief Área da forma, segundo o critério de ordenação por área.
 * @pre f != NULL.
 */
double forma_area(Forma f);

/** @brief Largura da forma. @pre f != NULL. */
double forma_largura(Forma f);

/** @brief Altura da forma. @pre f != NULL. */
double forma_altura(Forma f);

/**
 * @brief Cor usada pelo critério de ordenação por cor.
 * @pre f != NULL.
 */
const char *forma_get_cor_preenchimento(Forma f);

/**
 * @brief Comparador da ordem default entre duas formas.
 * @return < 0 se @p a precede @p b; > 0 se sucede; 0 se equivalentes.
 * @pre a != NULL; b != NULL.
 */
int forma_comparar_default(Forma a, Forma b);

/**
 * @brief Desloca a forma por (dx, dy), despachando para o tipo concreto.
 * @pre f != NULL.
 */
void forma_mover(Forma f, double dx, double dy);

/**
 * @brief Altera as cores da forma.
 * @param f     Forma (não nula).
 * @param corb  Nova cor da borda (não nula).
 * @param corp  Nova cor de preenchimento (não nula).
 * @pre f != NULL; corb != NULL; corp != NULL.
 */
void forma_set_cores(Forma f, const char *corb, const char *corp);

#endif /* FORMA_H */