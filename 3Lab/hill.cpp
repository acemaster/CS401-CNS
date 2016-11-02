#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


using namespace std;

int main(int argc,char **argv)
{
	cout<<"=====================================\n";
	cout<<"         Hill Cipher                 \n";
	cout<<"=====================================\n";
	cout<<"Only works for 3x3 key\n";
	int mul[3][1];
	int prod[3][1];
	int message_length;
	char message[512];
	int decrypt_message[512];
	message_length = atoi(argv[1]);
	for (int i = 0; i < message_length; ++i)
	{
		message[i]=argv[2][i];
	}
	if(message_length%3 == 1)
	{
		message[message_length++]='a';
		message[message_length++]='a';
	}
	else if(message_length%3 == 2)
	{
		message[message_length++]='a';
	}
	message[message_length]='\0';
	cout<<"Enter key: \n";
	int key[3][3];
	int key_inv[3][3];
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			cin>>key[i][j];
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			cout<<key[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"Enter key inverse : ";
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			cin>>key_inv[i][j];
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			cout<<key_inv[i][j]<<" ";
		}
		cout<<endl;
	}
	for(int z=0; z<message_length;z=z+3)
	{
		mul[0][0]=message[z]-97;
		mul[1][0]=message[z+1]-97;
		mul[2][0]=message[z+2]-97;
		cout<<"Letters: ";
		for(int i=0;i<3;i++)
			cout<<mul[i][0]<<" ";
		cout<<endl;
		for(int i=0;i<3;i++)
			for(int j=0;j<1;j++)
			{
				prod[i][j]=0;
				for(int k=0;k<3;k++)
					prod[i][j]+=key[i][k]*mul[k][j];
			}
		cout<<"Product: ";
		for(int i=0;i<3;i++)
			cout<<prod[i][0]<<" ";
		cout<<endl;
		decrypt_message[z]=prod[0][0];
		decrypt_message[z+1]=prod[1][0];
		decrypt_message[z+2]=prod[2][0];
		message[z]=prod[0][0]%26 + 97;
		message[z+1]=prod[1][0]%26 + 97;
		message[z+2]=prod[2][0]%26 + 97;
		cout<<endl;
	}
	cout<<"Encrypted message: "<<message<<endl;
	for(int z=0; z<message_length;z=z+3)
	{
		mul[0][0]=decrypt_message[z];
		mul[1][0]=decrypt_message[z+1];
		mul[2][0]=decrypt_message[z+2];
		cout<<"Letters: ";
		for(int i=0;i<3;i++)
			cout<<mul[i][0]<<" ";
		cout<<endl;
		for(int i=0;i<3;i++)
			for(int j=0;j<1;j++)
			{
				prod[i][j]=0;
				for(int k=0;k<3;k++)
					prod[i][j]+=key_inv[i][k]*mul[k][j];
			}
		cout<<"Product: ";
		for(int i=0;i<3;i++)
			cout<<prod[i][0]<<" ";
		cout<<endl;
		message[z]=prod[0][0]%26+97;
		message[z+1]=prod[1][0]%26+97;
		message[z+2]=prod[2][0]%26+97;
		cout<<endl;
	}
	cout<<"Decrypted message: "<<message<<endl;

}