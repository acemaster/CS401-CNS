#include <bits/stdc++.h>

using namespace std;

int s1[4][16];
int s2[4][4];
// int s3[2][8];
int pc2[10];


class block
{
	int a[16];
public:
	block()
	{
		for (int i = 0; i < 16; ++i)
		{
			a[i]=0;
		}
	}
	block(char *s)
	{
		for(int i=0;i<2;i++)
		{
			int k=(i*8)+7;
			int k_end = ((i-1)*8)+8;
			int no = s[i];
			cout<<no<<"====";
			for(int j=k;j>=k_end;j--)
			{
				
				a[j]=no%2;
				no=no/2;
				cout<<a[j];
			}
			cout<<endl;
		}
	}

	int getbit(int i)
	{
		return a[i];
	}
	void setbit(int i)
	{
		a[i]=1;
	}
	void resetbit(int i)
	{
		a[i]=0;
	}
	void show()
	{
		for(int i=0;i<16;i++)
		{
			cout<<a[i];
		}
		cout<<endl;
	}
};

class keyclass
{
	int a[8];
	public:
		keyclass(char *s)
		{
			for(int i=0;i<1;i++)
			{
				int k=(i*8)+7;
				int k_end = ((i-1)*8)+8;
				int no = s[i];
				cout<<no<<"====";
				for(int j=k;j>=k_end;j--)
				{
					
					a[j]=no%2;
					no=no/2;
					cout<<a[j];
				}
				cout<<endl;
			}

		}
	int getbit(int i)
	{
		return a[i];
	}
	void setbit(int i)
	{
		a[i]=1;
	}
	void resetbit(int i)
	{
		a[i]=0;
	}
	void show()
	{
		for(int i=0;i<8;i++)
		{
			cout<<a[i];
		}
		cout<<endl;
	}

	void rightshift(int no)
	{
		for(int i=0;i<no;i++)
		{
			int temp=a[0];
			for(int i=0;i<3;i++)
			{
				a[i]=a[i+1];
			}
			a[3]=temp;
			temp=a[7];
			for(int i=7;i>=5;i--)
			{
				a[i]=a[i-1];
			}
			a[4]=temp;
		}
	}
};

