/**
 * @file t_texto.c
 * @brief Testes unitários do TAD Texto (framework Unity).
 *
 * Compilação (exemplo):
 *   gcc -std=c99 -fstack-protector-all -Wall -Wextra -g \
 *       -DUNITY_INCLUDE_DOUBLE \
 *       test/t_texto.c src/texto.c Unity/src/unity.c -o test/t_texto
 */

#include "unity.h"
#include "../src/texto.h"

#define EPS 1e-9

/* Texto padrão: conteúdo "abc def" tem 7 caracteres (com o espaço). */
static Texto t;

void setUp(void)
{
    t = texto_criar(5, 30.0, 40.0, "black", "red",
                    'm', "abc def", "sans", "b", "12");
}

void tearDown(void)
{
    texto_destruir(t);
    t = NULL;
}

/* -------------------------------------------------------------------------- */
/*  Criação e getters                                                         */
/* -------------------------------------------------------------------------- */

void test_criar_nao_retorna_nulo(void)
{
    TEST_ASSERT_NOT_NULL(t);
}

void test_getters_basicos(void)
{
    TEST_ASSERT_EQUAL_INT(5, texto_get_id(t));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 30.0, texto_get_x(t));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 40.0, texto_get_y(t));
    TEST_ASSERT_EQUAL_CHAR('m', texto_get_ancora(t));
    TEST_ASSERT_EQUAL_STRING("abc def", texto_get_conteudo(t));
}

void test_getters_de_cor(void)
{
    TEST_ASSERT_EQUAL_STRING("black", texto_get_cor_borda(t));
    TEST_ASSERT_EQUAL_STRING("red", texto_get_cor_preenchimento(t));
}

void test_getters_de_estilo(void)
{
    TEST_ASSERT_EQUAL_STRING("sans", texto_get_familia(t));
    TEST_ASSERT_EQUAL_STRING("b", texto_get_peso(t));
    TEST_ASSERT_EQUAL_STRING("12", texto_get_tamanho(t));
}

/* Cores, conteúdo e estilo devem ser COPIADOS na criação. */
void test_strings_sao_copiadas_na_criacao(void)
{
    char corb[]     = "blue";
    char corp[]     = "green";
    char conteudo[] = "xyz";
    char familia[]  = "serif";
    char peso[]     = "l";
    char tamanho[]  = "20";

    Texto tt = texto_criar(6, 0.0, 0.0, corb, corp, 'i',
                           conteudo, familia, peso, tamanho);
    TEST_ASSERT_NOT_NULL(tt);

    /* corrompe todos os buffers originais */
    corb[0] = 'X'; corp[0] = 'X'; conteudo[0] = 'X';
    familia[0] = 'X'; peso[0] = 'X'; tamanho[0] = 'X';

    TEST_ASSERT_EQUAL_STRING("blue", texto_get_cor_borda(tt));
    TEST_ASSERT_EQUAL_STRING("green", texto_get_cor_preenchimento(tt));
    TEST_ASSERT_EQUAL_STRING("xyz", texto_get_conteudo(tt));
    TEST_ASSERT_EQUAL_STRING("serif", texto_get_familia(tt));
    TEST_ASSERT_EQUAL_STRING("l", texto_get_peso(tt));
    TEST_ASSERT_EQUAL_STRING("20", texto_get_tamanho(tt));

    texto_destruir(tt);
}

/* -------------------------------------------------------------------------- */
/*  Atributos derivados                                                       */
/* -------------------------------------------------------------------------- */

void test_num_caracteres_conta_espacos(void)
{
    /* "abc def" -> 7 */
    TEST_ASSERT_EQUAL_INT(7, texto_num_caracteres(t));
}

void test_largura(void)
{
    /* 1.0 * 7 = 7.0 */
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 7.0, texto_largura(t));
}

void test_altura_eh_constante(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, texto_altura(t));
}

void test_area(void)
{
    /* 10.0 * 7 = 70.0 */
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 70.0, texto_area(t));
}

/* -------------------------------------------------------------------------- */
/*  Modificações                                                              */
/* -------------------------------------------------------------------------- */

void test_mover_desloca_a_ancora(void)
{
    texto_mover(t, 5.0, -10.0);
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 35.0, texto_get_x(t));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 30.0, texto_get_y(t));
}

