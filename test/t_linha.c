#include "unity.h"
#include "../src/linha.h"

#define EPS 1e-9

static Linha l;

void setUp(void)
{
    l = linha_criar(7, 0.0, 0.0, 3.0, 4.0, "blue");
}

void tearDown(void)
{
    linha_destruir(l);
    l = NULL;
}

void test_criar_nao_retorna_nulo(void)
{
    TEST_ASSERT_NOT_NULL(l);
}

void test_getters_de_coordenadas(void)
{
    TEST_ASSERT_EQUAL_INT(7, linha_get_id(l));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 0.0, linha_get_x1(l));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 0.0, linha_get_y1(l));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 3.0, linha_get_x2(l));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 4.0, linha_get_y2(l));
}

void test_getter_de_cor(void)
{
    TEST_ASSERT_EQUAL_STRING("blue", linha_get_cor(l));
}

/* A cor deve ser copiada na criação. */
void test_cor_eh_copiada_na_criacao(void)
{
    char cor[] = "green";
    Linha ll = linha_criar(8, 1.0, 1.0, 2.0, 2.0, cor);
    TEST_ASSERT_NOT_NULL(ll);

    cor[0] = 'X';   /* corrompe o buffer original */
    TEST_ASSERT_EQUAL_STRING("green", linha_get_cor(ll));

    linha_destruir(ll);
}


void test_comprimento(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 5.0, linha_comprimento(l));
}

void test_largura_eh_comprimento(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 5.0, linha_largura(l));
}

void test_altura_eh_constante(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 1.5, linha_altura(l));
}

void test_area(void)
{
    /* 1.5 * comprimento = 1.5 * 5 = 7.5 */
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 7.5, linha_area(l));
}


void test_mover_translada_ambas_extremidades(void)
{
    linha_mover(l, 10.0, -2.0);
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, linha_get_x1(l));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, -2.0, linha_get_y1(l));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 13.0, linha_get_x2(l));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 2.0, linha_get_y2(l));
}

void test_mover_nao_altera_comprimento(void)
{
    linha_mover(l, 100.0, 50.0);
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 5.0, linha_comprimento(l));
}

void test_set_cor_atualiza(void)
{
    linha_set_cor(l, "red");
    TEST_ASSERT_EQUAL_STRING("red", linha_get_cor(l));
}

void test_set_cor_copia_o_argumento(void)
{
    char nova[] = "purple";
    linha_set_cor(l, nova);
    nova[0] = 'Z';
    TEST_ASSERT_EQUAL_STRING("purple", linha_get_cor(l));
}


void test_clone_copia_atributos_com_novo_id(void)
{
    Linha clone = linha_clonar(l, 42);
    TEST_ASSERT_NOT_NULL(clone);

    TEST_ASSERT_EQUAL_INT(42, linha_get_id(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 0.0, linha_get_x1(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 0.0, linha_get_y1(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 3.0, linha_get_x2(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 4.0, linha_get_y2(clone));
    TEST_ASSERT_EQUAL_STRING("blue", linha_get_cor(clone));

    linha_destruir(clone);
}

void test_clone_eh_independente(void)
{
    Linha clone = linha_clonar(l, 42);
    TEST_ASSERT_NOT_NULL(clone);

    linha_mover(l, 99.0, 99.0);
    linha_set_cor(l, "white");

    TEST_ASSERT_DOUBLE_WITHIN(EPS, 0.0, linha_get_x1(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 3.0, linha_get_x2(clone));
    TEST_ASSERT_EQUAL_STRING("blue", linha_get_cor(clone));

    linha_destruir(clone);
}


void test_destruir_nulo_eh_seguro(void)
{
    linha_destruir(NULL);
    TEST_PASS();
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_criar_nao_retorna_nulo);
    RUN_TEST(test_getters_de_coordenadas);
    RUN_TEST(test_getter_de_cor);
    RUN_TEST(test_cor_eh_copiada_na_criacao);

    RUN_TEST(test_comprimento);
    RUN_TEST(test_largura_eh_comprimento);
    RUN_TEST(test_altura_eh_constante);
    RUN_TEST(test_area);

    RUN_TEST(test_mover_translada_ambas_extremidades);
    RUN_TEST(test_mover_nao_altera_comprimento);
    RUN_TEST(test_set_cor_atualiza);
    RUN_TEST(test_set_cor_copia_o_argumento);

    RUN_TEST(test_clone_copia_atributos_com_novo_id);
    RUN_TEST(test_clone_eh_independente);

    RUN_TEST(test_destruir_nulo_eh_seguro);

    return UNITY_END();
}