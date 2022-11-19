#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>



int start=0, M=100000, acc;
	float mu=5, sigma2=1;
	float xp, y, r, p, q; //y è il xk+1
	FILE *f;

int metro(int d, int N){
	
	f=fopen("metrogauss2.txt","w");
	if(f==NULL){
	 perror("File non correttamente aperto");
	 exit(1);
	}
	srand(time(NULL));

	y=start;
	for (int i = 0; i < N; i++){
		acc=0;
		xp=rand()%(M);
		xp=xp/M;
		xp=y+d*(1.0-2.0*xp);
		r=1;
		p=exp((-pow((xp-mu),2)+pow((y-mu),2))/(2*sigma2));
		if(r>p){
			r=p;
		}
		q=rand()%(M);
		q=q/M;
		if(q<r){
			y=xp;
			acc=1;
		}
		fprintf(f, "%d  %f  %d\n", i, y, acc);
	}
	fclose(f);
	return 0;
}
