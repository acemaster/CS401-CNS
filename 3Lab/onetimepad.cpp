#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


using namespace std;

int main(int argc,char **argv)
{
	srand((int)time(0));
	int message_length;
	char message[512];
	char key[512];
	char cipher[27];
	char table[26][26];
	//Form table
	for(int i=0;i<26;i++)
	{
		for(int j=0;j<26;j++)
		{
			table[i][j]=((j+i)%26)+97;
		}
	}
	//Show table
	for(int i=0;i<26;i++)
	{
		for(int j=0;j<26;j++)
		{
			cout<<table[i][j]<<" ";
		}
		cout<<"\n";
	}
	//Message encryption starts
	message_length = atoi(argv[1]);
	for (int i = 0; i < message_length; ++i)
	{
		message[i]=argv[2][i];
	}
	for (int i = 0; i < message_length; ++i)
	{
		key[i]=(rand()%26)+97;
	}
	key[message_length]='\0';
	cout<<"\n=====================================\n";
	cout<<"\tOneTimePad cipher\n";
	cout<<"=======================================\n";
	cout<<"Key: "<<key<<endl;
	message[message_length]='\0';
	cout<<"Message: "<<message<<endl;
	for (int i = 0; i < message_length; ++i)
	{
		// cout<<int(message[i])-65<<endl;
		// cout<<char(cipher[message[i]-97])<<endl;
		// cout<<char(((int(cipher[message[i]-97]-96)+3)%26)+96)<<endl;
		// cout<<int(message[i])-96<<endl;
		// cout<<(int(message[i])-96+3)%26<<endl;
		// cout<<char((int(message[i])-96+3)%26)<<endl;
		// cout<<"=====================\n";
		int message_index = message[i]-97;
		int key_index = key[i]-97;
		cout<<"Pair: "<<message_index<<" : "<<key_index<<endl;
		message[i]=table[message_index][key_index];
	}

	cout<<"\nEncrypted message: "<<message<<endl;

		//Decrypt
	for (int i = 0; i < message_length; ++i)
	{
		int key_index = key[i]-97;
		for(int j=0;j<26;j++)
			if(table[j][key_index] == message[i])
			{
				message[i]=j+97;
				cout<<"Pair: "<<j<<" : "<<key_index<<endl;
				break;
			}
				
	}
	cout<<"\nDecrypted message: "<<message<<endl;
	return 0;
}