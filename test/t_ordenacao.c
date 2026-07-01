#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "../src/ordenacao.h"

/* Comparador para o módulo (itens = ponteiros para int). */
static int cmp_int(Item a, Item b)
{
    int x = *(int *) a;
    int y = *(int *) b;
    return (x > y) - (x < y);
}

/* Comparador para o qsort de referência (sobre int puro). */
static int cmp_q(const void *a, const void *b)
{
    int x = *(const int *) a;
    int y = *(const int *) b;
    return (x > y) - (x < y);
}

void setUp(void) {}
void tearDown(void) {}

/* Assinatura comum dos algoritmos. */
typedef void (*SortFn)(Item *, int, Comparador, FnSnapshot, void *);

/* Ordena 'entrada'com 'sort' e compara com o qsort. */
static void checa_caso(SortFn sort, const int *entrada, int n)
{
    int  data[64];
    Item vet[64];
    int  esperado[64];
    int  i;

    for (i = 0; i < n; i++) {
        data[i] = entrada[i];
        vet[i]  = &data[i];
        esperado[i] = entrada[i];
    }
    qsort(esperado, (size_t) n, sizeof(int), cmp_q);

    sort(vet, n, cmp_int, NULL, NULL);

    for (i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_INT(esperado[i], *(int *) vet[i]);
    }
}

/* Bateria de casos aplicada a um algoritmo. */
static void checa_algoritmo(SortFn sort)
{
    int embaralhado[] = {5, 3, 8, 1, 9, 2, 7, 4, 6, 0};
    int ordenado[]    = {1, 2, 3, 4, 5};
    int invertido[]   = {5, 4, 3, 2, 1};
    int duplicatas[]  = {3, 1, 2, 3, 1, 2, 3};
    int negativos[]   = {-3, 5, -1, 0, 2, -8, 7};
    int unitario[]    = {42};

    checa_caso(sort, embaralhado, 10);
    checa_caso(sort, ordenado, 5);
    checa_caso(sort, invertido, 5);
    checa_caso(sort, duplicatas, 7);
    checa_caso(sort, negativos, 7);
    checa_caso(sort, unitario, 1);
    checa_caso(sort, NULL, 0);      /* vetor vazio não deve quebrar */
}


void test_selection(void) { checa_algoritmo(ordenacao_selection); }
void test_bubble(void)    { checa_algoritmo(ordenacao_bubble); }
void test_insertion(void) { checa_algoritmo(ordenacao_insertion); }
void test_shell(void)     { checa_algoritmo(ordenacao_shell); }
void test_quick(void)     { checa_algoritmo(ordenacao_quick); }
void test_merge(void)     { checa_algoritmo(ordenacao_merge); }


static void conta_snapshot(Item *vet, int n, void *ctx)
{
    (void) vet;
    (void) n;
    (*(int *) ctx)++;
}

void test_snapshot_eh_chamado(void)
{
    int  data[] = {4, 2, 5, 1, 3};
    Item vet[5];
    int  i, frames;

    for (i = 0; i < 5; i++) vet[i] = &data[i];
    frames = 0;
    ordenacao_selection(vet, 5, cmp_int, conta_snapshot, &frames);
    TEST_ASSERT_TRUE(frames > 0);
}

void test_snapshot_nulo_nao_quebra(void)
{
    int  data[] = {3, 1, 2};
    Item vet[3];
    int  i;
    for (i = 0; i < 3; i++) vet[i] = &data[i];
    ordenacao_quick(vet, 3, cmp_int, NULL, NULL);   /* snap NULL */
    TEST_ASSERT_EQUAL_INT(1, *(int *) vet[0]);
    TEST_ASSERT_EQUAL_INT(2, *(int *) vet[1]);
    TEST_ASSERT_EQUAL_INT(3, *(int *) vet[2]);
}


void test_ordenar_reconhece_todos_os_codigos(void)
{
    const char *codigos[] = {"ss", "bs", "is", "shs", "qs", "ms"};
    int  c;
    for (c = 0; c < 6; c++) {
        int  data[] = {3, 1, 4, 1, 5, 9, 2, 6};
        Item vet[8];
        int  i, ret;
        for (i = 0; i < 8; i++) vet[i] = &data[i];

        ret = ordenar(codigos[c], vet, 8, cmp_int, NULL, NULL);
        TEST_ASSERT_EQUAL_INT(1, ret);
        /* confirma ordenação crescente */
        for (i = 0; i < 7; i++) {
            TEST_ASSERT_TRUE(*(int *) vet[i] <= *(int *) vet[i + 1]);
        }
    }
}

void test_ordenar_codigo_invalido_retorna_zero(void)
{
    int  data[] = {2, 1};
    Item vet[2];
    vet[0] = &data[0];
    vet[1] = &data[1];
    TEST_ASSERT_EQUAL_INT(0, ordenar("xx", vet, 2, cmp_int, NULL, NULL));
    /* vetor não deve ter sido tocado */
    TEST_ASSERT_EQUAL_INT(2, *(int *) vet[0]);
    TEST_ASSERT_EQUAL_INT(1, *(int *) vet[1]);
}


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_selection);
    RUN_TEST(test_bubble);
    RUN_TEST(test_insertion);
    RUN_TEST(test_shell);
    RUN_TEST(test_quick);
    RUN_TEST(test_merge);

    RUN_TEST(test_snapshot_eh_chamado);
    RUN_TEST(test_snapshot_nulo_nao_quebra);

    RUN_TEST(test_ordenar_reconhece_todos_os_codigos);
    RUN_TEST(test_ordenar_codigo_invalido_retorna_zero);

    return UNITY_END();
}