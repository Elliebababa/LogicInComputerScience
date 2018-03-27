#include<iostream>
#include<vector>
#include<string>
#include<fstream>
using namespace std;
int G[501][501];
int fileNum = 14;
vector<string> CNF;


ifstream &read(ifstream &ifile, int &word)
{
	ifile >> word;
	return ifile;
}

ofstream &write(ofstream &ofile, string str)
{
	ofile << str << endl;
	return ofile;
}

void satEncoding(int numOfV, int G[501][501])
{
	//X_i,j denotes that passing through point_j at the i th potision of the path

	//the first condition, every point should appear once
	for (int j = 1; j <= numOfV; j++)
	{
		string str ="";
		char buf[30];
		for (int i = 1; i <= numOfV; i++)
		{
			sprintf(buf,"%d ",numOfV*(i-1)+j);
			string st(buf);
			str+=st;
		}
		str +="0";
		CNF.push_back(str);
	}
	
	//the second condition, there's no points appear twice on the path
	for (int j = 1; j <= numOfV; j++)
	{
		for (int i = 1; i < numOfV; i++)
		{
			for (int k = i+1; k <= numOfV; k++)
			{
				string str="";
				char buf[30];
				sprintf(buf,"-%d -%d ",numOfV*(i-1)+j,numOfV*(k-1)+j);
				string st(buf);
				str+=st;
				str += "0";
				CNF.push_back(str);
			}
		}
	}
	
	//the third condition, every position on the path must pass through a point
	for (int i = 1; i <= numOfV; i++)
	{
		string str ="";
		char buf[30];
				
		for (int j = 1; j <= numOfV; j++)
		{
			sprintf(buf,"%d ",numOfV*(i-1)+j);
			string st(buf);
			str+=st;
		}
		str +="0";
		CNF.push_back(str);
	}
	
	//the fourth condition, no two points occupy the same position on the path
	for (int i = 1; i <= numOfV; i++)
	{
		for (int j = 1; j < numOfV; j++)
		{
			for (int k = j+1; k <= numOfV; k++)
			{
				string str="";
				char buf[30];
				sprintf(buf,"-%d -%d ",numOfV*(i-1)+j,numOfV*(i-1)+k);
				string st(buf);
				str+=st;
				str += "0";
				CNF.push_back(str);
			}
		}
	}
	//fifth condition
	for (int i= 1; i <= numOfV; i++)
	{
		vector<int>out;
		for (int j = 1; j <= numOfV; j++)
		{
			if (G[i][j])
			{
				out.push_back(j);
			}
		}
			for (int k = 1; k < numOfV; k++)
				{
				string str="";
				char buf[30];
				sprintf(buf,"-%d ",numOfV*(k-1)+i);
				string st(buf);
				str+=st;
				for (int q = 0; q < out.size(); q++)
				{
					sprintf(buf,"%d ",numOfV*(k)+out[q]);
					string st(buf);
					str+=st;
				}
				str += "0";
				CNF.push_back(str);
				}
	}
	
}

void printCNF(int numOfV,ofstream&out)
{
	int s = CNF.size();
	char buf[100];
	sprintf(buf,"p cnf %d %d",numOfV*numOfV,s);
	write(out,buf);
	for (int i = 0; i < s; i++)
		write(out,CNF[i]);
}


int main()
{	
	for (int q = 0; q < 12;q++)
	{
		
		char infile[30],outfile[30];
		sprintf(infile,"..\\hamiltonInput\\%d",q+1);
		sprintf(outfile,"hamiltonOutput\\sat%d.in",q+1);
		ifstream in(infile);
		ofstream out(outfile);
		int numOfV,numOfE,i;
		read(in,numOfV);
		read(in,numOfE);
		
		//initial
		CNF.clear();
		for (int i = 1; i <= numOfV; i++)
		{
		for (int j = 1; j <= i; j++)
			G[i][j] = G[j][i] = 0;
		}
		//graph input
		for (i = 0; i < numOfE; i++)
		{
			int v1,v2;
			read(in,v1);
			read(in,v2);
			G[v1][v2] = 1;
		}
		
		satEncoding(numOfV,G);
		printCNF(numOfV,out);
		in.close();
		out.close();
	}
	return 0;
}
