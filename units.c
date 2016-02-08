#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


extern pthread_mutex_t shop_door[4];
extern int shop_stock[4];

/*потоковая функция покупателя*/
void * buyer_f (void *arg){
    int r,i;
    int purchase;
    int buyer_bag=0;
    int my_num = *((int *) arg) ;
    /*Выполнение цикла, пока покупатель не насытился*/
    while (buyer_bag < 200){
     /*Поочерёдно опрашиваем магазины*/
     for (i=0;i<4;i++){
     r=pthread_mutex_trylock(&shop_door[i]);
     if (r!=0){
         printf("Я, покупатель № %d, пытался зайти в магазин № %d ,но он был занят\n",my_num + 1 ,i+1);
         continue;
        }
     /*Если магазин свободен и не пуст, то забираем из него случайное число товара, либо всё,если число больше остатка*/
     else {
         if (shop_stock[i]==0){
             printf("Я, покупатель № %d, зашёл в магазин № %d , но он был пуст\n",my_num + 1, i+1);
             pthread_mutex_unlock(&shop_door[i]);
             sleep(1);
             continue;
         }
         purchase= 50+rand() % 30;
         if (shop_stock[i]>=purchase){
             buyer_bag=buyer_bag+purchase;
             shop_stock[i]=shop_stock[i]-purchase;
             printf ("Я, покупатель № %d, купил в магазине № %d %d товара. В магазине осталось %d\n", my_num+1, i+1,purchase,shop_stock[i]);
         }
         else if (shop_stock[i]<purchase) {
             buyer_bag=buyer_bag+shop_stock[i];
             printf ("Я, покупатель № %d, купил в магазине № %d %d товара. В магазине осталось 0 \n", my_num+1, i+1,shop_stock[i]);
             shop_stock[i]=0;


         }
     pthread_mutex_unlock(&shop_door[i]);

     if (buyer_bag>200){
         break;
     }
     sleep(3);
     }
    }
    }
  printf("Я, покупатель № %d, купил достаточно\n",my_num+1);
  return NULL;
}


/*Потоковая функция загрузчика*/
void * loader_f (void){
     int r,i;
     int load=0;
     while (1){
     for (i=0;i<4;i++){
     r=pthread_mutex_trylock(&shop_door[i]);
     if (r!=0){
         printf("Я, загрузчик , пытался зайти в магазин № %d ,но он был занят\n",i+1);
         continue;
        }
     else {
         load= 70+rand()%50;
         shop_stock[i]=shop_stock[i]+load;
         printf("Я, загрузчик , добавил в магазин № %d %d товара\n", i+1,load);
         pthread_mutex_unlock(&shop_door[i]);
         sleep(5);
     }
    }
    }
return NULL;
}


