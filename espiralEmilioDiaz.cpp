// Universidad del Valle de Guatemala
// Programacion de Microprocesadores
// Emilio Diaz 
// 15316
// ---------------------------------------
// En colaboracion con Byron Mota y Ana Lucia Diaz
#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define MAXSZ 400
#define NUMTHREADS 8
/*
*/
using namespace cv;
//Imagen a ser modificada
Mat espiral(MAXSZ, MAXSZ, CV_8UC1 );
//Tamano de la imagen que se creara
//float mat[MAXSZ][MAXSZ];
int isPrime(int);
int a[MAXSZ][MAXSZ],z;
//thread function
void *threadFunction(void *threadid){
   std::cout<<"thread called\n";
   long tid;
   tid = (long)threadid;
   int max = tid*250;
   for (int i=250*(tid-1); i<max;i++){
	isPrime(i);
   }
   pthread_exit((void*) 0);
}
int main() {
	//direccion inicial para que se mueva a la derecha
    int initial_direction = UP, n = MAXSZ, p = 1, num = MAXSZ * MAXSZ + 1;    
	//rows y cols
    int r, c ;
    int row_right = 0, column_down = n - 1, row_left = n - 1, column_up = 0, colorVal = 0;
    //MATRIZ a 0
    for (r = 0; r < MAXSZ; r++) {
        for (c = 0; c < MAXSZ; c++)
            a[r][c] = 0;
    }
    //Generar espiral
    while (p != n * n + 1) {
        if (initial_direction == UP) {
            //Mover a la derecha
            r = row_right++;
            for (c = 0; c < n; c++) {
                if (a[r][c] == 0)
                    a[r][c] = num - p++;
            }
            initial_direction = RIGHT;
        } else if (initial_direction == RIGHT) {
            //Mover hacia abajo
            c = column_down--;
            for (r = 0; r < n; r++) {
                if (a[r][c] == 0)
                    a[r][c] = num - p++;
            }
            initial_direction = DOWN;
        } else if (initial_direction == DOWN) {
            //Mover hacia la izqueirda
            r = row_left--;
            for (c = n - 1; c >= 0; c--) {
                if (a[r][c] == 0)
                    a[r][c] = num - p++;
            }
            initial_direction = LEFT;
        } else if (initial_direction == LEFT) {
            //Mover hacia arriba
            c = column_up++;
            for (r = n - 1; r >= 0; r--) {
                if (a[r][c] == 0)
                    a[r][c] = num - p++;
            }
            initial_direction = UP;
        }
    }
    //creacion de threads
    pthread_t threads[NUMTHREADS];
    //Se define la variable que contendra el valor que retorne el thread
    void* return_status; 
	int t; 
    for (t = 1; t <= NUMTHREADS+1; t++){
        pthread_create(&threads[t], NULL, threadFunction, (void *) t);
    }
    std::cout<<"building image...\n";
    for (r = 0; r < MAXSZ; r++) {
        for (c = 0; c < MAXSZ; c++) {
            a[r][c] = isPrime(a[r][c]);
            uchar value = (uchar) a[r][c];
		    espiral.ptr<uchar>(r)[c] = value;
        }
    }   	
    std::cout<<"ending theads...\n";
    for (t = 0; t <= NUMTHREADS; t++){
        pthread_join(threads[t], &return_status);
    }
    std::cout<<"-- threads ended --";
        imwrite("espiralPrimos.png", espiral);
std::cout<<"Imagen guardada con exito como espiralPrimos.png";
    return 0;
}
//funcion dada para un numero primo
int isPrime(int number) {
    for (unsigned int i = 2; i < number; i++) {
        if (number % i == 0) {
            return 0;
        }
    }
    //number is prime	
    return 255;
}


