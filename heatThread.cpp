#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<cmath>
#include<ctime>
#include<pthread.h>


using namespace std;

int H,C;
double tinit,c0,c1,c2,delta,Max,numThread;
double cyl[1000][1000];
bool checkDelta[1000][1000];
int flops;
int iterations;

class Point{
public:
int x, y;
Point(int X,int Y){
x = X;
y = Y;
}
int getX()
{
return x;
}
int getY(){
return y;
}
};

void parse(string line)
{
  string copy[9];
  int count = 0;
 
  for(int i=0;i<line.length();i++)
    {
      if(line.at(i) == ' ')
	{
	  count++;
	}
      else
	{
	  copy[count] = copy[count] + line.at(i);
	}
    }
 
  H = atoi(copy[0].c_str());
  C = atoi(copy[1].c_str());
  tinit = atof(copy[2].c_str());
  c0 = atof(copy[3].c_str());
  c1 = atof(copy[4].c_str());
  c2 = atof(copy[5].c_str());
  delta = atof(copy[6].c_str());
  Max = atof(copy[7].c_str());
  numThread = atof(copy[8].c_str());
}
void printArray()
{
  for(int x=0;x<H;x++)
    {
      cout<<" "<<endl;
      for(int y=0;y<C;y++)
	{
	  cout<<cyl[x][y]<<"  ";
	}
      cout<< endl;
    }
}

bool checkConvergence()
{
  bool temp;
  for(int i=0;i<H;i++)
    {
      for(int j=0;j<C;j++)
	{
	  if(checkDelta[i][j])
	    {
	      temp = true;
	    }
	  else
	    {
	      temp = false;
	      return false;
	    }
	}
    }
  return temp;
}

double averageTemp()
{
  double sum = 0;
  for(int i=0;i<H;i++)
    {
      for(int j=0;j<C;j++)
	{
	  sum = sum + cyl[i][j];
	}
    }
  return sum/(H*C);
}



