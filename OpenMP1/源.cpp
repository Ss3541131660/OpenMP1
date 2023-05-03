#include <pthread.h>
#include <iostream>
#include<time.h>
#include <omp.h>
#include<stdio.h>
#include <semaphore.h>
#define NUM_THREADS 4
using namespace std;
int n = 5;
float** A = new float* [n];
float** pA = new float* [n];
//相关内容的定义


void gauss_elimination(float** mat, int n) {
}

int main() {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        A[i] = new float[n];
        pA[i] = new float[n];
    }
 
    for (int i = 0; i < n; i++) {
        A[i][i] = 1.0;//对角线为1.0
        for (int j =0; j < n; j++) {
            if (j >i)A[i][j] = rand() % 10;
            else if(j<i)A[i][j] = 0;
        }
    }
    //上三角矩阵
    cout << "原来的三角矩阵" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    for (int k = 0; k < n; k++) {
        for (int i = k + 1; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i][j] += A[k][j];
            }
        }
    }
    //以上都是矩阵初始化，非必要不要打开

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            pA[i][j] = A[i][j];
        }
    }

    //gauss_elimination(A, n);
    cout << endl;
    cout << "初等变换后矩阵" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    double tmp;
    int i, j, k;


#pragma omp parallel if(NUM_THREADS > 1), num_threads(NUM_THREADS), private(i, j, k, tmp)
    {
        for (k = 0; k < n; ++k) {
            // 串行部分，也可以尝试并行化
#pragma omp single
            {
                tmp = A[k][k];
                for (j = k + 1; j < n; ++j) {
                    A[k][j] =A[k][j] / tmp;
                }
                A[k][k] = 1.0;
            }

            // 并行部分，使用行划分
#pragma omp for
            for (i = k + 1; i < n; ++i) {
                tmp = A[i][k];
                for (j = k + 1; j < n; ++j) {
                    A[i][j] = A[i][j] - tmp * A[k][j];
                }
                A[i][k] = 0.0;
            }
            // 离开for循环时，各个线程默认同步，进入下一行的处理
        }
    }


    cout << endl;
    cout << "并行算法结果" << endl;
    // 输出结果矩阵
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    // 释放内存
    for (int i = 0; i < n; ++i) {
        delete[] A[i];
    }
    delete[] A;


    for (int k = 0; k < n; k++) {
        for (int j = k + 1; j < n; j++) {
            pA[k][j] = pA[k][j] / pA[k][k];
        }
        pA[k][k] = 1.0;
        for (int i = k + 1; i < n; i++) {
            for (int j = k + 1; j < n; j++) {
                pA[i][j] = pA[i][j] - pA[k][j] * pA[i][k];
            }
            pA[i][k] = 0;
        }
    }

    cout << endl;
    cout << "串行算法结果" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << pA[i][j] << " ";
        }
        cout << endl;
    }
}


