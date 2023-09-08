# Autor: Segundo Paladines
.DEFAULT_GOAL := compile

run:
        ./myshell $(comando)

compile:
        gcc -o myshell myshell.c -lpthread

clean:
        rm -f myshell
