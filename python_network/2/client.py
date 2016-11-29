import socket  
import sys
import threading

class ClientRead(threading.Thread):
	"""docstring for ClientRead"""
	def __init__(self, soc):
		threading.Thread.__init__(self)
		self.client = soc
		self.size = 1024
		self.key = 0

	def run(self):
		print "Starting read thread"
		running = 1
		flag = 1
		while running:
			data = self.client.recv(self.size)
			if data:
				print "Recieved server data: " + data
				if data == 'sendkey':
					self.client.send(str(self.key))
				elif flag == 1:
					arr = data.split(":")
					if arr[0] == 'getkey':
						self.key = int(arr[1])
						flag = 0
					
			else:
				self.client.close()
				running = 0

		
s = socket.socket()         # Create a socket object
host = socket.gethostname() # Get local machine name
port = int(sys.argv[1])              # Reserve a port for your service.

s.connect((host, port))
running = 1
group = raw_input("Enter the group (A/B): ")
s.send(group)
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

