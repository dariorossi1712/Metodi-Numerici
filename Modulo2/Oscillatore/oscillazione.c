#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"/home/dario/Documents/Metodi/ran2.h"
#include"/home/dario/Documents/Metodi/usefulstuff.h"

/* Programma per la simulazione dell'oscillatore armonico*/

#define N 10
int Nlatt=N;
long int seed = 13;  //seed per il generatore di numeri casuali ran2 importato come libreria
float eta, d_metro; //eta=a*omega = parametro reticolo * pulsazione;  d_metro = parametro del metropolis = 2*sqrt(eta) 
int iflag, measures, i_decorrel, i_term;  //vedi ising. i_term = passo di termalizzazione
int npp[N], nmm[N]; //array per definire le posizioni dei primi vicini del lattice
float field[N];  // array contenente la configurazione del "percorso fatto" nel senso di path integral


/* per ogni coordinata definisco il passo in avanti o indietro con le opportune condizioni al bordo*/
void geometry(){
    
    for(int i=0; i<Nlatt; i++){
        npp[i] = i + 1;
        nmm[i] = i - 1;
    }
    
    npp[Nlatt-1] = 0; //Condizioni al
    nmm[0] = Nlatt-1; //bordo periodiche
    
    return;
        
}

/* -------------------------*/

/* Assegno la configurazione di partenza della catena di Markov 
Qui l'utilità della partenza a caldo o a freddo non ha una grande utilità*/
void initialize_lattice(int iflag){
    float x;
    
    /* PARTENZA A FREDDO (tutti gli spin a 1, o -1, come se fosse T = 0) */
    if(iflag == 0){
        for(int i = 0; i<Nlatt; i++){
                
            field[i] = 0;
            
        }
    }
    /* PARTENZA A CALDO (spin random, come se fosse T = infinito) */
    if(iflag == 1){
        for(int i = 0; i<Nlatt; i++){
            //float x;
            //seed = i;
            x = 1-2*ran2(&seed);
            field[i] = x;
            //printf("%f\n", field[i]);
        }
    }
    
    // AGGIUNGERE IF PER RIPARTIRE DALLA CONFIGURAZIONE PRECEDENTE
    
    
    return;
}


/*funzione per avanzare col metropolis e modificare la configurazione*/

void update_metropolis(){
    float c1, c2; //sono solo shortcut per scrivere 1/eta e altra funz di eta
    int ip, im; //coordinate dei 2 primi vicini
    float force, phi, phi_prova;  //force = forza del campo intorno; phi = valore attuale del campo; phi_prova = valore di prova del campo
    float p_rat, x; // p_rat = valore di energia con cui confrontare x che è estratto a caso e mi dirà se accettare il passo o no

    c1 = 1/eta;
    c2 = 1/eta + eta/2;

    /*loop su tutti i siti, qui il sito non è scelto a caso ma faccio una spazzata 
    iterativa su tutti i siti, si può dimostrare che va bene lo stesso per il bilancio dettagliato, 
    ma meno banale da provare*/
    for(int i = 0; i<Nlatt; i++){
        ip = npp[i];
        im = nmm[i];
        force = field[ip] + field[im];

        phi = field[i];
        phi_prova = phi + 2*d_metro*(0.5 - ran2(&seed));

        p_rat= c1 * phi_prova * force - c2*pow(phi_prova,2);
        p_rat= p_rat - c1*phi*force + c2*pow(phi,2);
        //printf("%f\n", p_rat);
        x = log(ran2(&seed));  //METRO-TEST! 

        if(x<p_rat)field[i]=phi_prova;  //test accettanza, se p_rat>1 accetto
    }
    return;
}


/*Tale funzione ha prende come argomento un array di 2 elementi che vuiene dichiarato nel main e restituisce il puntatore ad esso*/
double * measure(double obs[2]){ 

    for(int i=0; i<Nlatt; i++){
        obs[0]=obs[0] + pow(field[i],2);
        obs[1] = obs[1] + pow(field[i]-field[npp[i]],2);
    }

    obs[0] = obs[0]/Nlatt;  //media sul singolo path di y**2
    obs[1] = obs[1]/Nlatt;  //media sul singolo path di Delta y**2

    //printf("%lf  %lf\n", obs[0], obs[1]);

    return obs;
}


/*=================================== SIMULAZIONE =============================================*/

//NOTA: il file input.txt deve essere già presente e contenere i sei valori specificati sotto scritti in riga

int main(){
    FILE * lat, *input, *misure; /* file in cui stampo il field per ogni passo del metropolis(lat, era uno schiribizzo mio, in realtà si può togliere);
    file ( input ) da cui prendo i valori iniziali; file in cui stampo i valori medi delle osservabili della funzione measure() ( misure )*/
    
    int x; //per leggere l'input.txt
    
    //========  OPERAZIONI PRELIMINARI
    init = fopen("/home/dario/Documents/Metodi/Modulo2/Oscillatore/input.txt","r" );
    control_file(lat);

    misure = fopen("/home/dario/Documents/Metodi/Modulo2/Oscillatore/misure.txt","w" );
    control_file(misure);

    x= fscanf(init, "%f  %f  %d  %d  %d  %d", &eta, &d_metro, &measures, &i_decorrel, &iflag, &i_term);/* Prendo i valoriniziali di
    eta(float), d_metro(float) l'intervallo intorno alla posizione scelta in cui prendere la posizione di prova,
    measures(int) numero di misure, i_decorrel(int) numero di passi del metropolis tra una misura e la successiva, iflag(int) stabilisce solo la configurazione di partenza,
    i_term(int) numero di passi di metropolis dopo il quale si ha termalizzazione e si può iniziare a prendere misure*/
    
    /*lat = fopen("/home/dario/Documents/Metodi/Modulo2/Oscillatore/lattice.txt","w" );
    control_file(lat);*/

    initialize_lattice(iflag);
    geometry();

    //=======  TERMALIZZAZIONE
    for (int i = 1; i<i_term+1; i++){
        update_metropolis();
    }

    //==========  SESSIONE ALL'EQUILIBRIO con MISURE
    double obs[2]={0,0};
    for (int iter=0; iter<measures; iter++){
        // AGGIORNAMENTO CONFIGURAZIONE
        for(int idec=0; idec<i_decorrel; idec++ ){
            update_metropolis();
            /*for (int i = 0; i < Nlatt; i++){  //stampo su file il field ma non so a cosa serva e cosa si debba vedere
                fprintf(lat, "%f  ", field[i]);}*/
            
            //fprintf(lat, "\n");
            }
        obs[0] = measure(obs)[0];
        obs[1] = measure(obs)[1];
        //printf("%lf  %lf  %d\n", obs[0], obs[1], iter);
        fprintf(misure,"%lf  %lf  %d\n", obs[0], obs[1], iter); //prendo misure a questa configurazione e le stampo su file
    }

    //fclose(lat);
    fclose(misure);
    fclose(init);
    return 0;

}
