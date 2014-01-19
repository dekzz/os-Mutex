#include <iostream>
#include <cstdlib>
#include <pthread.h>
using namespace std;

int PRAVO, ZASTAVICA[2];

void *KO (int i, int j);
void *NKO (int i, int j);
void *th (void *x);

int main ()
{
  int p, q;
  int temp[2] = {0,1} ;
  pthread_t tid[2];
  
  for(p = 0; p < 2; p++)
  {
    if(pthread_create(&tid[p], NULL, th, &temp[p]) != 0)
    {
      cout << "Creating thread " << p+1 << " failed!" << endl;
      exit(1);
    }
  }

  for(q = 0; q < 2; q++)
     pthread_join(tid[q], NULL);

  return 0;
}

void *KO (int i, int j)
{
  ZASTAVICA[i] = 1;
  while (ZASTAVICA[j] != 0)
  {
    if(PRAVO == j)
    {
      ZASTAVICA[i] = 0;
      while(PRAVO == j);
      ZASTAVICA[i] = 1;
    }
  }
}

void *NKO (int i, int j)
{
  PRAVO = j;
  ZASTAVICA[i] = 0;
}

void *th (void *x)
{
  int i = *((int*)x);
  int k, l;
  for (k = 1; k < 6; k++)
  {
    KO(0, 1);
    for (l = 1; l < 6; l++)
    {
      cout << "Thread " << i+1 << ": K.O. no. " << k << " (" << l << "/5)" << endl;
    }
    NKO(0, 1);
    sleep(1);
  }
}
