PROJ_NAME = ted
ALUNO     = IngridyGimenes
LIBS      =

OBJETOS = 

CC      = gcc
CFLAGS  = -ggdb -O0 -std=c99 -fstack-protector-all \
          -Werror=implicit-function-declaration \
          -I./src -I./Unity/src \
          -DUNITY_INCLUDE_DOUBLE
LDFLAGS = -O0

# ─── Executável principal ───────────────────────────────────────
$(PROJ_NAME): $(OBJETOS)
	$(CC) -o src/$(PROJ_NAME) $(LDFLAGS) $(OBJETOS) $(LIBS) -lm

# ─── Regra genérica de compilação (.o) ──────────────────────────
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

# ─── Dependências dos módulos ────────────────────────────────────

src/retangulo.o: src/retangulo.c src/retangulo.h

test/t_retangulo.o: test/t_retangulo.c src/retangulo.h Unity/src/unity.h





# ─── Testes unitários ────────────────────────────────────────────


t_retangulo: test/t_retangulo.o src/retangulo.o Unity/src/unity.o
	$(CC) $(LDFLAGS) test/t_retangulo.o src/retangulo.o Unity/src/unity.o \
		-o test/t_retangulo
	./test/t_retangulo


tstall: t_retangulo

# ─── Utilitários ─────────────────────────────────────────────────
clean:
	rm -f src/retangulo.o \
		Unity/src/unity.o \
		  test/t_retangulo.o \
		  test/t_retangulo \
		  src/$(PROJ_NAME)

rodatestes: ted
	@mkdir -p saida
	@count=0; \
	for geo in $$(find testes/ -name "*.geo"); do \
	    dir=$$(dirname $$geo); \
	    stem=$$(basename $$geo .geo); \
	    qrydir=$$dir/$$stem; \
	    if [ -d "$$qrydir" ]; then \
	        for qry in $$qrydir/*.qry; do \
	            qrystem=$$(basename $$qry .qry); \
	            echo "[GEO+QRY] $$stem + $$qrystem"; \
	            ./src/ted -e $$dir -f $$stem.geo -q $$stem/$$qrystem.qry -o ./saida; \
	            count=$$((count + 1)); \
	        done; \
	    else \
	        echo "[GEO]     $$stem"; \
	        ./src/ted -e $$dir -f $$stem.geo -o ./saida; \
	        count=$$((count + 1)); \
	    fi; \
	done; \
	echo ""; \
	echo "$$count arquivo(s) processado(s). Gerados em saida/:"; \
	ls saida/
	
run: $(PROJ_NAME)
	./src/$(PROJ_NAME)