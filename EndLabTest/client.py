import socket  
import sys
import threading
from Crypto.PublicKey import RSA
from Crypto import Random

class ClientRead(threading.Thread):
	"""docstring for ClientRead"""
	def __init__(self, soc,key):
		threading.Thread.__init__(self)
		self.client = soc
		self.size = 1024
		self.key = key

	def run(self):
		print "Starting read thread"
		running = 1
		flag = 1
		while running:
			data = self.client.recv(self.size)
			if data:
				decrypted_text = key.decrypt(data)
				print "decrypted_text", decrypted_text


random_generator = Random.new().read
key = RSA.generate(1024, random_generator)
publickey = key.publickey().exportKey('PEM')	
s = socket.socket()         # Create a socket object
host = socket.gethostname() # Get local machine name
port = int(sys.argv[1])              # Reserve a port for your service.
username = sys.argv[2]
password = sys.argv[3]
username_password = username + "#" + password
print username_password
s.connect((host, port))
running = 1
s.send(username_password)
data = s.recv(1024)
if data == "Successful":
	readt = ClientRead(s,key)
	readt.start()
	while running:
		message = raw_input("$ > ")
		if message == 'exit':
			running = 0
			readt.join()
			continue
		message = message + "#" + publickey
		s.send(message)
else:
	print data

