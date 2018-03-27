#include<iostream>
#include<fstream>
using namespace std;

ifstream &read(ifstream &ifile, int &word)
{
	ifile >> word;
	return ifile;
}

ifstream &read(ifstream &ifile, string &word)
{
	ifile >> word;
	return ifile;
}

ofstream &write(ofstream &ofile, int str)
{
	ofile << str << " ";
	return ofile;
}

int main()
{
	int numOfV;
	string str;
	for (int q =0; q < 12; q++)
	{
	char infile[100],outfile[100];
	sprintf(infile,"hamiltonInput/%d",q+1);
	ifstream inn(infile);
	read(inn,numOfV);
	inn.close();
	cout << numOfV << endl;
	sprintf(infile,"hamiltonOutput/output/sat%d.out",q+1);
	sprintf(outfile,"hamiltonOutput/output/path/path%d",q+1);
	ifstream in(infile);
	ofstream out(outfile);
	if (!in) cout << "fail to open" << infile << endl;
	string satResult;
	read(in,satResult);
	if (satResult == "SAT")
	{
	int *p = new int[numOfV+1];
	int i;
	while (read(in,i))
	{
		if (i > 0)
		p[(i-1)/numOfV+1] = (i-1)%numOfV+1;
	
	}
	for (int i = 1; i <= numOfV; i++)
		write(out,p[i]);
	}
	else write(out,0);
	in.close();
	out.close();
	}
	return 0;
}
