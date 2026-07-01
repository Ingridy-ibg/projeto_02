#include <stdlib.h>
#include "unity.h"
#include "abb.h"

static int cmp_int(Item a, Item b)
{
    int x = *(int *) a;
    int y = *(int *) b;
    return (x > y) - (x < y);
}

static int V[]  = {50, 30, 70, 20, 40, 60, 80};
static int NAO  = 999;   /* não inserido */

static Abb a;

void setUp(void)
{
    a = abb_criar(cmp_int);
}

void tearDown(void)
{
    abb_destruir(a, NULL);   /* itens estáticos*/
    a = NULL;
}

static void inserir_padrao(void)
{
    size_t i;
    for (i = 0; i < sizeof(V) / sizeof(V[0]); i++) {
        TEST_ASSERT_EQUAL_INT(1, abb_inserir(a, &V[i]));
    }
}

/* Coletor: acumula os valores visitados num vetor. */
typedef struct { int buf[16]; int idx; } Coletor;

static void coletar(Item it, void *ctx)
{
    Coletor *c = ctx;
    c->buf[c->idx++] = *(int *) it;
}

/* Percorre em-ordem e verifica se a sequência bate com o esperado. */
static void checar_em_ordem(const int *esperado, int m)
{
    Coletor c;
    int i;
    c.idx = 0;
    abb_percorrer_em_ordem(a, coletar, &c);
    TEST_ASSERT_EQUAL_INT(m, c.idx);
    for (i = 0; i < m; i++) {
        TEST_ASSERT_EQUAL_INT(esperado[i], c.buf[i]);
    }
}

void test_criar_vazia(void)
{
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_TRUE(abb_vazia(a));
    TEST_ASSERT_EQUAL_INT(0, abb_tamanho(a));
}

void test_em_ordem_fica_crescente(void)
{
    int esperado[] = {20, 30, 40, 50, 60, 70, 80};
    inserir_padrao();
    TEST_ASSERT_EQUAL_INT(7, abb_tamanho(a));
    TEST_ASSERT_FALSE(abb_vazia(a));
    checar_em_ordem(esperado, 7);
}

void test_buscar_encontra_o_item_armazenado(void)
{
    inserir_padrao();
    Item achado = abb_buscar(a, &V[4]);   /* valor 40 */
    TEST_ASSERT_NOT_NULL(achado);
    TEST_ASSERT_EQUAL_INT(40, *(int *) achado);
    TEST_ASSERT_EQUAL_PTR(&V[4], achado); /* devolve o ponteiro real guardado */
}

void test_buscar_inexistente_retorna_nulo(void)
{
    inserir_padrao();
    TEST_ASSERT_NULL(abb_buscar(a, &NAO));
}


/* Folha: 20 é folha na árvore padrão. */
void test_remover_folha(void)
{
    int esperado[] = {30, 40, 50, 60, 70, 80};
    inserir_padrao();
    Item r = abb_remover(a, &V[3]);       /* 20 */
    TEST_ASSERT_EQUAL_INT(20, *(int *) r);
    TEST_ASSERT_EQUAL_INT(6, abb_tamanho(a));
    checar_em_ordem(esperado, 6);
}

/* Um filho: removendo 20 primeiro, 30 passa a ter só o filho 40. */
void test_remover_no_com_um_filho(void)
{
    int esperado[] = {40, 50, 60, 70, 80};
    inserir_padrao();
    abb_remover(a, &V[3]);                 /* remove 20 -> 30 fica com 1 filho */
    Item r = abb_remover(a, &V[1]);        /* remove 30 */
    TEST_ASSERT_EQUAL_INT(30, *(int *) r);
    TEST_ASSERT_EQUAL_INT(5, abb_tamanho(a));
    checar_em_ordem(esperado, 5);
}

/* Dois filhos: 30 (filhos 20 e 40) na árvore padrão. */
void test_remover_no_com_dois_filhos(void)
{
    int esperado[] = {20, 40, 50, 60, 70, 80};
    inserir_padrao();
    Item r = abb_remover(a, &V[1]);        /* 30, dois filhos */
    TEST_ASSERT_EQUAL_INT(30, *(int *) r);
    TEST_ASSERT_EQUAL_INT(6, abb_tamanho(a));
    checar_em_ordem(esperado, 6);          /* continua crescente e sem o 30 */
}

/* Raiz com dois filhos: 50. */
void test_remover_raiz(void)
{
    int esperado[] = {20, 30, 40, 60, 70, 80};
    inserir_padrao();
    Item r = abb_remover(a, &V[0]);        /* 50, raiz */
    TEST_ASSERT_EQUAL_INT(50, *(int *) r);
    TEST_ASSERT_EQUAL_INT(6, abb_tamanho(a));
    checar_em_ordem(esperado, 6);
}

void test_remover_inexistente_retorna_nulo(void)
{
    inserir_padrao();
    TEST_ASSERT_NULL(abb_remover(a, &NAO));
    TEST_ASSERT_EQUAL_INT(7, abb_tamanho(a));   /* tamanho inalterado */
}

/* Remover tudo, um a um, deixa a árvore vazia e consistente. */
void test_remover_ate_esvaziar(void)
{
    size_t i;
    inserir_padrao();
    for (i = 0; i < sizeof(V) / sizeof(V[0]); i++) {
        TEST_ASSERT_NOT_NULL(abb_remover(a, &V[i]));
    }
    TEST_ASSERT_TRUE(abb_vazia(a));
    TEST_ASSERT_EQUAL_INT(0, abb_tamanho(a));
}



/* Com libera_item, os itens alocados devem ser liberados sem vazamento*/
void test_destruir_com_libera_item(void)
{
    Abb a2 = abb_criar(cmp_int);
    int i;
    for (i = 0; i < 5; i++) {
        int *p = malloc(sizeof(int));
        *p = (i * 37) % 100;               /* valores variados */
        abb_inserir(a2, p);
    }
    abb_destruir(a2, free);                /* libera ints e nós */
    TEST_PASS();
}

void test_destruir_nulo_eh_seguro(void)
{
    abb_destruir(NULL, NULL);
    TEST_PASS();
}


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_criar_vazia);
    RUN_TEST(test_em_ordem_fica_crescente);

    RUN_TEST(test_buscar_encontra_o_item_armazenado);
    RUN_TEST(test_buscar_inexistente_retorna_nulo);

    RUN_TEST(test_remover_folha);
    RUN_TEST(test_remover_no_com_um_filho);
    RUN_TEST(test_remover_no_com_dois_filhos);
    RUN_TEST(test_remover_raiz);
    RUN_TEST(test_remover_inexistente_retorna_nulo);
    RUN_TEST(test_remover_ate_esvaziar);

    RUN_TEST(test_destruir_com_libera_item);
    RUN_TEST(test_destruir_nulo_eh_seguro);

    return UNITY_END();
}