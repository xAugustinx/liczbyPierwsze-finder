#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>

int threadAmount,turnOn = 1;
long * liczby;

long liczbaPoprzedniegoKonca;

FILE * meowFile;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;
int czyLiczbaJestPierwsza(long x) {
    if (x == 2 || x == 3 || x == 7 || x == 5) {return 1;}

    if (x%2 == 0 || x%3 == 0 || x%7 == 0 || x%5 == 0 ) return 0;

    for (int i = 11; i * i < x; i+=2 ) {
        if (!(x % i)) {return 0;}
    }
    return 1;
}


void * maths(void * args) {
    long licznik = *(long*)args;
    while (1) {
        if (!turnOn) {break;}
        if (czyLiczbaJestPierwsza(licznik))
        {printf("%ld\n",licznik);}
        pthread_mutex_lock(&lock);
        fprintf(meowFile,"%ld\n",licznik);
        pthread_mutex_unlock(&lock);
        pthread_barrier_wait(&barrier);
        usleep(20000);
        licznik+= threadAmount;
    }
}

int main() {
    if (access("wladyszKomyszKasiniak", F_OK) != 0) {
        FILE * tworzeniePliku = fopen("wladyszKomyszKasiniak","w");
        fclose(tworzeniePliku);
    }
    else {
        FILE * koncowaLiczba = fopen("wladyszKomyszKasiniak", "r");
        fseek(koncowaLiczba, -2, SEEK_END  );
        int koniec = ftell(koncowaLiczba);
        char wartosc;

        for (int i = koniec; i > 0; i--) {
            fseek(koncowaLiczba, i, SEEK_SET );
            fscanf(koncowaLiczba,"%c",&wartosc  );
            if (wartosc == '\n') {
                fseek(koncowaLiczba, i+1, SEEK_SET );
                fscanf(koncowaLiczba,"%ld",&liczbaPoprzedniegoKonca);
                break;
            }
        }
        printf("%ld\n",liczbaPoprzedniegoKonca);
    }

    meowFile = fopen("wladyszKomyszKasiniak","a");



    threadAmount = get_nprocs();
    printf("You want to Use every thread? ");
    char awanser = 'Y';
    scanf("%c", &awanser);
    if (!(awanser == 'y' || awanser == 'Y')) {
        printf("0 - %d\n",get_nprocs() );
        scanf("%d",&threadAmount);
    }

    liczby = malloc(sizeof(long*) * threadAmount);
    pthread_t threads[threadAmount];
    pthread_barrier_init(&barrier, NULL,threadAmount);


    for (int i = 0; i < threadAmount; i++) {
        long * wartoscWejsciowa = malloc( sizeof(long) * 1 );
        *wartoscWejsciowa = i + 2 + liczbaPoprzedniegoKonca;

        pthread_create(&threads[i], NULL, maths,wartoscWejsciowa );
    }
    while (turnOn) {
        scanf("%c", &awanser);
        if (awanser == 's' || awanser == 'S') {turnOn = 0;}
    }
}