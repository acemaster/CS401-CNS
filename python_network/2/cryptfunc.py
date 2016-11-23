import sys
import subprocess


def getkey(index):
	output = subprocess.Popen(["./enc", "0","11",str(index)], stdout=subprocess.PIPE).communicate()[0]
	return output


def encrpyt(index,e,n,message):
	output = subprocess.Popen(["./enc", "1","11",e,n,str(m)], stdout=subprocess.PIPE).communicate()[0]
	return output

def decrpyt(index,d,n,message):
	output = subprocess.Popen(["./enc", "1","11",d,n,str(m)], stdout=subprocess.PIPE).communicate()[0]
	return output