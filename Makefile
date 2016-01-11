deck: deck.c
	gcc -o deck -g -O2 deck.c

clean: 
	rm -rf deck
