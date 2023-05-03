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
//������ݵĶ���


void gauss_elimination(float** mat, int n) {
}

int main() {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        A[i] = new float[n];
        pA[i] = new float[n];
    }
 
    for (int i = 0; i < n; i++) {
        A[i][i] = 1.0;//�Խ���Ϊ1.0
        for (int j =0; j < n; j++) {
            if (j >i)A[i][j] = rand() % 10;
            else if(j<i)A[i][j] = 0;
        }
    }
    //�����Ǿ���
    cout << "ԭ�������Ǿ���" << endl;
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
    //���϶��Ǿ����ʼ�����Ǳ�Ҫ��Ҫ��

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            pA[i][j] = A[i][j];
        }
    }

    //gauss_elimination(A, n);
    cout << endl;
    cout << "���ȱ任�����" << endl;
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
            // ���в��֣�Ҳ���Գ��Բ��л�
#pragma omp single
            {
                tmp = A[k][k];
                for (j = k + 1; j < n; ++j) {
                    A[k][j] =A[k][j] / tmp;
                }
                A[k][k] = 1.0;
            }

            // ���в��֣�ʹ���л���
#pragma omp for
            for (i = k + 1; i < n; ++i) {
                tmp = A[i][k];
                for (j = k + 1; j < n; ++j) {
                    A[i][j] = A[i][j] - tmp * A[k][j];
                }
                A[i][k] = 0.0;
            }
            // �뿪forѭ��ʱ�������߳�Ĭ��ͬ����������һ�еĴ���
        }
    }


    cout << endl;
    cout << "�����㷨���" << endl;
    // ����������
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    // �ͷ��ڴ�
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
    cout << "�����㷨���" << endl;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << pA[i][j] << " ";
        }
        cout << endl;
    }
}


