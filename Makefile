all: 
	gcc consommateur_producteur.c -lpthread -o consommateur_producteur

clean:
	rm consommateur_producteur
