#include <bits/stdc++.h>
#include <stdio.h>
#include <gmp.h>
#include <string.h>

using namespace std;

class Modular{
public:
	vector<char> number;
	Modular(){
		// cout<<"Empty\n";
	}
	Modular(char no[]){
		for(int i=strlen(no)-1;i>=0;i--)
			number.push_back(no[i]);
	}

	// Modular(int no)
	// {
	// 	vector<int>::iterator it;
	// 	while(no!=0)
	// 	{
	// 		it = number.begin();
	// 		number.insert(it,no%10);
	// 		no=no/10;
	// 	}
	// }

	void print()
	{
		for(int i=number.size()-1;i>=0;i--)
			cout<<number[i];
		cout<<"\n";
	}

	void clear()
	{
		number.clear();
	}

	void copy(Modular *mtemp)
	{
		number.clear();
		for(int i=0;i<mtemp->number.size();i++)
			number.push_back(mtemp->number[i]);
	}

	
};

void add(Modular *m1,Modular *m2,Modular *msum)
	{
		int size;
		if(m1->number.size()<m2->number.size())
			size=m1->number.size();
		else
			size = m2->number.size();
		int carry=0;
		for(int i=0;i<size;i++)
		{
			int temps = (int(m1->number[i]-48)+int(m2->number[i]-48)+carry)%10;
			msum->number.push_back(char(temps+48));
			carry = (int(m1->number[i]-48)+int(m2->number[i]-48)+carry)/10;
		}
		while(size<m2->number.size())
			msum->number.push_back(m2->number[size++]);
		while(size<m1->number.size())
			msum->number.push_back(m1->number[size++]);
	}

void sub(Modular *m1,Modular *m2,Modular *msum)
	{
		int size;
		Modular *top;
		Modular *bottom;
		if(m1->number.size()<m2->number.size())
		{
			size=m1->number.size();
			top = m2;
			bottom = m1;
		}
		else if(m1->number.size()>m2->number.size())
		{
			size = m2->number.size();
			top = m1;
			bottom = m2;
		}
		else
		{
			int flag=0;
			size=m1->number.size();
			for(i=size;i>=0;i++)
				if(m1->number[i] > m2->number[i])
				{
					flag=1;
					top = m1;
					bottom = m2;
					break;
				}
				else if(m2->number[i] > m1->number[i])
				{
					flag=1;
					top = m2;
					bottom = m1;
					break;
				}
				else
				{
					continue;
				}
			if(flag == 0)
				cout<<"Equal numbers..............\n"
				cout<<0;
				return;
		}
		for(int i=0;i<size;i++)
		{
			if(top->number[i]<bottom->number[i])
			{
				int k=i+1;
				while(1)
				{
					if(top->number[k] > 0)
						top->number[k]=top->number[k]-1;
				}
			}
		}
		while(size<m2->number.size())
			msum->number.push_back(m2->number[size++]);
		while(size<m1->number.size())
			msum->number.push_back(m1->number[size++]);
	}

void mul(Modular *m1,Modular *m2,Modular *mprdt)
{
	Modular mtemp;
	mprdt->clear();
	int tempc;
	cout<<"============Multiplication===============\n";
	// m1->print();
	// m2->print();
	// cout<<"==========================================\n";
	for(int i=0;i<m1->number.size();i++)
	{
		tempc=0;
		for(int j=0;j<m2->number.size();j++)
		{
			int tempp=((int(m1->number[i]-48)*int(m2->number[j]-48))+tempc)%10;
			// cout<<tempp<<"\n";
			mtemp.number.push_back(char(tempp+48));
			tempc = ((int(m1->number[i]-48)*int(m2->number[j]-48))+tempc)/10;
		}
		mtemp.number.push_back(char(tempc+48));
		// mtemp.print();
		cout<<"--------------------------------\n";
		Modular mtemp2;
		for(int k=0;k<i;k++)
			mtemp.number.insert(mtemp.number.begin(),'0');
		mtemp.print();
		add(mprdt,&mtemp,&mtemp2);
		mprdt->copy(&mtemp2);
		mprdt->print();
		cout<<"---------------------------------\n";
		mtemp2.clear();
		mtemp.clear();
	}
	mprdt->print();
}

int main()
{
	cout<<"Modular Arithemetic\n";
	cout<<"====================\n";
	// Modular m = new Modular(10);
	Modular *m2 = new Modular("20");
	int a = '0';
	cout<<a<<endl;
	Modular *m3 = new Modular("172");
	Modular m4;
	// m.print();
	m2->print();
	m3->print();
	add(m2,m3,&m4);
	m4.print();
	m3->copy(&m4);
	m3->print();
	mul(m3,m2,&m4);
	m4.print();
	return 0;
}