/**
 * @file t_circulo.c
 * @brief Testes unitários do TAD Circulo (framework Unity).
 *
 * Compilação (exemplo de target no Makefile):
 *   gcc -std=c99 -fstack-protector-all -Wall -Wextra -g \
 *       -DUNITY_INCLUDE_DOUBLE \
 *       t_circulo.c circulo.c unity.c -o t_circulo -lm
 *
 * Obs.: a flag -DUNITY_INCLUDE_DOUBLE habilita TEST_ASSERT_DOUBLE_WITHIN.
 */

#include "unity.h"
#include "../src/circulo.h"

/* Valor de PI com precisão dupla, igual ao M_PI usado na implementação. */
#define PI_REF 3.14159265358979323846
/* Tolerância para comparações de ponto flutuante. */
#define EPS 1e-9

/* Círculo padrão recriado antes de cada teste. */
static Circulo c;

void setUp(void)
{
    c = circulo_criar(1, 10.0, 20.0, 5.0, "black", "red");
}

void tearDown(void)
{
    circulo_destruir(c);
    c = NULL;
}

/* -------------------------------------------------------------------------- */
/*  Criação e getters                                                         */
/* -------------------------------------------------------------------------- */

void test_criar_nao_retorna_nulo(void)
{
    TEST_ASSERT_NOT_NULL(c);
}

void test_getters_basicos(void)
{
    TEST_ASSERT_EQUAL_INT(1, circulo_get_id(c));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, circulo_get_x(c));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 20.0, circulo_get_y(c));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 5.0, circulo_get_raio(c));
}

void test_getters_de_cor(void)
{
    TEST_ASSERT_EQUAL_STRING("black", circulo_get_cor_borda(c));
    TEST_ASSERT_EQUAL_STRING("red", circulo_get_cor_preenchimento(c));
}

/* As cores devem ser COPIADAS internamente: alterar o buffer original
   depois da criação não pode afetar o círculo. */
void test_cores_sao_copiadas_na_criacao(void)
{
    char borda[] = "blue";
    char preench[] = "green";

    Circulo cc = circulo_criar(2, 0.0, 0.0, 1.0, borda, preench);
    TEST_ASSERT_NOT_NULL(cc);

    borda[0] = 'X';     /* corrompe o buffer original */
    preench[0] = 'Y';

    TEST_ASSERT_EQUAL_STRING("blue", circulo_get_cor_borda(cc));
    TEST_ASSERT_EQUAL_STRING("green", circulo_get_cor_preenchimento(cc));

    circulo_destruir(cc);
}

/* -------------------------------------------------------------------------- */
/*  Atributos derivados                                                       */
/* -------------------------------------------------------------------------- */

void test_area(void)
{
    /* r = 5  ->  area = pi * 25 */
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, PI_REF * 25.0, circulo_area(c));
}

void test_largura_eh_diametro(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, circulo_largura(c));
}

void test_altura_eh_diametro(void)
{
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, circulo_altura(c));
}

/* -------------------------------------------------------------------------- */
/*  Modificações                                                              */
/* -------------------------------------------------------------------------- */

void test_mover_desloca_o_centro(void)
{
    circulo_mover(c, 3.0, -7.5);
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 13.0, circulo_get_x(c));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 12.5, circulo_get_y(c));
}

void test_mover_nao_altera_raio(void)
{
    circulo_mover(c, 100.0, 100.0);
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 5.0, circulo_get_raio(c));
}

void test_set_cores_atualiza(void)
{
    circulo_set_cores(c, "yellow", "purple");
    TEST_ASSERT_EQUAL_STRING("yellow", circulo_get_cor_borda(c));
    TEST_ASSERT_EQUAL_STRING("purple", circulo_get_cor_preenchimento(c));
}

/* set_cores também deve copiar (e liberar as cores antigas sem corromper). */
void test_set_cores_copia_os_argumentos(void)
{
    char nova_borda[] = "cyan";
    char nova_preench[] = "magenta";

    circulo_set_cores(c, nova_borda, nova_preench);
    nova_borda[0] = 'Z';
    nova_preench[0] = 'W';

    TEST_ASSERT_EQUAL_STRING("cyan", circulo_get_cor_borda(c));
    TEST_ASSERT_EQUAL_STRING("magenta", circulo_get_cor_preenchimento(c));
}

/* -------------------------------------------------------------------------- */
/*  Clonagem                                                                  */
/* -------------------------------------------------------------------------- */

void test_clone_copia_atributos_com_novo_id(void)
{
    Circulo clone = circulo_clonar(c, 99);
    TEST_ASSERT_NOT_NULL(clone);

    TEST_ASSERT_EQUAL_INT(99, circulo_get_id(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, circulo_get_x(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 20.0, circulo_get_y(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 5.0, circulo_get_raio(clone));
    TEST_ASSERT_EQUAL_STRING("black", circulo_get_cor_borda(clone));
    TEST_ASSERT_EQUAL_STRING("red", circulo_get_cor_preenchimento(clone));

    circulo_destruir(clone);
}

/* O clone deve ser independente: mexer no original não afeta o clone. */
void test_clone_eh_independente(void)
{
    Circulo clone = circulo_clonar(c, 99);
    TEST_ASSERT_NOT_NULL(clone);

    circulo_mover(c, 50.0, 50.0);
    circulo_set_cores(c, "white", "white");

    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, circulo_get_x(clone));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 20.0, circulo_get_y(clone));
    TEST_ASSERT_EQUAL_STRING("black", circulo_get_cor_borda(clone));
    TEST_ASSERT_EQUAL_STRING("red", circulo_get_cor_preenchimento(clone));

    circulo_destruir(clone);
}

/* -------------------------------------------------------------------------- */
/*  Robustez                                                                  */
/* -------------------------------------------------------------------------- */

/* circulo_destruir(NULL) deve ser seguro (no-op). */
void test_destruir_nulo_eh_seguro(void)
{
    circulo_destruir(NULL);
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
    RUN_TEST(test_cores_sao_copiadas_na_criacao);

    RUN_TEST(test_area);
    RUN_TEST(test_largura_eh_diametro);
    RUN_TEST(test_altura_eh_diametro);

    RUN_TEST(test_mover_desloca_o_centro);
    RUN_TEST(test_mover_nao_altera_raio);
    RUN_TEST(test_set_cores_atualiza);
    RUN_TEST(test_set_cores_copia_os_argumentos);

    RUN_TEST(test_clone_copia_atributos_com_novo_id);
    RUN_TEST(test_clone_eh_independente);

    RUN_TEST(test_destruir_nulo_eh_seguro);

    return UNITY_END();
}