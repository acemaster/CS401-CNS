import socket
import threading
import select
import sys


class Server:
	"""docstring for Server"""
	def __init__(self, port,threads,groups):
		self.host = ''
		self.port = int(port)
		self.backlog = 5
		self.size = 1024
		self.server = None
		self.threads = threads
		self.groups = groups

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
					print "Current group: "+ str(self.groups)
					c = Client(self.server.accept(),self.groups)
					c.start()
					self.threads.append(c)
				elif s == sys.stdin:
					junk = sys.stdin.readline()
					running = 0
		self.server.close()
		for c in self.threads:
			c.join()

class Client(threading.Thread):
	"""docstring for Client"""
	def __init__(self,(client,address),groups):
		threading.Thread.__init__(self)
		# self.private_key = private_key
		self.client = client
		self.address = address
		self.size = 1024
		self.groups = groups

	def run(self):
		print "Created Client thread"
		running = 1
		flag = 0
		while running:
			data = self.client.recv(self.size)
			if data:
				if flag = 0:
					self.groups[data].append(self.client)
					flag = 1
				else:
					print "Client data: "+ data
			else:
				self.client.close()
				running = 0

	def send_message(self,message):
		print "Sending message to client"
		self.client.send(message)



if __name__=="__main__":
	clients=[]
	clients_grps = {}
	clients_grps['A']=[]
	clients_grps['B']=[]
	s = Server(sys.argv[2],clients,clients_grps)
	s.run()


		