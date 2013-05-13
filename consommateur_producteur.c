#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "Semaphore.h"

#ifndef DEBUG
//#define DEBUG
#endif

void consommateur(char* m, sem_t* plein, sem_t* vide, sem_t* mutex, char* str)
{
	int i=0;
	int j=0;
	int taille;
	int v ;
	for(i=0; i< strlen(str); i++) {
		#ifdef DEBUG
		printf("                                                 ");
		printf("0.1 : attente plein\n");
		#endif
		sem_wait(plein);
		sem_getvalue(plein, &v);
		#ifdef DEBUG
		printf("                                                 ");
		printf("0.2 : attente fini plein = %d, prise mutex\n", v);
		#endif
		sem_wait(mutex);
		#ifdef DEBUG
		printf("                                                 ");
		printf("0.3 : mutex pris\n");
		printf("                                                 ");
		printf("0.4 : rien\n");
		#endif
		taille = *m;

		char message = (*(m+1));
		(*m)--;
		taille = *m;
		
		printf("                                                 ");
		printf("0.5 : caratere %c, taille restante %d\n", message, taille);

		#ifdef DEBUG
		printf("                                                 ");
		printf("0.52 : caracteres dans m : ");
		for(j=1; j <= taille+1; j++) {

			printf(" %c ", *(m+j));
		}
		printf("\n");
		#endif
		for(j=1; j <= taille+1; j++) {
			*(m+j) = *(m+j+1);
		}
		#ifdef DEBUG
		printf("                                                 ");
		printf("0.53 : caracteres dans m : ");
		for(j=1; j <= taille+1; j++) {

			printf(" %c ", *(m+j));
		}
		printf("\n");
		#endif
		sem_post(vide);
		sem_getvalue(vide, &v);
		#ifdef DEBUG
		printf("                                                 ");
		printf("0.6 : post vide = %d\n", v);
		#endif
		sem_post(mutex);
		#ifdef DEBUG
		printf("                                                 ");
		printf("0.7 : mutex rendu\n");
		#endif
	}
}


void *thConsommateur (void *pt) 
{ 
	/*printf("Consommateur : \n");
	printf("&memoirePartage = %p", *((void**)pt));
	printf("&plein = %p", *((void**)pt +1));
	printf("&vide = %p", *((void**)pt+2));
	printf("&mutex = %p", *((void**)pt+3));
	printf("&str = %p \n", *((void**)pt+4));*/
	consommateur((char*) *((void**)pt), (sem_t*) *((void**)pt+1), (sem_t*) *((void**)pt+2), (sem_t*) *((void**)pt+3), (char*) *((void**)pt+4));
	printf ("Fils : Fin\n");
}

void producteur(char* m, sem_t* plein, sem_t* vide, sem_t* mutex, char* str)
{
	int v;
	#ifdef DEBUG
	sem_getvalue(plein, &v);
        printf ("plein : %d\n", v);
        sem_getvalue(vide, &v);
        printf ("vide : %d\n", v);
        
        printf ("mutex : %d\n", v);
	#endif
	int i=0;
	#ifdef DEBUG
	v = strlen(str);
	printf ("adresse : %p\n", str);
	printf ("string : %s\n", str);
	printf ("size : %d\n", v);
	#endif
	for(i=0; i< strlen(str); i++) {
		#ifdef DEBUG
		printf("1.1 : attente vide \n");
		#endif
		sem_wait(vide);
		#ifdef DEBUG
		printf("1.2 : attente fini, prise mutex\n");
		#endif
		sem_wait(mutex);
		#ifdef DEBUG
		printf("1.3 : mutex pris\n");
		printf("1.4 : rien\n");
		#endif
		v = m[0];
		#ifdef DEBUG
		printf("1.5 : m = %i \n", v);
		#endif
		*(m+v+1) = str[i];
		(*m)++;
		
		printf("1.6 : str = %c, m = %c \n", str[i], *(m+v+1));
		sem_post(plein);
		sem_post(mutex);
		#ifdef DEBUG
		printf("1.7 : mutex rendu\n");
		#endif
	}
}

void *thProducteur (void *pt) 
{int v;

	/*printf("Producteur : \n");
	printf("&memoirePartage = %p\n", *((void**)pt));
	printf("&plein = %p\n", *((void**)pt +1));
	printf("&vide = %p\n", *((void**)pt+2));
	printf("&mutex = %p\n", *((void**)pt+3));
	printf("&str = %p -> %s\n", *((void**)pt+4), *((void**)pt+4));*/
	//printf ("string : %s\n", );
	#ifdef DEBUG
        sem_getvalue((sem_t*) *((void**)pt+1), &v);
        printf ("plein : %d\n", v);
        sem_getvalue((sem_t*) *((void**)pt+2), &v);
        printf ("vide : %d\n", v);
        sem_getvalue((sem_t*) *((void**)pt+3), &v);
        printf ("mutex : %d\n", v);
	#endif
	producteur((char*) *((void**)pt), (sem_t*) *((void**)pt+1), (sem_t*) *((void**)pt+2), (sem_t*) *((void**)pt+3), (char*) *((void**)pt+4));

	sem_destroy(*((void**)pt +1));
	sem_destroy(*((void**)pt +2));
	sem_destroy(*((void**)pt +3));
	printf ("Pere : Fin\n");
}


int main (int argc, char** argv) {
	char* str = (char*)malloc(999*sizeof(char));
	char* memoirePartage = (char*)malloc(10*sizeof(char));
	pthread_t thread[2]; 

	void* val [5];

        int v;
	void* test = NULL;

	sem_t plein;
	sem_t vide;
	sem_t mutex;
	
	strcpy(str, "Bonjour");

	#ifdef DEBUG
	printf ("string : %s\n", str);
  
	printf("adresse string = %p \n", str);
	#endif
	printf ("start.\n");
	
	sem_init(&plein, 1, 0);
	sem_init(&vide, 1, 2);
	sem_init(&mutex, 1, 1);

	#ifdef DEBUG
        sem_getvalue(&plein, &v);
        sem_getvalue(&vide, &v);
        sem_getvalue(&mutex, &v);
	printf ("plein : %d\n", v);
        printf ("vide : %d\n", v);
        printf ("mutex : %d\n", v);
	#endif

	*memoirePartage = 0;

	val[0] = memoirePartage;
	val[1] = &plein;
	val[2] = &vide;
	val[3] = &mutex;
	val[4] = str;
	
	pthread_create (thread, NULL, thConsommateur, val);
	pthread_create (thread+1, NULL, thProducteur, val);


	pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);
	return 0;
}
