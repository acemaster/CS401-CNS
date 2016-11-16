import socket  
import sys
import threading

class ClientRead(threading.Thread):
	"""docstring for ClientRead"""
	def __init__(self, soc):
		threading.Thread.__init__(self)
		self.client = soc
		self.size = 1024

	def run(self):
		print "Starting read thread"
		running = 1
		while running:
			data = self.client.recv(self.size)
			if data:
				print "Recieved server data: " + data
			else:
				self.client.close()
				running = 0

		
s = socket.socket()         # Create a socket object
host = socket.gethostname() # Get local machine name
port = int(sys.argv[1])              # Reserve a port for your service.

s.connect((host, port))
running = 1
cread = ClientRead(s)
cread.start()
while running:
	message = raw_input("Enter the message: ")
	if message == 'exit':
		running = 0
		continue
	client = raw_input("Client no: ")
	message = message + ":" + client
	s.send(message)
