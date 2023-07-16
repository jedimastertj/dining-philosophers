#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t forks[5];
sem_t bowls[2];

void accessBowl(int bNumber, int pNumber) {
    sem_wait(&bowls[bNumber]);
    printf("philosohper %d accessed bowl %d \n", pNumber, bNumber);
}

void leaveBowl(int bNumber, int pNumber) {
    sem_post(&bowls[bNumber]);
    printf("philosohper %d left bowl %d \n", pNumber, bNumber);
}

void pickFork(int fNumber) {
    sem_wait(&forks[fNumber]);
}

void putFork(int fNumber) {
    sem_post(&forks[fNumber]);
}

void takeForks(int pNumber) {
    int rfNumber = pNumber;
    int lfNumber = (pNumber+1)%5;
    if (pNumber == 4) {
        pickFork(rfNumber);
        printf("philosohper %d picked fork %d on their right \n", pNumber, rfNumber);
        pickFork(lfNumber);
        printf("philosohper %d picked fork %d on their left \n", pNumber, lfNumber);
    } else {
        pickFork(lfNumber);
        printf("philosohper %d picked fork %d on their left \n", pNumber, lfNumber);
        pickFork(rfNumber);
        printf("philosohper %d picked fork %d on their right \n", pNumber, rfNumber);
    }
}

void leaveForks(int pNumber) {
    int rfNumber = pNumber;
    int lfNumber = (pNumber+1)%5;
    putFork(rfNumber);
    printf("philosohper %d put fork %d on their right \n", pNumber, rfNumber);
    putFork(lfNumber);
    printf("philosohper %d put fork %d on their left \n", pNumber, lfNumber);
}

void* algo(void* x) {
    while (1) {
        int pNumber = *((int*)x);
        int bNumber = (pNumber%2 == 0) ? 0 : 1;
        printf("philosopher %d is hungry \n", pNumber);
        takeForks(pNumber);
        accessBowl(bNumber, pNumber);
        printf("philosopher %d is eating \n", pNumber);
        leaveBowl(bNumber, pNumber);
        leaveForks(pNumber);
    }
}

int main(int argc, char const *argv[]){
    pthread_t philosophers[5];
    int nums[] = {0, 1, 2, 3, 4};
    
    for (int i = 0; i < 5; i++) {
        sem_init(&forks[i], 0, 1);
    }

    for (int i = 0; i < 2; i++) {
        sem_init(&bowls[i], 0, 1);
    }

    for (int i = 0; i < 5; i++) {
        pthread_create(&philosophers[i], NULL, algo, (void*)&nums[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < 2; i++) {
        sem_destroy(&bowls[i]);
    }

    for (int i = 0; i < 5; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}