void test_set_cores_atualiza(void)
{
    texto_set_cores(t, "cyan", "magenta");
    TEST_ASSERT_EQUAL_STRING("cyan", texto_get_cor_borda(t));
    TEST_ASSERT_EQUAL_STRING("magenta", texto_get_cor_preenchimento(t));
}

void test_set_cores_copia_os_argumentos(void)
{
    char nb[] = "pink";
    char np[] = "orange";

    texto_set_cores(t, nb, np);
    nb[0] = 'Z';
    np[0] = 'W';

    TEST_ASSERT_EQUAL_STRING("pink", texto_get_cor_borda(t));
    TEST_ASSERT_EQUAL_STRING("orange", texto_get_cor_preenchimento(t));
}

/* set_cores não pode alterar o conteúdo nem o estilo. */
void test_set_cores_preserva_conteudo_e_estilo(void)
{
    texto_set_cores(t, "white", "white");
    TEST_ASSERT_EQUAL_STRING("abc def", texto_get_conteudo(t));
    TEST_ASSERT_EQUAL_STRING("sans", texto_get_familia(t));
    TEST_ASSERT_EQUAL_STRING("b", texto_get_peso(t));
    TEST_ASSERT_EQUAL_STRING("12", texto_get_tamanho(t));
}

/* -------------------------------------------------------------------------- */
/*  Clonagem                                                                  */
/* -------------------------------------------------------------------------- */

void test_clone_copia_atributos_com_novo_id(void)
{
    Texto clone = texto_clonar(t, 100);
    TEST_ASSERT_NOT_NULL(clone);

    TEST_ASSERT_EQUAL_INT(100, texto_get_id(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 30.0, texto_get_x(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 40.0, texto_get_y(clone));
    TEST_ASSERT_EQUAL_CHAR('m', texto_get_ancora(clone));
    TEST_ASSERT_EQUAL_STRING("abc def", texto_get_conteudo(clone));
    TEST_ASSERT_EQUAL_STRING("black", texto_get_cor_borda(clone));
    TEST_ASSERT_EQUAL_STRING("red", texto_get_cor_preenchimento(clone));
    TEST_ASSERT_EQUAL_STRING("sans", texto_get_familia(clone));
    TEST_ASSERT_EQUAL_STRING("b", texto_get_peso(clone));
    TEST_ASSERT_EQUAL_STRING("12", texto_get_tamanho(clone));

    texto_destruir(clone);
}

void test_clone_eh_independente(void)
{
    Texto clone = texto_clonar(t, 100);
    TEST_ASSERT_NOT_NULL(clone);

    texto_mover(t, 99.0, 99.0);
    texto_set_cores(t, "white", "white");

    TEST_ASSERT_DOUBLE_WITHIN(EPS, 30.0, texto_get_x(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 40.0, texto_get_y(clone));
    TEST_ASSERT_EQUAL_STRING("black", texto_get_cor_borda(clone));
    TEST_ASSERT_EQUAL_STRING("red", texto_get_cor_preenchimento(clone));

    texto_destruir(clone);
}

/* -------------------------------------------------------------------------- */
/*  Robustez                                                                  */
/* -------------------------------------------------------------------------- */

void test_destruir_nulo_eh_seguro(void)
{
    texto_destruir(NULL);
    TEST_PASS();
}

/* -------------------------------------------------------------------------- */
/*  Runner                                                                    */
/* -------------------------------------------------------------------------- */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_criar_nao_retorna_nulo);
    RUN_TEST(test_getters_basicos);
    RUN_TEST(test_getters_de_cor);
    RUN_TEST(test_getters_de_estilo);
    RUN_TEST(test_strings_sao_copiadas_na_criacao);

    RUN_TEST(test_num_caracteres_conta_espacos);
    RUN_TEST(test_largura);
    RUN_TEST(test_altura_eh_constante);
    RUN_TEST(test_area);

    RUN_TEST(test_mover_desloca_a_ancora);
    RUN_TEST(test_set_cores_atualiza);
    RUN_TEST(test_set_cores_copia_os_argumentos);
    RUN_TEST(test_set_cores_preserva_conteudo_e_estilo);

    RUN_TEST(test_clone_copia_atributos_com_novo_id);
    RUN_TEST(test_clone_eh_independente);

    RUN_TEST(test_destruir_nulo_eh_seguro);

    return UNITY_END();
}