void sim()
{
  iterations = 0;
  double temp;
  while(iterations < Max && !checkConvergence())
    {
      iterations++;
      //printArray();
      for(int i=0;i<H;i++)
	{
	  for(int j=0;j<C;j++)
	    {
	      flops = flops+6;
     	       temp = cyl[i][j];
	      if((i == H/2 && j == C/2) || (i==H/4 && j==C/4) || (i==H-3 && j==C-3))
		{
		  //do nothing
		}
	      else if(i == 0 && j>0 && j < H-1)
		{
		  //no d0 n0 d1 and not on side//top row
		  cyl[i][j] = (cyl[i][j]*c0) + (cyl[i+1][j]*c1)+(cyl[i][j+1]*c1)+(cyl[i][j-1]*c1) + (cyl[i+1][j-1]*c2)+(cyl[i+1][j+1]*c2);
		}
	      else if(i == H-1 && j >0 && j !=H-1)//no d2 n3 d3// bottom row
		{
		  cyl[i][j] = (cyl[i][j]*c0)+(cyl[i][j-1]*c1)+(cyl[i][j+1]*c1)+(cyl[i-1][j]*c1)+(cyl[i-1][j-1]*c2)+(cyl[i-1][j+1]*c2);
		}
	      else if(j == 0 && i > 0 && i < H-1)//1st col
		{
		  cyl[i][j] = (cyl[i][j]*c0)+(cyl[i][C-1]*c1)+(cyl[i][j+1]*c1)+(cyl[i][j-1]*c1)+(cyl[i+1][j+1]*c2)+(cyl[i-1][C-1]*c2)+(cyl[i+1][C-1]*c2)+(cyl[i-1][j+1]*c2)+(cyl[i+1][j-1]*c2);
		}
	      else if(j == C-1 && i > 0 && i<H-1)//last col
		{
		  cyl[i][j] = (cyl[i][j]*c0)+(cyl[i][j-1]*c1)+(cyl[i][0]*c1)+(cyl[i+1][j]*c1)+(cyl[i-1][j]*c1)+(cyl[i-1][j-1]*c2)+(cyl[i+1][j-1]*c2)+(cyl[0][j-1]*c2)+(cyl[0][j+1]*c2);
		}
	      else if(i > 0 && i < H-1 && j > 0 && j < C-1)//middle
		{
		  cyl[i][j] = (cyl[i][j]*c0)+(cyl[i-1][j]*c1)+(cyl[i+1][j]*c1)+(cyl[i][j+1]*c1)+(cyl[i][j-1]*c1)+(cyl[i-1][j+1]*c2)+(cyl[i-1][j-1]*c2)+(cyl[i+1][j+1]*c2)+(cyl[i+1][j-1]*c2);
		}
	      else if(i == H-1 && j == 0)//bot left corner
		{
		  cyl[i][j] = (cyl[i][j]*c0)+(cyl[i-1][j]*c1)+(cyl[i][j+1]*c1)+(cyl[i][C-1]*c1)+(cyl[i-1][j+1]*c2)+(cyl[i-1][C-1]*c2);
		}
	      else if(i == 0 && j==0)//top left
		{
		  cyl[i][j] = (cyl[i][j]*c0)+(cyl[i][j+1]*c1)+(cyl[i+1][j]*c1)+(cyl[i][C-1]*c1)+(cyl[H-1][j]*c1)+(cyl[i+1][j+1]*c2)+(cyl[i+1][C-1]*c2);
		}
	      else if(i == H-1 && j == C-1)//bot right
		{
		  cyl[i][j] = (cyl[i][j]*c0)+(cyl[i-1][j]*c1)+(cyl[i][j-1]*c1)+(cyl[0][C-1]*c1)+(cyl[i-1][j-1]*c2)+(cyl[i-1][0]*c2);
		}
	      else if(i==0 && j== C-1)//top right
		{
		  cyl[i][j] = (cyl[i][j]*c0)+(cyl[i][j-1]*c1)+(cyl[i+1][j]*c1)+(cyl[i][0]*c1)+(cyl[i+1][j-1]*c2)+(cyl[i+1][0]*c2);
		}
	      //	           cout<<temp<<cyl[i][j]<<endl;
	      flops++;
		   if(abs(cyl[i][j]-temp) <= delta )
	    {
	     
	      checkDelta[i][j] = true;
	    }
	    }
	}
      
    }
}

void * cellThread(void* param)
{



}
																	    	      
						  	  

int main(void)
{
  flops = 0;
  int start_s = clock();
  cout<<"enter H C tinit c0 c1 c2 delta max threads"<<endl;
  string line;
  getline(cin,line);
  parse(line);
  // double cylinder[H][C];
  for(int i=0;i<H;i++)
    {
      for(int j=0;j<C;j++)
	{
	  cyl[i][j] = tinit;
	  checkDelta[i][j] = false;
	}
    }
  
  cyl[H/2][C/2] = 40;
  cyl[H/4][C/4] = 0;
  cyl[H-3][C-3] = 0;
  // cout<<"test1"<<endl;
  int numCells = H*C;
  int cellsPerThread = numCells/numThread;
  int count = 0;
  Point point(1,3);
  cout<<point.getX()<<endl;
  
  int x=0;
  int y=0;
  int threadCells[50][50];
  for(int w=0;w<50;w++)
    {
      for(int z=0;z<C;z++)
	{
	  threadCells[w][z] = -1;
	}
    }
  int c = 0;
  while(count<Max)
    {
      while(x<H && y<C)
	{
	  while(c < cellsPerThread)
	    {
	      



    }
  printArray();
  sim();
  printArray();
  if(checkConvergence())
    cout<<"CONVERGES"<<endl;
  cout<<"Avergage: "<<averageTemp()<<endl;
  int stop_s=clock();
  int t = (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000;
  cout<<"time: "<<(stop_s-start_s)/double(CLOCKS_PER_SEC)*1000<<endl;
  cout<<"FLOPS per second: "<<flops/t<<endl;
  cout<<"Iterations: "<<iterations<<endl;
  
  
}
