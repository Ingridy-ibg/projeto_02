PROJ_NAME = ted
ALUNO     = IngridyGimenes
LIBS      = -lm

OBJETOS = 

CC      = gcc
CFLAGS  = -ggdb -O0 -std=c99 -fstack-protector-all \
          -Werror=implicit-function-declaration \
          -I./src -I./Unity/src \
          -DUNITY_INCLUDE_DOUBLE
LDFLAGS = -O0

# Objeto do Unity (definido UMA vez, usado por todos os testes)
UNITY_OBJ = Unity/src/unity.o

# ─── Executável principal ───────────────────────────────────────
$(PROJ_NAME): $(OBJETOS)
	$(CC) -o src/$(PROJ_NAME) $(LDFLAGS) $(OBJETOS) $(LIBS) -lm

# ─── Regra genérica de compilação (.o) ──────────────────────────
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

# ─── Dependências dos módulos ────────────────────────────────────

src/retangulo.o: src/retangulo.c src/retangulo.h

test/t_retangulo.o: test/t_retangulo.c src/retangulo.h Unity/src/unity.h

src/circulo.o: src/circulo.c src/circulo.h

test/t_circulo.o: test/t_circulo.c src/circulo.h Unity/src/unity.h

src/linha.o: src/linha.c src/linha.h

test/t_linha.o: test/t_linha.c src/linha.h Unity/src/unity.h

src/texto.o: src/texto.c src/texto.h

test/t_texto.o: test/t_texto.c src/texto.h Unity/src/unity.h

src/forma.o: src/forma.c src/forma.h src/circulo.h src/retangulo.h src/linha.h src/texto.h

test/t_forma.o: test/t_forma.c src/forma.h src/circulo.h src/retangulo.h src/linha.h src/texto.h Unity/src/unity.h

src/lista.o: src/lista.c src/lista.h

test/t_lista.o: test/t_lista.c src/lista.h Unity/src/unity.h

test/t_abb.o: test/t_abb.c src/abb.h Unity/src/unity.h

src/abb.o: src/abb.c src/abb.h





# ─── Testes unitários ────────────────────────────────────────────


t_retangulo: test/t_retangulo.o src/retangulo.o $(UNITY_OBJ)
	$(CC) $(LDFLAGS) test/t_retangulo.o src/retangulo.o $(UNITY_OBJ) \
		-o test/t_retangulo $(LIBS)
	./test/t_retangulo

t_circulo: test/t_circulo.o src/circulo.o $(UNITY_OBJ)
	$(CC) $(LDFLAGS) test/t_circulo.o src/circulo.o Unity/src/unity.o \
		-o test/t_circulo $(LIBS)
	./test/t_circulo	

t_linha: test/t_linha.o src/linha.o Unity/src/unity.o $(UNITY_OBJ)
	$(CC) $(LDFLAGS) test/t_linha.o src/linha.o Unity/src/unity.o \
		-o test/t_linha $(LIBS)
	./test/t_linha

t_texto: test/t_texto.o src/texto.o Unity/src/unity.o $(UNITY_OBJ)
	$(CC) $(LDFLAGS) test/t_texto.o src/texto.o Unity/src/unity.o \
		-o test/t_texto $(LIBS)
	./test/t_texto

t_forma: test/t_forma.o src/forma.o src/circulo.o src/retangulo.o src/linha.o src/texto.o $(UNITY_OBJ)
	$(CC) $(LDFLAGS) test/t_forma.o src/forma.o src/circulo.o src/retangulo.o src/linha.o src/texto.o $(UNITY_OBJ) \
		-o test/t_forma $(LIBS)
	./test/t_forma

t_lista: test/t_lista.o src/lista.o $(UNITY_OBJ)
	$(CC) $(LDFLAGS) test/t_lista.o src/lista.o $(UNITY_OBJ) \
		-o test/t_lista $(LIBS)
	./test/t_lista

t_abb: test/t_abb.o src/abb.o $(UNITY_OBJ)
	$(CC) $(LDFLAGS) test/t_abb.o src/abb.o $(UNITY_OBJ) \
		-o test/t_abb $(LIBS)
	./test/t_abb

tstall: t_retangulo t_circulo t_linha t_texto t_forma t_lista t_abb


# ─── Utilitários ─────────────────────────────────────────────────
clean:
	rm -f src/circulo.o test/t_circulo.o test/t_circulo \
		src/retangulo.o test/t_retangulo.o test/t_retangulo \
		src/linha.o test/t_linha.o test/t_linha \
		src/texto.o test/t_texto.o test/t_texto \
		src/forma.o test/t_forma.o test/t_forma \
		src/lista.o test/t_lista.o test/t_lista \
		src/abb.o test/t_abb.o test/t_abb \
		Unity/src/unity.o \
		src/$(PROJ_NAME)

run: $(PROJ_NAME)
	./src/$(PROJ_NAME)