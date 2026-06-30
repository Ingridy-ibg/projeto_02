/**
 * @file t_forma.c
 * @brief Testes unitários do wrapper Forma (framework Unity).
 *
 * Compilação (exemplo):
 *   gcc -std=c99 -fstack-protector-all -Wall -Wextra -g -DUNITY_INCLUDE_DOUBLE \
 *       test/t_forma.c src/forma.c src/circulo.c src/retangulo.c \
 *       src/linha.c src/texto.c Unity/src/unity.c -o test/t_forma -lm
 */

#include "unity.h"
#include "../src/forma.h"
#include "../src/circulo.h"
#include "../src/retangulo.h"
#include "../src/linha.h"
#include "../src/texto.h"

#define EPS 1e-9
#define PI_REF 3.14159265358979323846

void setUp(void) {}
void tearDown(void) {}

static Forma nova_circulo(int id, double x, double y, double r,
                          const char *cb, const char *cp)
{
    return forma_envolver(TIPO_CIRCULO, circulo_criar(id, x, y, r, cb, cp));
}
static Forma nova_linha(int id, double x1, double y1, double x2, double y2,
                        const char *cor)
{
    return forma_envolver(TIPO_LINHA, linha_criar(id, x1, y1, x2, y2, cor));
}



void test_circulo_uniforme(void)
{
    Forma f = nova_circulo(1, 10.0, 20.0, 5.0, "black", "red");
    TEST_ASSERT_NOT_NULL(f);

    TEST_ASSERT_EQUAL_INT(TIPO_CIRCULO, forma_get_tipo(f));
    TEST_ASSERT_EQUAL_INT(1, forma_get_id(f));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, forma_get_x(f));   /* centro */
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 20.0, forma_get_y(f));
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, PI_REF * 25.0, forma_area(f));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, forma_largura(f)); /* diâmetro */
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, forma_altura(f));
    TEST_ASSERT_EQUAL_STRING("red", forma_get_cor_preenchimento(f));

    forma_destruir(f);
}


void test_retangulo_uniforme(void)
{
    Forma f = forma_envolver(TIPO_RETANGULO,
                             retangulo_criar(2, 1.0, 2.0, 4.0, 5.0, "blue", "green"));
    TEST_ASSERT_NOT_NULL(f);

    TEST_ASSERT_EQUAL_INT(TIPO_RETANGULO, forma_get_tipo(f));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 1.0, forma_get_x(f));    /* canto inf. esq. */
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 2.0, forma_get_y(f));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 20.0, forma_area(f));    /* 4*5 */
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 4.0, forma_largura(f));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 5.0, forma_altura(f));
    TEST_ASSERT_EQUAL_STRING("green", forma_get_cor_preenchimento(f));

    forma_destruir(f);
}


void test_linha_uniforme(void)
{
    Forma f = nova_linha(3, 0.0, 0.0, 3.0, 4.0, "purple");  /* comprimento 5 */
    TEST_ASSERT_NOT_NULL(f);

    TEST_ASSERT_EQUAL_INT(TIPO_LINHA, forma_get_tipo(f));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 0.0, forma_get_x(f));    /* 1a extremidade */
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 0.0, forma_get_y(f));
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 7.5, forma_area(f));    /* 1.5*5 */
    TEST_ASSERT_DOUBLE_WITHIN(1e-9, 5.0, forma_largura(f)); /* = comprimento */
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 1.5, forma_altura(f));
    /* critério c: linha usa sua cor única */
    TEST_ASSERT_EQUAL_STRING("purple", forma_get_cor_preenchimento(f));

    forma_destruir(f);
}

void test_texto_uniforme(void)
{
    Forma f = forma_envolver(TIPO_TEXTO,
                  texto_criar(4, 7.0, 8.0, "black", "navy", 'i',
                              "abcd", "sans", "n", "12"));   /* 4 chars */
    TEST_ASSERT_NOT_NULL(f);

    TEST_ASSERT_EQUAL_INT(TIPO_TEXTO, forma_get_tipo(f));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 7.0, forma_get_x(f));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 8.0, forma_get_y(f));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 40.0, forma_area(f));    /* 10*4 */
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 4.0, forma_largura(f));  /* 1*4 */
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 10.0, forma_altura(f));
    TEST_ASSERT_EQUAL_STRING("navy", forma_get_cor_preenchimento(f));

    forma_destruir(f);
}


void test_get_especifica_roundtrip(void)
{
    Forma f = nova_circulo(9, 1.0, 1.0, 7.0, "black", "white");
    TEST_ASSERT_NOT_NULL(f);

    TEST_ASSERT_EQUAL_INT(TIPO_CIRCULO, forma_get_tipo(f));
    Circulo c = forma_get_especifica(f);
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 7.0, circulo_get_raio(c));  

    forma_destruir(f);
}


void test_mover_despacha(void)
{
    Forma f = nova_linha(5, 0.0, 0.0, 10.0, 0.0, "red");
    forma_mover(f, 2.0, 3.0);

    Linha l = forma_get_especifica(f);
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 2.0, linha_get_x1(l));
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 12.0, linha_get_x2(l));   /* ambas movidas */
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 3.0, linha_get_y1(l));

    forma_destruir(f);
}

