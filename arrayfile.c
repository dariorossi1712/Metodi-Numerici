#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

int main(void){
	int L=5;
	FILE *f[L];
	for(int i=0; i<L; i++ ){
		char filename[10];
		sprintf(filename, "file%d.dat", i);
		f[i]=fopen(filename, "w");
		if(f[i]==NULL){
	 perror("File non correttamente aperto");
	 exit(i);
	}
	}
	return 0;

}	