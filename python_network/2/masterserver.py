import socket
import threading
import select
import sys
import subprocess

def tobits(s):
    result = []
    for c in s:
        bits = bin(ord(c))[2:]
        bits = '00000000'[len(bits):] + bits
        result.extend([int(b) for b in bits])
    return result

def frombits(bits):
    chars = []
    for b in range(len(bits) / 8):
        byte = bits[b*8:(b+1)*8]
        chars.append(chr(int(''.join([str(bit) for bit in byte]), 2)))
    return ''.join(chars)

class Server:
	"""docstring for Server"""
	def __init__(self, port):
		self.host = ''
		self.port = int(port)
		self.backlog = 5
		self.size = 1024
		self.server = None
		self.threads = []
		self.thread_count = 0

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
					print str(self.thread_count) + " Current thread count"
					self.thread_count = self.thread_count + 1
					c = Client(self.server.accept(),self.threads,self.thread_count)
					c.start()
					self.threads.append(c)
				elif s == sys.stdin:
					junk = sys.stdin.readline()
					running = 0
		print "Closing master server"
		self.server.close()
		for c in self.threads:
			c.send_message("exit")
			c.join()

class Client(threading.Thread):
	"""docstring for Client"""
	def __init__(self,(client,address),threads,thread_count):
		threading.Thread.__init__(self)
		# self.private_key = private_key
		self.client = client
		self.address = address
		self.size = 1024
		self.threads = threads
		self.thread_count = thread_count - 1
		print "Thread id : "+ str(self.thread_count)

	def run(self):
		print "Created Child Server thread"
		running = 1
		while running:
			data = self.client.recv(self.size)
			if data:
				print "Child Server data: " + data
				send_client_id = (self.thread_count + 1)%2
				self.threads[send_client_id].send_message(data)
			else:
				self.client.close()
				running = 0

	def send_message(self,message):
		print "Sending to other server"
		self.client.send(message)


if __name__=="__main__":
	print "Starting master server"
	print "Getting keys for groups"
	output = subprocess.Popen(["./enc", "0","11","0"], stdout=subprocess.PIPE).communicate()[0]
	group_creds={}
	output_arr = output.split(":")
	group_creds['A']={}
	group_creds['A']['e']=output_arr[0]
	group_creds['A']['n']=output_arr[1]
	group_creds['A']['d']=output_arr[2]
	output = subprocess.Popen(["./enc", "0","11","1"], stdout=subprocess.PIPE).communicate()[0]
	output_arr = output.split(":")
	group_creds['B']={}
	group_creds['B']['e']=output_arr[0]
	group_creds['B']['n']=output_arr[1]
	group_creds['B']['d']=output_arr[2]
	print group_creds
	# s = Server(sys.argv[1])
	# s.run()


		