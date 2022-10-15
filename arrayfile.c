//A program in C to create .dat type files recursively

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

int main(void){
	int L=5;
	FILE *f[L]; //array of file type pointers
	for(int i=0; i<L; i++ ){
		char filename[10];
		sprintf(filename, "file%d.dat", i); //it modifies at each  the name of the file to be created
		f[i]=fopen(filename, "w");
		if(f[i]==NULL){	//check if the file is opened correctly
	 		perror("File non correttamente aperto");
	 		exit(i);
			}
	fclose(f[i]);	
	}
	return 0;

}	
