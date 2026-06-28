#include "unity.h"
#include "../src/retangulo.h"

#define EPS 1e-9

/* Retângulo padrão: âncora (10,20), 4 de largura por 5 de altura -> área 20. */
static Retangulo r;

void setUp(void)
{
    r = retangulo_criar(3, 10.0, 20.0, 4.0, 5.0, "black", "yellow");
}

void tearDown(void)
{
    retangulo_destruir(r);
    r = NULL;
}


void test_criar_nao_retorna_nulo(void)
{
    TEST_ASSERT_NOT_NULL(r);
}

void test_getters_basicos(void)
{
    TEST_ASSERT_EQUAL_INT(3, retangulo_get_id(r));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, retangulo_get_x(r));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 20.0, retangulo_get_y(r));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 4.0, retangulo_get_largura(r));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 5.0, retangulo_get_altura(r));
}

void test_getters_de_cor(void)
{
    TEST_ASSERT_EQUAL_STRING("black", retangulo_get_cor_borda(r));
    TEST_ASSERT_EQUAL_STRING("yellow", retangulo_get_cor_preenchimento(r));
}

/* As cores devem ser copiadas na criação. */
void test_cores_sao_copiadas_na_criacao(void)
{
    char borda[] = "blue";
    char preench[] = "green";

    Retangulo rr = retangulo_criar(4, 0.0, 0.0, 1.0, 1.0, borda, preench);
    TEST_ASSERT_NOT_NULL(rr);

    borda[0] = 'X';
    preench[0] = 'Y';

    TEST_ASSERT_EQUAL_STRING("blue", retangulo_get_cor_borda(rr));
    TEST_ASSERT_EQUAL_STRING("green", retangulo_get_cor_preenchimento(rr));

    retangulo_destruir(rr);
}

void test_area(void)
{
    /* 4 * 5 = 20 */
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 20.0, retangulo_area(r));
}

void test_mover_desloca_a_ancora(void)
{
    retangulo_mover(r, 3.0, -7.5);
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 13.0, retangulo_get_x(r));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 12.5, retangulo_get_y(r));
}

void test_mover_nao_altera_dimensoes(void)
{
    retangulo_mover(r, 100.0, 100.0);
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 4.0, retangulo_get_largura(r));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 5.0, retangulo_get_altura(r));
}

void test_set_cores_atualiza(void)
{
    retangulo_set_cores(r, "red", "purple");
    TEST_ASSERT_EQUAL_STRING("red", retangulo_get_cor_borda(r));
    TEST_ASSERT_EQUAL_STRING("purple", retangulo_get_cor_preenchimento(r));
}

void test_set_cores_copia_os_argumentos(void)
{
    char nb[] = "cyan";
    char np[] = "magenta";

    retangulo_set_cores(r, nb, np);
    nb[0] = 'Z';
    np[0] = 'W';

    TEST_ASSERT_EQUAL_STRING("cyan", retangulo_get_cor_borda(r));
    TEST_ASSERT_EQUAL_STRING("magenta", retangulo_get_cor_preenchimento(r));
}


void test_clone_copia_atributos_com_novo_id(void)
{
    Retangulo clone = retangulo_clonar(r, 77);
    TEST_ASSERT_NOT_NULL(clone);

    TEST_ASSERT_EQUAL_INT(77, retangulo_get_id(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, retangulo_get_x(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 20.0, retangulo_get_y(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 4.0, retangulo_get_largura(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 5.0, retangulo_get_altura(clone));
    TEST_ASSERT_EQUAL_STRING("black", retangulo_get_cor_borda(clone));
    TEST_ASSERT_EQUAL_STRING("yellow", retangulo_get_cor_preenchimento(clone));

    retangulo_destruir(clone);
}

void test_clone_eh_independente(void)
{
    Retangulo clone = retangulo_clonar(r, 77);
    TEST_ASSERT_NOT_NULL(clone);

    retangulo_mover(r, 50.0, 50.0);
    retangulo_set_cores(r, "white", "white");

    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, retangulo_get_x(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 20.0, retangulo_get_y(clone));
    TEST_ASSERT_EQUAL_STRING("black", retangulo_get_cor_borda(clone));
    TEST_ASSERT_EQUAL_STRING("yellow", retangulo_get_cor_preenchimento(clone));

    retangulo_destruir(clone);
}

void test_destruir_nulo_eh_seguro(void)
{
    retangulo_destruir(NULL);
    TEST_PASS();
}


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_criar_nao_retorna_nulo);
    RUN_TEST(test_getters_basicos);
    RUN_TEST(test_getters_de_cor);
    RUN_TEST(test_cores_sao_copiadas_na_criacao);

    RUN_TEST(test_area);

    RUN_TEST(test_mover_desloca_a_ancora);
    RUN_TEST(test_mover_nao_altera_dimensoes);
    RUN_TEST(test_set_cores_atualiza);
    RUN_TEST(test_set_cores_copia_os_argumentos);

    RUN_TEST(test_clone_copia_atributos_com_novo_id);
    RUN_TEST(test_clone_eh_independente);

    RUN_TEST(test_destruir_nulo_eh_seguro);

    return UNITY_END();
}