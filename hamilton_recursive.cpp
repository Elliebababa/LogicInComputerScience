#include<iostream>
#include<vector>
#include<set>
#include<fstream>
#include<time.h>
using namespace std;
int G[501][501];
//set<int> visited;
bool visited[501];
int outDegree[501];
vector<int> path;
int numOfV,numOfE,i;

void printPath(vector<int>&);

ifstream &read(ifstream &ifile, int &word)
{
	ifile >> word;
	return ifile;
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
		//cout << i << visited[i] << G[last][i] << endl;
		if (!visited[i]&& G[last][i])
			nextPos.push_back(i);
	}
	sortPos(nextPos,0,nextPos.size()-1);
/*
	for (int i = 0; i < nextPos.size() ; i++)
		cout << nextPos[i] << " " << outDegree[nextPos[i]] <<  " ";
	cout << endl;
	cout << "next : " << next << " " << visited[next] << endl;
*/
	return nextPos;
}

bool hamiltonRecursive(int G[][501], int& numOfV, vector<int>&path, int pos)
{
	//cout << endl << "pos " << pos << " : ";
	//for (int i = 0; i < numOfV; i++)
	//	cout << visited[i] << " ";
	//cout << endl;
	//cout << "current path";
	//printPath(path);
	//cout << endl;
	if (pos == numOfV)
	{//check if it forms a circuit
		//if (G[path[pos-1]][path[0]])
			return true;
		//else return false;
	}
	
	else
	{
		if (pos == 0)
		{
			for (int i = 0; i < numOfV; i++)
			{
				visited[i+1] = true;
				path.push_back(i+1);
				
				for (int v = 1; v <= numOfV; v++)
					if (G[v][i+1]) outDegree[v]--;
				
				if (hamiltonRecursive(G,numOfV,path,pos+1))
					return true;
				
				for (int v = 1; v <= numOfV; v++)
					if (G[v][i+1]) outDegree[v]++;
				
				path.pop_back();
				visited[i+1] = false;
			}
		}
		else
		{
			vector<int> nextPos = genNext(numOfV, G, path[pos-1]);
			//int n = findNext(numOfV, G, path[pos-1]);
			if (nextPos.size() == 0) return false;
			else
				for (int j = 0 ;j < nextPos.size(); j++)
			{
				int n = nextPos[j];
				visited[n] = true;
				path.push_back(n);
				for (int v = 1; v <= numOfV; v++)
					if (G[v][n]) outDegree[v]--;
				
				if (hamiltonRecursive(G,numOfV,path,pos+1))
					return true;
				
				for (int v = 1; v <= numOfV; v++)
					if (G[v][n]) outDegree[v]++;
				
				path.pop_back();
				visited[n] = false;
			}
		}
		/*
		for (int i = 0 ; i < numOfV; i++)
		{
			if (!visited[i+1] && (pos == 0 || G[path[pos-1]][i+1]))
			{
				visited[i+1] = true;
				path.push_back(i+1);
				if (hamiltonRecursive(G,numOfV,path,pos+1))
					return true;
				path.pop_back();
				visited[i+1] = false;
			}
		}*/
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
	int fileNum = 14;
	for (int q = 0; q < 14;q++)
	//int q = 20;	
	{
		char infile[30];
		sprintf(infile,"hamiltonInput/%d",q+1);
		ifstream in(infile);
		int numOfV,numOfE,i;
		read(in,numOfV);
		read(in,numOfE);
		
		//initial
		
		for (i = 1; i <= numOfV; i++)
		{
		visited[i] = false;
		outDegree[i] = 0;
		for (int j = 1; j <= i; j++)
			G[i][j] = G[j][i] = 0;
		}
		//graph
		for (i = 0; i < numOfE; i++)
		{
			int v1,v2;
			read(in,v1);
			read(in,v2);
			//cout << v1 << " " << v2 << endl;
			G[v1][v2] = 1;
			outDegree[v1]++;
		}
		
		
		time_t start,end;
		bool findPath = false;
		
		
		cout << "case" << q+1 << " : ";
		
		start = time(NULL);
		findPath = solveHamilton(G,numOfV,path);
		end = time(NULL);
		
		if (findPath)
		printPath(path);
		else cout << "There's no solution for the case" << endl;
		cout << "it took " << difftime(end,start) << " seconds to solve it." << endl << endl;
	}
	return 0;
}