void test_set_cores_no_retangulo(void)
{
    Forma f = forma_envolver(TIPO_RETANGULO,
                  retangulo_criar(6, 0.0, 0.0, 1.0, 1.0, "black", "white"));
    forma_set_cores(f, "red", "blue");
    TEST_ASSERT_EQUAL_STRING("blue", forma_get_cor_preenchimento(f));
    forma_destruir(f);
}

/* Para a linha, set_cores usa corb (cor da borda). */
void test_set_cores_na_linha_usa_corb(void)
{
    Forma f = nova_linha(7, 0.0, 0.0, 1.0, 1.0, "black");
    forma_set_cores(f, "orange", "ignorada");
    TEST_ASSERT_EQUAL_STRING("orange", forma_get_cor_preenchimento(f));
    forma_destruir(f);
}


void test_clone_novo_id_independente(void)
{
    Forma f = nova_circulo(1, 5.0, 5.0, 2.0, "black", "red");
    Forma clone = forma_clonar(f, 50);
    TEST_ASSERT_NOT_NULL(clone);

    TEST_ASSERT_EQUAL_INT(50, forma_get_id(clone));
    TEST_ASSERT_EQUAL_INT(TIPO_CIRCULO, forma_get_tipo(clone));

    forma_mover(f, 100.0, 100.0);                 /* mexe no original */
    TEST_ASSERT_DOUBLE_WITHIN(EPS, 5.0, forma_get_x(clone));  /* clone intacto */

    forma_destruir(f);
    forma_destruir(clone);
}

/* -------------------------------------------------------------------------- */
/*  Ordem default                                                             */
/* -------------------------------------------------------------------------- */

void test_ordem_por_y(void)
{
    Forma a = nova_circulo(1, 0.0, 10.0, 1.0, "k", "k");
    Forma b = nova_circulo(2, 0.0, 20.0, 1.0, "k", "k");
    TEST_ASSERT_TRUE(forma_comparar_default(a, b) < 0);   /* y menor vem antes */
    TEST_ASSERT_TRUE(forma_comparar_default(b, a) > 0);
    forma_destruir(a);
    forma_destruir(b);
}

void test_desempate_por_x(void)
{
    Forma a = nova_circulo(1, 5.0, 10.0, 1.0, "k", "k");
    Forma b = nova_circulo(2, 8.0, 10.0, 1.0, "k", "k");   /* mesmo y */
    TEST_ASSERT_TRUE(forma_comparar_default(a, b) < 0);    /* x menor vem antes */
    forma_destruir(a);
    forma_destruir(b);
}

void test_desempate_por_area(void)
{
    /* mesmo y e x; áreas diferentes (raios 1 e 3) */
    Forma a = nova_circulo(1, 0.0, 0.0, 1.0, "k", "k");
    Forma b = nova_circulo(2, 0.0, 0.0, 3.0, "k", "k");
    TEST_ASSERT_TRUE(forma_comparar_default(a, b) < 0);    /* área menor antes */
    forma_destruir(a);
    forma_destruir(b);
}

void test_area_zero_para_linha_e_texto(void)
{
    /* linha de comprimento grande, mas área default = 0 */
    Forma linha = nova_linha(10, 0.0, 0.0, 100.0, 0.0, "k");
    Forma texto = forma_envolver(TIPO_TEXTO,
                      texto_criar(5, 0.0, 0.0, "k", "k", 'i',
                                  "texto longo", "sans", "n", "12"));
    /* mesmo y(0) e x(0); ambas área default 0 -> desempata por id (5 < 10) */
    TEST_ASSERT_TRUE(forma_comparar_default(texto, linha) < 0);
    forma_destruir(linha);
    forma_destruir(texto);
}



void test_envolver_nulo_retorna_nulo(void)
{
    TEST_ASSERT_NULL(forma_envolver(TIPO_CIRCULO, NULL));
}

void test_destruir_nulo_eh_seguro(void)
{
    forma_destruir(NULL);
    TEST_PASS();
}


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_circulo_uniforme);
    RUN_TEST(test_retangulo_uniforme);
    RUN_TEST(test_linha_uniforme);
    RUN_TEST(test_texto_uniforme);

    RUN_TEST(test_get_especifica_roundtrip);

    RUN_TEST(test_mover_despacha);
    RUN_TEST(test_set_cores_no_retangulo);
    RUN_TEST(test_set_cores_na_linha_usa_corb);

    RUN_TEST(test_clone_novo_id_independente);

    RUN_TEST(test_ordem_por_y);
    RUN_TEST(test_desempate_por_x);
    RUN_TEST(test_desempate_por_area);
    RUN_TEST(test_area_zero_para_linha_e_texto);

    RUN_TEST(test_envolver_nulo_retorna_nulo);
    RUN_TEST(test_destruir_nulo_eh_seguro);

    return UNITY_END();
}