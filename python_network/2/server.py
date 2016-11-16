import socket
import threading
import select
import sys


class Server:
	"""docstring for Server"""
	def __init__(self, port,threads,masterServer):
		self.host = ''
		self.port = int(port)
		self.backlog = 5
		self.size = 1024
		self.server = None
		self.threads = threads
		self.masterServer = masterServer

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
		self.open_socket()
		input = [self.server,sys.stdin]
		running = 1
		while running:
			inputready,outputready,exceptready = select.select(input,[],[])
			for s in inputready:
				if s == self.server:
					c = Client(self.server.accept(),self.masterServer)
					c.start()
					self.threads.append(c)
				elif s == sys.stdin:
					junk = sys.stdin.readline()
					running = 0
		self.server.close()
		for c in self.threads:
			c.join()
		print "Closing master Server"
		self.masterServer.close_server()

class Client(threading.Thread):
	"""docstring for Client"""
	def __init__(self,(client,address),masterServer):
		threading.Thread.__init__(self)
		# self.private_key = private_key
		self.client = client
		self.address = address
		self.size = 1024
		self.masterServer = masterServer

	def run(self):
		print "Created Client thread"
		running = 1
		while running:
			data = self.client.recv(self.size)
			if data:
				print "Client data: " + data
				self.masterServer.send_message(data)
			else:
				self.client.close()
				running = 0

	def send_message(self,message):
		print "Sending message to client"
		self.client.send(message)


class ConnectToMaster(threading.Thread):
	def __init__(self,port,clients):
		threading.Thread.__init__(self)
		self.port = int(port)
		self.server = None
		self.clients = clients
		self.size = 1024
		self.running = 1

	def run(self):
		print "Connecting to master Server"
		self.server = socket.socket()         # Create a socket object
		host = socket.gethostname() # Get local machine name
		port = self.port             # Reserve a port for your service.

		self.server.connect((host, port))
		while self.running:
			data = self.server.recv(self.size)
			if data:
				if data == 'exit':
					break
				print "Master Server data: " + data
				recv_data = data.split(":")
				client_no = recv_data[1]
				message = recv_data[0]
				self.clients[int(client_no)].send_message(message)

			else:
				self.server.close()
				self.running = 0
		print "Master server closed"

	def send_message(self,message):
		print "Sending message to master server"
		self.server.send(message)

	def close_server(self):
		self.running = 0

if __name__=="__main__":
	clients = []
	masterServer = ConnectToMaster(sys.argv[1],clients)
	masterServer.start()
	s = Server(sys.argv[2],clients,masterServer)
	s.run()


		