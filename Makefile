CC = gcc
MD = mkdir -p
RM = rm -rf

CFLAGS +=	\
-Wextra		\
-Wall   	\
-lm   		\

SAVE_FILE := results

MODE ?= P1


.PHONY: header obj static dynamic clean


header:
	@$(MAKE) --no-print-directory MODE=P1 main

obj: 
	@$(MAKE) --no-print-directory MODE=P2 main

dynamic:
	@$(MAKE) --no-print-directory MODE=PD main

static:
	@$(MAKE) --no-print-directory MODE=PS main


# test: libODEsolver.h ODEsolver.h | ODEsolver.c
# 	@echo [$@], [$^], [$<], [$?]


ODEsolverlib.o: ODEsolverlib.h
	$(CC) -DODESOLVERLIB_IMPLEMENTATION -x c -c $<

libODEsolver.so: ODEsolver.c
	$(CC) $(CFLAGS) -DPROG3 -fPIC -shared -o $@ $<


libODEsolver.a: ODEsolver.o
	ar -cvq $@ $<
	@ar -t $@

ODEsolver.o: ODEsolver.c
	$(CC) $(CFLAGS) -c $<

ifeq ($(MODE), P1)
main: main.c | $(SAVE_FILE)
	@echo [INFO]: Simple compilation of programs
	$(CC) $< -o $@ -DPROG1 $(CFLAGS)
else ifeq ($(MODE), P2)
main: main.c ODEsolverlib.o | $(SAVE_FILE)
	@echo [INFO]: Compile with obj lib
	$(CC) $< ODEsolverlib.o -o $@ -DPROG2 $(CFLAGS)
else ifeq ($(MODE), PD)
main: main.c libODEsolver.so | $(SAVE_FILE)
	@echo [INFO]: Compile the dynamique library
	$(CC) $< -o $@ -DPROG3 $(CFLAGS) -L. -lODEsolver -Wl,-rpath=./
else ifeq ($(MODE), PS)
main: main.c libODEsolver.a | $(SAVE_FILE)
	@echo [INFO]: Compile the static library
	$(CC) $< -o $@ -L. -lODEsolver -DPROG4 $(CFLAGS)
else
	@echo ERROR
endif


$(SAVE_FILE):
	$(MD) $(SAVE_FILE)

clean:
	$(RM) *.o
	$(RM) *.a
	$(RM) *.so
	$(RM) results
	$(RM) main
