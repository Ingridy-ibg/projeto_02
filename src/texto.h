#ifndef TEXTO_H
#define TEXTO_H

/**
 * @file texto.h
 * @brief TAD Texto.
 *
 * Representa um texto do plano cartesiano, conforme o comando .geo:
 *
 *     t i x y corb corp a txto
 *
 * onde:
 *   - i        : identificador inteiro (>= 1) da forma;
 *   - (x, y)   : coordenada âncora do texto;
 *   - corb     : cor da borda (string no padrão de cores SVG);
 *   - corp     : cor de preenchimento (string no padrão de cores SVG);
 *   - a        : posição da âncora: 'i' (início), 'm' (meio) ou 'f' (fim);
 *   - txto     : o conteúdo do texto (pode conter espaços; vai até o fim da
 *                linha no arquivo de entrada).
 *
 * Estilo de fonte (definido pelo comando ts vigente no momento da criação):
 *   - familia  : "sans", "serif" ou "cursive";
 *   - peso     : "n" (normal), "b" (bold), "b+" (bolder) ou "l" (lighter);
 *   - tamanho  : tamanho da fonte (mantido como string, repassado tal qual ao
 *                SVG).
 *
 * IMPORTANTE: o comando ts altera o estilo dos textos SUBSEQUENTES. Portanto o
 * estilo é um "retrato" fixado na criação de cada texto. Quem controla o
 * "estilo corrente" (e o estilo inicial, antes de qualquer ts) é o parser do
 * .geo, que repassa familia/peso/tamanho a texto_criar(). Este módulo não
 * assume defaults.
 *
 * Atributos de ordenação (definições do enunciado):
 *   - largura : 1.0  * número de caracteres;
 *   - altura  : 10.0 (constante);
 *   - área    : 10.0 * número de caracteres.
 *
 * Este é um TAD OPACO: a struct é definida apenas em texto.c.
 *
 * Propriedade (ownership):
 *   - todas as strings passadas (cores, conteúdo, estilo) são COPIADAS
 *     internamente; o chamador continua dono dos buffers originais;
 *   - as strings devolvidas pelos getters pertencem ao texto; não liberar nem
 *     modificar;
 *   - todo Texto criado deve ser liberado com texto_destruir().
 */

/**
 * @brief Ponteiro opaco para um texto.
 *
 * A struct subjacente é definida apenas em texto.c (contrato: nenhuma struct
 * é definida neste cabeçalho).
 */
typedef struct texto *Texto;

/**
 * @brief Cria um texto, com estilo de fonte já embutido.
 *
 * @param id        Identificador inteiro da forma (>= 1).
 * @param x         x da âncora.
 * @param y         y da âncora.
 * @param corb      Cor da borda (não nula). É copiada.
 * @param corp      Cor de preenchimento (não nula). É copiada.
 * @param ancora    Posição da âncora: 'i', 'm' ou 'f'.
 * @param conteudo  Texto a exibir (não nulo). É copiado.
 * @param familia   Família da fonte (não nula). É copiada.
 * @param peso      Peso da fonte (não nulo). É copiado.
 * @param tamanho   Tamanho da fonte (não nulo). É copiado.
 *
 * @return Novo Texto alocado dinamicamente, ou NULL em falha de alocação.
 *         O chamador deve liberá-lo com texto_destruir().
 *
 * @pre corb, corp, conteudo, familia, peso, tamanho != NULL;
 *      ancora em {'i','m','f'}.
 */
Texto texto_criar(int id, double x, double y,
                  const char *corb, const char *corp,
                  char ancora, const char *conteudo,
                  const char *familia, const char *peso, const char *tamanho);

/**
 * @brief Cria uma cópia independente de um texto, com novo identificador.
 *
 * Útil para o comando de clonagem (cm). A cópia possui suas próprias strings;
 * alterações em uma não afetam a outra.
 *
 * @param t        Texto a clonar (não nulo).
 * @param novo_id  Identificador do clone (>= 1).
 * @return Novo Texto idêntico a @p t (exceto pelo id), ou NULL em falha.
 * @pre t != NULL.
 */
Texto texto_clonar(Texto t, int novo_id);

/**
 * @brief Libera toda a memória associada a um texto.
 *
 * Após a chamada, @p t não deve mais ser usado. Chamar com NULL é seguro.
 *
 * @param t Texto a destruir (pode ser NULL).
 */
void texto_destruir(Texto t);

/* -------------------------------------------------------------------------- */
/*  Consultas (getters)                                                       */
/* -------------------------------------------------------------------------- */

/** @brief Identificador do texto. @pre t != NULL. */
int texto_get_id(Texto t);

/** @brief x da âncora. @pre t != NULL. */
double texto_get_x(Texto t);

/** @brief y da âncora. @pre t != NULL. */
double texto_get_y(Texto t);

/** @brief Cor da borda (não liberar nem modificar). @pre t != NULL. */
const char *texto_get_cor_borda(Texto t);

/** @brief Cor de preenchimento (não liberar nem modificar). @pre t != NULL. */
const char *texto_get_cor_preenchimento(Texto t);

/** @brief Posição da âncora: 'i', 'm' ou 'f'. @pre t != NULL. */
char texto_get_ancora(Texto t);

/** @brief Conteúdo do texto (não liberar nem modificar). @pre t != NULL. */
const char *texto_get_conteudo(Texto t);

/** @brief Família da fonte (não liberar nem modificar). @pre t != NULL. */
const char *texto_get_familia(Texto t);

/** @brief Peso da fonte (não liberar nem modificar). @pre t != NULL. */
const char *texto_get_peso(Texto t);

/** @brief Tamanho da fonte (não liberar nem modificar). @pre t != NULL. */
const char *texto_get_tamanho(Texto t);

/* -------------------------------------------------------------------------- */
/*  Atributos derivados (para os critérios de ordenação)                      */
/* -------------------------------------------------------------------------- */

/**
 * @brief Número de caracteres do conteúdo (strlen, contando espaços).
 * @param t Texto (não nulo).
 * @pre t != NULL.
 */
int texto_num_caracteres(Texto t);

/**
 * @brief Largura do texto: 1.0 * número de caracteres.
 * @param t Texto (não nulo).
 * @pre t != NULL.
 */
double texto_largura(Texto t);

/**
 * @brief Altura do texto: constante 10.0.
 * @param t Texto (não nulo).
 * @pre t != NULL.
 */
double texto_altura(Texto t);

/**
 * @brief Área do texto: 10.0 * número de caracteres.
 * @param t Texto (não nulo).
 * @pre t != NULL.
 */
double texto_area(Texto t);

/* -------------------------------------------------------------------------- */
/*  Modificações                                                              */
/* -------------------------------------------------------------------------- */

/**
 * @brief Desloca a âncora do texto por (dx, dy).
 *
 * Usado na clonagem com movimento (cm) e no reposicionamento de find/findrm.
 *
 * @param t   Texto (não nulo).
 * @param dx  Deslocamento no eixo x.
 * @param dy  Deslocamento no eixo y.
 * @pre t != NULL.
 */
void texto_mover(Texto t, double dx, double dy);

/**
 * @brief Altera as cores do texto (comando mc).
 *
 * As novas strings são copiadas; as cores anteriores são liberadas.
 *
 * @param t     Texto (não nulo).
 * @param corb  Nova cor da borda (não nula). É copiada.
 * @param corp  Nova cor de preenchimento (não nula). É copiada.
 * @pre t != NULL; corb != NULL; corp != NULL.
 */
void texto_set_cores(Texto t, const char *corb, const char *corp);

#endif /* TEXTO_H */