void desround(int roundno,block *b,keyclass *key,block *bend)
{
	//Split into 6 and 10
	int b1[6];
	int b2[10];
	int l1[6];
	int r1[10];
	cout<<"Block: ";
	b->show();
	for(int i=0;i<6;i++)
	{
		b1[i]=b->getbit(i);
	}
	for(int i=0;i<10;i++)
	{
		b2[i]=b->getbit(i+6);
		l1[i]=b->getbit(i+6);
	}
	//displaying l1 and r1
	cout<<"l1==============\n";
	for(int i=0;i<6;i++)
	{
		cout<<b1[i];
	}
	cout<<endl;
	cout<<"l2=============\n";
	for(int i=0;i<10;i++)
	{
		cout<<b2[i];
	}	
	//xor with key
	key->rightshift(1);
	key->show();
	int contractedkey[6];
	for(int i=0;i<10;i++)
	{
		contractedkey[i]=key->getbit(pc2[i]);
	}
	cout<<"\nContracted key\n";
	for(int i=0;i<10;i++)
	{
		cout<<contractedkey[i];
	}
	for(int i=0;i<10;i++)
	{
		if (b2[i]==contractedkey[i])
		{
			b2[i]=1;
		}
		else
			b2[i]=0;
	}
	//Round using sbox;
	cout<<"\n====Round started=====\n";
	int bit1=b2[0];
	int bit2=b2[5];
	int rowindex1= bit2*pow(2,0)+bit1*pow(2,1);
	int columnindex1 = 0;
	cout<<"Row: "<<rowindex1<<endl;
	int k=0;
	for(int i=4;i>=1;i--)
	{
		columnindex1+=b2[i]*pow(2,k);
		k++;
	}
	cout<<"Column: "<<columnindex1<<endl;
	bit1=b2[6];
	bit2=b2[9];
	int rowindex2= bit2*pow(2,0)+bit1*pow(2,1);
	int columnindex2 = 0;
	cout<<"Row: "<<rowindex2<<endl;
	k=0;
	for(int i=8;i>=7;i--)
	{
		columnindex2+=b2[i]*pow(2,k);
		k++;
	}
	cout<<"Column: "<<columnindex2<<endl;
	int s1_val = s1[rowindex1][columnindex1];
	int s2_val = s2[rowindex2][columnindex2];
	// int s3_val = s3[rowindex][columnindex];
	cout<<s1_val<<";"<<s2_val<<";\n";
	int finalr[6];
	finalr[3]=s1_val%2;
	s1_val/=2;
	finalr[2]=s1_val%2;
	s1_val/=2;
	finalr[1]=s1_val%2;
	s1_val/=2;
	finalr[0]=s1_val%2;
	s1_val/=2;
	finalr[5]=s2_val%2;
	s2_val/=2;
	finalr[4]=s2_val%2;
	s2_val/=2;
	for(int i=0;i<6;i++)
	{
		cout<<finalr[i];
	}
	cout<<"\n====Round ended=====\n";
	//s box done
	for(int i=0;i<6;i++)
	{
		if(finalr[i] == b1[i])
			r1[i]=1;
		else
			r1[i]=0;
	}
	cout<<"\n====r1======\n";
	for(int i=0;i<6;i++)
	{
		cout<<r1[i];
	}
	cout<<"\n==Final Cipher block==\n";
	for(int i=0;i<10;i++)
	{
		if(l1[i]== 1)
			bend->setbit(i);
		else
			bend->resetbit(i);
	}
	for(int i=10;i<16;i++)
	{
		if(r1[i-10]== 1)
			bend->setbit(i);
		else
			bend->resetbit(i);
	}
	bend->show();
	cout<<"\nDone one block";

}
int main(int argc,char **argv)
{
	for(int i=0;i<4;i++)
	{
		for (int j = 0; j < 16; ++j)
		{
			cin>>s1[i][j];
		}
	}
	for(int i=0;i<4;i++)
	{
		for (int j = 0; j < 4; ++j)
		{
			cin>>s2[i][j];
		}
	}
		for (int j = 0; j < 6; ++j)
		{
			cin>>pc2[j];
		}
	
	// for(int i=0;i<4;i++)
	// {
	// 	for (int j = 0; j < ; ++j)
	// 	{
	// 		cin>>s3[i][j];
	// 	}
	// }
	cout<<"S1================\n";
	for(int i=0;i<4;i++)
	{
		for (int j = 0; j < 16; ++j)
		{
			cout<<s1[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"S2================\n";
	for(int i=0;i<4;i++)
	{
		for (int j = 0; j < 4; ++j)
		{
			cout<<s2[i][j]<<" ";
		}
		cout<<endl;
	}
	// cout<<"S3================\n";
	// for(int i=0;i<2;i++)
	// {
	// 	for (int j = 0; j < 8; ++j)
	// 	{
	// 		cout<<s3[i][j]<<" ";
	// 	}
	// 	cout<<endl;
	// }
	block b(argv[1]);
	block bend;
	b.show();
	keyclass k(argv[2]);
	k.show();
	desround(1,&b,&k,&bend);
	block b2(argv[3]);
	block bend2;
	b2.show();
	desround(1,&b2,&k,&bend2);
	//IV
	int c0[10];
	for(int i=0;i<10;i++)
	{
		c0[i]=0;
	}
	int finalc1dash[16];
	for(int i=0;i<16;i++)
	{
		if(c0[i] == bend.getbit(i))
			finalc1dash[i]=1;
		else
			finalc1dash[i]=0;
	}
	int finalc2dash[16];
	for(int i=0;i<16;i++)
	{
		if(bend.getbit(i) == bend2.getbit(i))
			finalc2dash[i]=1;
		else
			finalc2dash[i]=0;
	}
	cout<<"\nC1 dash==============\n";
	for (int i = 0; i < 16; ++i)
	{
		cout<<finalc1dash[i];
	}
	cout<<"\nC2 dash==============\n";
	for (int i = 0; i < 16; ++i)
	{
		cout<<finalc1dash[i];
	}
	cout<<endl;
	return 0;
}