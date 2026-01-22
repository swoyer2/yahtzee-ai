main: main.c roll.c score.c
	gcc -o main main.c roll.c score.c agent.c -I. -lm
