#include<iostream>
#include<vector>
#include<set>
#include<fstream>
#include<time.h>
using namespace std;
int G[501][501];
bool visited[501];
int outDegree[501];
int inDegree[501];
vector<int> path;
int numOfV,numOfE,i;
time_t start,end;
#define LIMITTIME 1200
void printPath(vector<int>&);

ifstream &read(ifstream &ifile, int &word)
{
	ifile >> word;
	return ifile;
}

int checkDegree(int * array)
{
	int count = 0;
	for (int j = 1; j <= numOfV; j++)
	{
		if (!array[j])
		count++;
	}
	return count;
}


void sortPos(vector<int>&nextPos,int begin, int end)
{
	if (begin >= end)
		return;
	int pivot = outDegree[nextPos[begin]];
	int i = begin , j = end;
	while (i < j)
	{
		while (i < j && outDegree[nextPos[i]] > pivot)
			i++;
		while (i < j && outDegree[nextPos[j]] < pivot)
			j--;
		if (i < j)
		{
			int tmp = nextPos[i];
			nextPos[i] = nextPos[j];
			nextPos[j] = tmp;
			i++;
			j--;
		}
	}
	sortPos(nextPos,begin,i-1);
	sortPos(nextPos,i+1,end);
}

vector<int> genNext(int&numOfV,int G[][501],int last)
{
	vector<int>nextPos;
	int next = 1;
	while (next <=numOfV && (visited[next] || !G[last][next])) next++;
	if (next == numOfV+1) return nextPos;
	nextPos.push_back(next);
	for (int i = next+1; i <= numOfV; i++)
	{
		if (!visited[i]&& G[last][i])
			nextPos.push_back(i);
	}
	sortPos(nextPos,0,nextPos.size()-1);
	return nextPos;
}

bool hamiltonRecursive(int G[][501], int& numOfV, vector<int>&path, int pos)
{
	if ((difftime(time(NULL),start)) > LIMITTIME) return false;

	if (pos == numOfV)
	{
			return true;
	}
	
	else
	{
		if (pos == 0)
		{
			//若有两个出度为0的点，则无解
			if (checkDegree(outDegree) >= 2)return false;
			
			vector<int> nextPos;
			for (int i = 0; i < numOfV; i++) nextPos.push_back(i);
			sortPos(nextPos,0,numOfV-1);
			for (int j = 0 ;j < nextPos.size(); j++)
			{
				int n = nextPos[j];
				visited[n] = true;
				path.push_back(n);
				
				if (hamiltonRecursive(G,numOfV,path,pos+1))
					return true;
				
				path.pop_back();
				visited[n] = false;
			}
		}
		else
		{
			//若有入度为0的点，则无解
			if (checkDegree(inDegree) >= 1) return false;
			//若有两个出度为0的点，则无解
			if (checkDegree(outDegree) >= 2) return false;
			vector<int> nextPos = genNext(numOfV, G, path[pos-1]);
			if (nextPos.size() == 0) return false;
			else
			for (int j = 0 ;j < nextPos.size(); j++)
			{
				int n = nextPos[j];
				visited[n] = true;
				path.push_back(n);

				int curPos = path[pos-1];
				for (int v = 1; v <= numOfV; v++)
				{
					if (G[v][curPos]) outDegree[v]--;
					if (G[curPos][v]) inDegree[v]--;
				}
				
				if (hamiltonRecursive(G,numOfV,path,pos+1))
					return true;
				
				for (int v = 1; v <= numOfV; v++)
				{
					if (G[v][curPos]) outDegree[v]++;
					if (G[curPos][v]) inDegree[v]++;
				}
				
				path.pop_back();
				visited[n] = false;
			}
		}
		return false;
	}
}


int findStart(int G[][501], int& numOfV)
{
	int start = -1;
	for (int j = 1; j <= numOfV; j++)
	{
		bool flag = false;
		for (int i = 1; i <= numOfV; i++)
			if (G[i][j])
			{
				flag = true;
				break;
			}
		if (!flag)
		{
			if (start == -1)
				start = j;//将没有入度的点作为起点
			else return -2;//多个没有入度的点则无解
		}	
	}
	return start;
}

bool solveHamilton(int G[][501], int& numOfV, vector<int>&path)
{
	int start = findStart(G, numOfV);
	cout << "start : "<< start << endl; 
	if (start == -2)
		return false;
	else if (start == -1)
	{
	if (hamiltonRecursive(G,numOfV,path,0))
		return true;
	else return false;
	}
	else 
	{
		path.push_back(start);
		visited[start] = true;
		if (hamiltonRecursive(G,numOfV,path,1))
		return true;
		else return false;
	}
}

void printPath(vector<int>&path)
{
	for (int i = 0 ; i < path.size(); i++)
		cout << path[i] << " ";
	cout << endl;
}


int main()
{
	for (int q = 0; q < 14;q++)
	{
		char infile[30];
		sprintf(infile,"..\\hamiltonInput\\%d",q+1);
		ifstream in(infile);
		int numOfV,numOfE,i;
		read(in,numOfV);
		read(in,numOfE);
		
		//initial
		
		for (i = 1; i <= numOfV; i++)
		{
		visited[i] = false;
		outDegree[i] = 0;
		inDegree[i] = 0;
		for (int j = 1; j <= i; j++)
			G[i][j] = G[j][i] = 0;
		}
		path.clear();
		//graph
		for (i = 0; i < numOfE; i++)
		{
			int v1,v2;
			read(in,v1);
			read(in,v2);
			G[v1][v2] = 1;
			outDegree[v1]++;
		}
		
		
		bool findPath = false;
		
		
		cout << "case" << q+1 << " : ";
		
		start = time(NULL);
		findPath = solveHamilton(G,numOfV,path);
		end = time(NULL);
		
		if (difftime(end,start) <= LIMITTIME)
		{
		if (findPath)
		printPath(path);
		else cout << "There's no solution for the case" << endl;
		cout << "it took " << difftime(end,start) << " seconds to solve it." << endl << endl;
		}
		else
			cout << "it exceeds the time limit " << LIMITTIME << " seconds." << endl << endl;
	}
	return 0;
}
