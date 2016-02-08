#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "ad.h"

int shop_stock[4]; //массив с значением остатка для магазинов
int buyer[5];
pthread_mutex_t shop_door[4]; // отдельный mutex для каждого магазина

int main(void)
{   int i,r;
    int n[5];
    pthread_t buyer[5],loader;
    /*Инициализируем массив с номерами покупателей*/
    for (i=0;i<5;i++){
        n[i]=i;
    }
    /*Создаём  mutex для каждого магазина*/
    for (i=0;i<4;i++){
        r=pthread_mutex_init(&shop_door[i],NULL);
        if(r!=0){
            perror("Инициализация mutex");

        }
        /*Заполняем магазины случайными начальными значениями */
        shop_stock[i]=(100 + rand() % 50  );
        printf("я магазин № %d, в начале дня во мне %d\n" ,i+1, shop_stock[i]);

    }
    /*Создаём 5 покупателей*/
    for (i=0;i<5;i++){
        r=pthread_create(&buyer[i],NULL,buyer_f,&n[i]);
        if (r!=0){
            perror("Создание потока");
        }

    }
    /*Создаём загрузчки*/
    r=pthread_create(&loader,NULL,loader_f,NULL);
    if (r!=0){
        perror("Создание потока");
    }
    /*Ожидаем завершения работы всех покупателей*/
    for (i=0;i<5;i++){
        pthread_join(buyer[i],NULL);
    }

    printf ("Все покупатели насытелись\n");
    return 0;
}

