#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include "metrogauss.h"
#include "listfunction.h"

int main(void){
	int N=10000;
	list *d=NULL;
	//d=scan_term(d);  //versione in cui si leggono i valori di delta da terminale
	FILE *f, *g, *delta;
	delta=fopen("delta.txt", "r");
	if(delta==NULL){
	 perror("File non correttamente aperto");
	 exit(1);
	}
	d=scan_file(delta, d); //versione in cui si leggono i valori di delta da file delta.txt
	f=fopen("metrogauss_accett.txt", "w");
	if(f==NULL){
	 perror("File non correttamente aperto");
	 exit(2);
	}
	while (d!=NULL){
		int a, acc, x;
		float b, sum=0;
		metro(d->val, N); //funz di metrogauss.h che realizza un metropolis a N passi producendo un file con variabili distribuite gaussianamente
		g=fopen("metrogauss2.txt", "r");
		if(g==NULL){
	 		perror("File non correttamente aperto");
	 		exit(3);
	 	}
	 	x=fscanf(g, "%d  %f  %d", &a, &b, &acc);
	 	while (x!=EOF){
	 		sum=sum+acc;
	 		x=fscanf(g, "%d  %f  %d", &a, &b, &acc);
	 		}	 	
	 	sum=(sum/N)*100; // calcolo l'accettanza percentuale per il valore di delta in esame
	 	fprintf(f, "%f %f\n", sum, d->val);
	 	fclose(g);
	 	d=d->next;
		}
	fclose(f);
	fclose(delta);
	return 0;

}