
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <sys/wait.h>
#include <sys/shm.h>
using namespace std;

int N;
int memID;

struct dret
{
  int TRAZIM[10];
  int BROJ[10];
}*dretva;

void memRelease()
{
   shmdt((dret*) dretva);
   shmctl(memID, IPC_RMID, NULL);
   cout << "Memory released!" << endl;
   exit(0);
}

void KO(int g)
{
  int max = 0;
  dretva->TRAZIM[g] = 1;
  for (int r = 0; r < g; r++)
  {
    if (dretva->BROJ[r] > max)
        max = dretva->BROJ[r];
  }
  dretva->BROJ[g] = max + 1;
  dretva->TRAZIM[g] = 0;
  for(int j = 0; j < N; j++)
  {
    while(dretva->TRAZIM[j] != 0);
    while(dretva->BROJ[j] != 0 && (dretva->BROJ[j] < dretva->BROJ[g] || (dretva->BROJ[j] == dretva->BROJ[g] && j < g)));
  }
}

void NKO(int o)
{
  dretva->BROJ[o] = 0;
}

void proc (int i)
{
  int k, l;
  for (k = 1; k < N+1; k++)
  {
    KO(i);
    for (l = 1; l < 6; l++)
    {
      cout << "Process " << i+1 << ": K.O. no. " << k << " (" << l << "/5)" << endl;
    }
    NKO(i);
    sleep(1);
  }
}

int main(int argc, char *argv[])
{
  memID = shmget(IPC_PRIVATE, sizeof(int)*100, 0600);
  if(memID == -1)
  {
     cout << "No memory!" << endl;
     exit(1);
  }

  N = atoi(argv[1]);

  dretva = (dret*)shmat(memID, NULL, 0);
  
  //sigset(SIGINT, memRelease);

  int a, b;
  int temp[N];
  for(int c = 0; c < N; c++)
    temp[c] = c; 

  for(a = 0; a < N; a++)
  {
    if(fork() == 0)
    {
      proc(temp[a]);
      exit(0);
    }
  }

  for(b = 0; b < N; b++)
    wait(NULL);

  memRelease();
  return 0;
}
