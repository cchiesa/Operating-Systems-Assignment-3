#include<iostream>
#include<queue>
#include<cstdlib>
#include<cstdio>
#include<pthread.h>
#include<math.h>
#include<vector>
#include<semaphore.h>
#include<unistd.h>
#include<time.h>

//#include<SafeQueue.h>
//#include<mutex>

using namespace std;
//queue <int> FilterQ;
vector< queue<int> > vec;//vector of qs
int numQ; //keep track of q number
queue <int> sequenceQ;
sem_t sem;
int numThread;//keep track of thread number
sem_t semQ;//sem for checking if q is emtpy
pthread_t threads[1500];
int numPrimes;
int count;
void * filterThread(void* param)
{
  
  
  int filter = *((int*)param);
  cout<<numThread-1<<" prime is: "<<filter<<endl;//1st prime is 2
  //look for next prime then make new filter thread
  sem_wait(&semQ);//wait unil q not empty
  sem_wait(&sem);
  
  count++;
  if(numThread-1 == numPrimes){
    cout<<"end"<<endl;
    pthread_exit(NULL);
    exit(0);
  }
  int *arg =(int*) malloc(sizeof(*arg));
  int nextPrime =  vec[numQ].front();
  *arg = nextPrime;
  //  cout<<"next Prime is: "<<nextPrime<<endl;
  // vec[numQ].pop();//using numQ so wait and post
  int copyNumQ = numQ;//save numQ so +1 is goin to next thread
  numQ++;//increment numQ for next thread
  numThread++;//incr thread num
  
  //  cout<<"nextPrime"<<nextPrime<<endl;
  pthread_create(&threads[numThread],NULL,filterThread,arg);//make next thread
  vec[numQ-1].pop();
  sem_post(&sem);
  /// sem_post(&semQ);//increment sem for Q
  //cout<<copyNumQ<<"copy numq"<<endl;
  
  while(!vec[copyNumQ].empty())
    {
      sem_wait(&sem);
      int nextInt = vec[copyNumQ].front();
      //cout<<nextInt<<endl;
     
      vec[copyNumQ].pop();
      sem_post(&sem);
      if(nextInt%filter == 0)//if not prime
	{
	  //dont pass to next q
	}
      else
	{
	  // pass to next q
	  sem_wait(&sem);
	  vec[copyNumQ+1].push(nextInt);
	  // cout<<nextInt<<endl;
	  /// sem_post(&semQ);
	  sem_post(&sem);
	}
    }
  pthread_exit(NULL);
  

}

//filter thread 2
void * twoThread(void * param)//takes filter and number to check
{
  // cout<<"2"<<endl;
  //make three thread
  sem_wait(&sem);
  numThread++;//now at 3
  int nt = numThread;
  sem_post(&sem);
  cout<<"1st prime is: 2"<<endl;
  pthread_create(&threads[numThread],NULL,filterThread,&numThread);
  //check for numbers not divisble by 2 and push on q
  while(!sequenceQ.empty())
    {
      sem_wait(&sem);
     int n =  sequenceQ.front();
     
     sequenceQ.pop();
     sem_post(&sem);
     //     cout<<n<<endl;
  if(n%2 != 0)
    {
      //      cout<<n<<endl;
      //if not prime
    
      //     sem_wait(&sem); 
      //push on queue
      sem_wait(&sem);
      vec[0].push(n);
      sem_post(&sem);
      sem_post(&semQ);//tell 3 q is not empty
      // sem_post(&sem);
      
    }
    }
  pthread_exit(NULL);
}



int main(void)
{
  count = 0;
  numThread = 2;
  sem_init(&sem,0,1);
  sem_init(&semQ,0,0);
  numQ = 0;
  cout<<"Enter number: "<<endl;
  int n;
  cin >> n;
  numPrimes = n;//num of threads
  //add qs to vector
 
  for(int j=0;j<1500;j++)
    {
      //  cout<<"test"<<endl;
      vec.push_back(queue<int>());
    }
  
  pthread_create(&threads[numThread],NULL,twoThread,(void*)NULL);
  
  for(int i=4;i<10000;i++)
    {
      sem_wait(&sem);
      sequenceQ.push(i);
      sem_post(&sem);
    }
  
  
  
  pthread_detach(*threads);
  sleep(5);
  
}
    
