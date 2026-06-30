#include <stdlib.h>
#include "unity.h"
#include "../src/lista.h"


static int v[5] = {10, 20, 30, 40, 50};

static Lista l;

void setUp(void)
{
    l = lista_criar();
}

void tearDown(void)
{
    lista_destruir(l, NULL);  
    l = NULL;
}


static int val(Item it)
{
    return *(int *) it;
}

void test_criar_lista_vazia(void)
{
    TEST_ASSERT_NOT_NULL(l);
    TEST_ASSERT_EQUAL_INT(0, lista_tamanho(l));
    TEST_ASSERT_TRUE(lista_vazia(l));
}
 

void test_inserir_fim_mantem_ordem(void)
{
    TEST_ASSERT_EQUAL_INT(1, lista_inserir_fim(l, &v[0]));
    TEST_ASSERT_EQUAL_INT(1, lista_inserir_fim(l, &v[1]));
    TEST_ASSERT_EQUAL_INT(1, lista_inserir_fim(l, &v[2]));

    TEST_ASSERT_EQUAL_INT(3, lista_tamanho(l));
    TEST_ASSERT_FALSE(lista_vazia(l));

    int n;
    Item *vet = lista_para_vetor(l, &n);
    TEST_ASSERT_EQUAL_INT(3, n);
    TEST_ASSERT_EQUAL_INT(10, val(vet[0]));
    TEST_ASSERT_EQUAL_INT(20, val(vet[1]));
    TEST_ASSERT_EQUAL_INT(30, val(vet[2]));
    free(vet);
}

void test_inserir_inicio_inverte_ordem(void)
{
    lista_inserir_inicio(l, &v[0]);   /* 10 */
    lista_inserir_inicio(l, &v[1]);   /* 20 10 */
    lista_inserir_inicio(l, &v[2]);   /* 30 20 10 */

    int n;
    Item *vet = lista_para_vetor(l, &n);
    TEST_ASSERT_EQUAL_INT(3, n);
    TEST_ASSERT_EQUAL_INT(30, val(vet[0]));
    TEST_ASSERT_EQUAL_INT(20, val(vet[1]));
    TEST_ASSERT_EQUAL_INT(10, val(vet[2]));
    free(vet);
}

void test_inserir_misto(void)
{
    lista_inserir_fim(l, &v[1]);      /* 20 */
    lista_inserir_inicio(l, &v[0]);   /* 10 20 */
    lista_inserir_fim(l, &v[2]);      /* 10 20 30 */

    int n;
    Item *vet = lista_para_vetor(l, &n);
    TEST_ASSERT_EQUAL_INT(10, val(vet[0]));
    TEST_ASSERT_EQUAL_INT(20, val(vet[1]));
    TEST_ASSERT_EQUAL_INT(30, val(vet[2]));
    free(vet);
}


void test_remover_do_meio(void)
{
    lista_inserir_fim(l, &v[0]);
    lista_inserir_fim(l, &v[1]);
    lista_inserir_fim(l, &v[2]);

    TEST_ASSERT_EQUAL_INT(1, lista_remover(l, &v[1]));   /* remove 20 */
    TEST_ASSERT_EQUAL_INT(2, lista_tamanho(l));

    int n;
    Item *vet = lista_para_vetor(l, &n);
    TEST_ASSERT_EQUAL_INT(10, val(vet[0]));
    TEST_ASSERT_EQUAL_INT(30, val(vet[1]));
    free(vet);
}

void test_remover_o_primeiro(void)
{
    lista_inserir_fim(l, &v[0]);
    lista_inserir_fim(l, &v[1]);
    TEST_ASSERT_EQUAL_INT(1, lista_remover(l, &v[0]));

    int n;
    Item *vet = lista_para_vetor(l, &n);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_INT(20, val(vet[0]));
    free(vet);
}

/* Remover o último deve atualizar o ponteiro de fim */
void test_remover_o_ultimo_atualiza_fim(void)
{
    lista_inserir_fim(l, &v[0]);
    lista_inserir_fim(l, &v[1]);
    TEST_ASSERT_EQUAL_INT(1, lista_remover(l, &v[1]));   /* remove o fim (20) */

    lista_inserir_fim(l, &v[2]);   /* deve ir para o fim: 10 30 */
    int n;
    Item *vet = lista_para_vetor(l, &n);
    TEST_ASSERT_EQUAL_INT(2, n);
    TEST_ASSERT_EQUAL_INT(10, val(vet[0]));
    TEST_ASSERT_EQUAL_INT(30, val(vet[1]));
    free(vet);
}

void test_remover_unico_esvazia(void)
{
    lista_inserir_fim(l, &v[0]);
    TEST_ASSERT_EQUAL_INT(1, lista_remover(l, &v[0]));
    TEST_ASSERT_TRUE(lista_vazia(l));
    TEST_ASSERT_EQUAL_INT(0, lista_tamanho(l));
}

void test_remover_inexistente_retorna_zero(void)
{
    lista_inserir_fim(l, &v[0]);
    TEST_ASSERT_EQUAL_INT(0, lista_remover(l, &v[3]));   /* 40 não está */
    TEST_ASSERT_EQUAL_INT(1, lista_tamanho(l));
}



void test_para_vetor_vazia_retorna_nulo(void)
{
    int n = -1;
    Item *vet = lista_para_vetor(l, &n);
    TEST_ASSERT_NULL(vet);
    TEST_ASSERT_EQUAL_INT(0, n);
}



static void soma(Item it, void *ctx)
{
    *(int *) ctx += *(int *) it;
}

void test_percorrer_visita_todos_em_ordem(void)
{
    lista_inserir_fim(l, &v[0]);   /* 10 */
    lista_inserir_fim(l, &v[1]);   /* 20 */
    lista_inserir_fim(l, &v[2]);   /* 30 */

    int acc = 0;
    lista_percorrer(l, soma, &acc);
    TEST_ASSERT_EQUAL_INT(60, acc);
}


void test_destruir_com_libera_item(void)
{
    Lista l2 = lista_criar();
    int *a = malloc(sizeof(int)); *a = 1;
    int *b = malloc(sizeof(int)); *b = 2;
    lista_inserir_fim(l2, a);
    lista_inserir_fim(l2, b);
    lista_destruir(l2, free);   /* libera os ints e os nós */
    TEST_PASS();
}

void test_destruir_nulo_eh_seguro(void)
{
    lista_destruir(NULL, NULL);
    TEST_PASS();
}


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_criar_lista_vazia);

    RUN_TEST(test_inserir_fim_mantem_ordem);
    RUN_TEST(test_inserir_inicio_inverte_ordem);
    RUN_TEST(test_inserir_misto);

    RUN_TEST(test_remover_do_meio);
    RUN_TEST(test_remover_o_primeiro);
    RUN_TEST(test_remover_o_ultimo_atualiza_fim);
    RUN_TEST(test_remover_unico_esvazia);
    RUN_TEST(test_remover_inexistente_retorna_zero);

    RUN_TEST(test_para_vetor_vazia_retorna_nulo);

    RUN_TEST(test_percorrer_visita_todos_em_ordem);

    RUN_TEST(test_destruir_com_libera_item);
    RUN_TEST(test_destruir_nulo_eh_seguro);

    return UNITY_END();
}