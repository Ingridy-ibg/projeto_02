#include <stdlib.h>

#include "forma.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

struct forma {
    TipoForma tipo;
    void     *esp;   /* aponta para Circulo / Retangulo / Linha / Texto */
};

static void destruir_especifica(TipoForma tipo, void *esp)
{
    switch (tipo) {
        case TIPO_CIRCULO:   circulo_destruir(esp);   break;
        case TIPO_RETANGULO: retangulo_destruir(esp); break;
        case TIPO_LINHA:     linha_destruir(esp);     break;
        case TIPO_TEXTO:     texto_destruir(esp);     break;
    }
}

static double area_ordem_default(Forma f)
{
    switch (f->tipo) {
        case TIPO_CIRCULO:   return circulo_area(f->esp);
        case TIPO_RETANGULO: return retangulo_area(f->esp);
        case TIPO_LINHA:     return 0.0;
        case TIPO_TEXTO:     return 0.0;
    }
    return 0.0;
}

Forma forma_envolver(TipoForma tipo, void *especifica)
{
    Forma f;

    if (especifica == NULL) {
        return NULL;
    }
    f = malloc(sizeof(struct forma));
    if (f == NULL) {
        return NULL;   
    }
    f->tipo = tipo;
    f->esp  = especifica;
    return f;
}

Forma forma_clonar(Forma f, int novo_id)
{
    void *clone_esp = NULL;
    Forma nova;

    if (f == NULL) {
        return NULL;
    }

    switch (f->tipo) {
        case TIPO_CIRCULO:   clone_esp = circulo_clonar(f->esp, novo_id);   break;
        case TIPO_RETANGULO: clone_esp = retangulo_clonar(f->esp, novo_id); break;
        case TIPO_LINHA:     clone_esp = linha_clonar(f->esp, novo_id);     break;
        case TIPO_TEXTO:     clone_esp = texto_clonar(f->esp, novo_id);     break;
    }
    if (clone_esp == NULL) {
        return NULL;
    }

    nova = forma_envolver(f->tipo, clone_esp);
    if (nova == NULL) {
        destruir_especifica(f->tipo, clone_esp);  
        return NULL;
    }
    return nova;
}

void forma_destruir(Forma f)
{
    if (f == NULL) {
        return;
    }
    destruir_especifica(f->tipo, f->esp);
    free(f);
}


TipoForma forma_get_tipo(Forma f)
{
    return f->tipo;
}

void *forma_get_especifica(Forma f)
{
    return f->esp;
}


int forma_get_id(Forma f)
{
    switch (f->tipo) {
        case TIPO_CIRCULO:   return circulo_get_id(f->esp);
        case TIPO_RETANGULO: return retangulo_get_id(f->esp);
        case TIPO_LINHA:     return linha_get_id(f->esp);
        case TIPO_TEXTO:     return texto_get_id(f->esp);
    }
    return 0;
}

double forma_get_x(Forma f)
{
    switch (f->tipo) {
        case TIPO_CIRCULO:   return circulo_get_x(f->esp);
        case TIPO_RETANGULO: return retangulo_get_x(f->esp);
        case TIPO_LINHA:     return linha_get_x1(f->esp);  
        case TIPO_TEXTO:     return texto_get_x(f->esp);
    }
    return 0.0;
}

double forma_get_y(Forma f)
{
    switch (f->tipo) {
        case TIPO_CIRCULO:   return circulo_get_y(f->esp);
        case TIPO_RETANGULO: return retangulo_get_y(f->esp);
        case TIPO_LINHA:     return linha_get_y1(f->esp); 
        case TIPO_TEXTO:     return texto_get_y(f->esp);
    }
    return 0.0;
}

double forma_area(Forma f)
{
    switch (f->tipo) {
        case TIPO_CIRCULO:   return circulo_area(f->esp);
        case TIPO_RETANGULO: return retangulo_area(f->esp);
        case TIPO_LINHA:     return linha_area(f->esp);
        case TIPO_TEXTO:     return texto_area(f->esp);
    }
    return 0.0;
}

double forma_largura(Forma f)
{
    switch (f->tipo) {
        case TIPO_CIRCULO:   return circulo_largura(f->esp);
        case TIPO_RETANGULO: return retangulo_get_largura(f->esp);
        case TIPO_LINHA:     return linha_largura(f->esp);
        case TIPO_TEXTO:     return texto_largura(f->esp);
    }
    return 0.0;
}

double forma_altura(Forma f)
{
    switch (f->tipo) {
        case TIPO_CIRCULO:   return circulo_altura(f->esp);
        case TIPO_RETANGULO: return retangulo_get_altura(f->esp);
        case TIPO_LINHA:     return linha_altura(f->esp);
        case TIPO_TEXTO:     return texto_altura(f->esp);
    }
    return 0.0;
}

const char *forma_get_cor_preenchimento(Forma f)
{
    switch (f->tipo) {
        case TIPO_CIRCULO:   return circulo_get_cor_preenchimento(f->esp);
        case TIPO_RETANGULO: return retangulo_get_cor_preenchimento(f->esp);
        case TIPO_LINHA:     return linha_get_cor(f->esp);   /* cor única */
        case TIPO_TEXTO:     return texto_get_cor_preenchimento(f->esp);
    }
    return NULL;
}



int forma_comparar_default(Forma a, Forma b)
{
    double ya, yb, xa, xb, aa, ab;
    int    ia, ib;

    ya = forma_get_y(a);
    yb = forma_get_y(b);
    if (ya < yb) return -1;
    if (ya > yb) return  1;

    xa = forma_get_x(a);
    xb = forma_get_x(b);
    if (xa < xb) return -1;
    if (xa > xb) return  1;

    aa = area_ordem_default(a);
    ab = area_ordem_default(b);
    if (aa < ab) return -1;
    if (aa > ab) return  1;

    ia = forma_get_id(a);
    ib = forma_get_id(b);
    if (ia < ib) return -1;
    if (ia > ib) return  1;

    return 0;
}


void forma_mover(Forma f, double dx, double dy)
{
    switch (f->tipo) {
        case TIPO_CIRCULO:   circulo_mover(f->esp, dx, dy);   break;
        case TIPO_RETANGULO: retangulo_mover(f->esp, dx, dy); break;
        case TIPO_LINHA:     linha_mover(f->esp, dx, dy);     break;
        case TIPO_TEXTO:     texto_mover(f->esp, dx, dy);     break;
    }
}

void forma_set_cores(Forma f, const char *corb, const char *corp)
{
    switch (f->tipo) {
        case TIPO_CIRCULO:   circulo_set_cores(f->esp, corb, corp);   break;
        case TIPO_RETANGULO: retangulo_set_cores(f->esp, corb, corp); break;
        case TIPO_LINHA:     linha_set_cor(f->esp, corb);             break;
        case TIPO_TEXTO:     texto_set_cores(f->esp, corb, corp);     break;
    }
}