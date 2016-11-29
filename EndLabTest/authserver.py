import socket
import threading
import select
import sys
import os
from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto import Random

username_passwords = {}
username_levels = {}
filepath_levels = {}

class Server(threading.Thread):
	"""docstring for Server"""
	def __init__(self, port,threads,username_passwords,username_levels,filepath_levels):
		threading.Thread.__init__(self)
		self.host = ''
		self.port = int(port)
		self.backlog = 5
		self.size = 1024
		self.server = None
		self.threads = threads
		self.username_passwords = username_passwords
		self.username_levels = username_levels
		self.filepath_levels = filepath_levels

	def open_socket(self):
		try:
			self.server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
			self.server.bind((self.host,self.port))
			self.server.listen(5)
		except socket.error, (value,message):
			if self.server:
				self.server.close()
			print "Couldnot open server: "+ message
			sys.exit(1)

	def run(self):
		print "Starting Server thread"
		self.open_socket()
		input = [self.server]
		running = 1
		while running:
			inputready,outputready,exceptready = select.select(input,[],[])
			for s in inputready:
				if s == self.server:
					c = Client(self.server.accept(),self.username_passwords,self.username_levels,self.filepath_levels)
					c.start()
					self.threads.append(c)
		self.server.close()
		for c in self.threads:
			c.join()
		print "Closing master Server"

class Client(threading.Thread):
	"""docstring for Client"""
	def __init__(self,(client,address),username_passwords,username_levels,filepath_levels):
		threading.Thread.__init__(self)
		# self.private_key = private_key
		self.client = client
		self.address = address
		self.size = 2024
		self.username_passwords = username_passwords
		self.username_levels = username_levels
		self.filepath_levels = filepath_levels

	def run(self):
		print self.username_passwords.keys()
		data = self.client.recv(self.size)
		print data
		data_arr = data.split("#")
		username = data_arr[0]
		password = data_arr[1]
		if username in self.username_passwords:
			hashed = SHA256.new(password).digest()
			if hashed == self.username_passwords[username]:
				self.client.send("Successful")
				running = True
				while running:
					data = self.client.recv(self.size)
					if data:
						print data
						data_arr = data.split("#")
						data = data_arr[0]
						publickey = data_arr[1]
						if data == 'ls':
							files = ""
							for f in self.filepath_levels.keys():
								if self.filepath_levels[f] >= self.username_levels[username]:
									files = files + str(f)+"\n"
							if files == "":
								files = "No files"
							rsakey = RSA.importKey(publickey)
							encrypted_text = rsakey.publickey().encrypt(files,32)
							print "Encrypted text", encrypted_text
							self.client.send(encrypted_text[0])

			else:
				self.client.send("UnSuccessful")
		else:
			self.client.send("UnSuccessful")


	def send_message(self,message):
		print "Sending message to client"
		self.client.send(message)


class MasterThread(threading.Thread):
	def __init__(self,file_name,username_levels,username_passwords,filepath_levels):
		threading.Thread.__init__(self)
		self.file_name = file_name  
		self.username_levels = username_levels
		self.username_passwords = username_passwords
		self.filepath_levels = filepath_levels

	def run(self):
		with open(self.file_name) as f:
			content = f.read().splitlines()
			for i in range(0,len(content),3):
				username = content[i]
				content[i+1] =content[i+1]
				print  "Adding ",content[i]," ",content[i+1]
				password = SHA256.new(content[i+1]).digest()
				self.username_passwords[username] = password
				self.username_levels[username] = int(content[i+2])
		print "Added users"
		running = True
		while running:
			cmd = raw_input("$ > ")
			if cmd == 'show':
				print "Users:----"
				for key in self.username_passwords.keys():
					print key
			elif cmd == 'adduser':
				username = raw_input("username: ")
				password = raw_input("password: ")
				level = raw_input("level: ")
				password_enc = SHA256.new(password).digest()
				self.username_passwords[username]=password_enc
				self.username_levels[username]=level
			elif cmd == 'ls':
				data = '\n'.join(os.listdir("."))
				print data
			elif cmd == "addfilelevel":
				filepath = raw_input("path relative: ")
				level = raw_input("level: ")
				self.filepath_levels[filepath]=int(level)
			elif cmd == "lp":
				data = os.listdir(".")
				for f in data:
					if str(f) in self.filepath_levels:
						print str(f)," : ",self.filepath_levels[str(f)]
			elif cmd == "test":
				username = raw_input("username: ")
				password = raw_input("password: ")
				username_password = username + "#" + password
				data_arr = username_password.split("#")
				if data_arr[0] in self.username_passwords:
					password = SHA256.new(data_arr[1]).digest()
					if password == self.username_passwords[data_arr[0]]:
						print "Successful"
					else:
						print "UnSuccessful"
				else:
					print "UnSuccessful"
			elif cmd == "exit":
				running = False


if __name__=="__main__":
	clients = []
	m = MasterThread("test.txt",username_levels,username_passwords,filepath_levels)
	m.start()
	s = Server(sys.argv[1],clients,username_passwords,username_levels,filepath_levels)
	s.start()


		