import sys

text = sys.argv[1]
'''
Hashing
'''

from Crypto.Hash import SHA256
hashed  = SHA256.new(text).digest()
print "HASHED",hashed, len(hashed)
print len(hashed)


import os
from Crypto.Hash import MD5
def get_file_checksum(filename):
    h = MD5.new()
    chunk_size = 8192
    with open(filename, 'rb') as f:
        while True:
            chunk = f.read(chunk_size)
            if len(chunk) == 0:
                break
            h.update(chunk)
    return h.hexdigest()


print "checksum",get_file_checksum("test.txt")



'''
DES
'''
from Crypto.Cipher import DES
des = DES.new("01234567", DES.MODE_ECB) #key, mode
cipher_text = des.encrypt(text)
print "cipher_text", cipher_text
plain_text = des.decrypt(cipher_text)
print "plain_text", plain_text



'''
RSA
'''


from Crypto.PublicKey import RSA
from Crypto import Random

random_generator = Random.new().read

key = RSA.generate(1024, random_generator)
publickey = key.publickey().exportKey('PEM')

#assume publickey is sent over a socket to another client

rsakey = RSA.importKey(publickey)
encrypted_text = rsakey.publickey().encrypt(text,32)
print "Encrypted text", encrypted_text

#client encrypts using public key. assume that encyrpted text is sent back to server

decrypted_text = key.decrypt(encrypted_text)
print "decrypted_text", decrypted_text



'''
AES
'''

from Crypto.Cipher import AES
import base64
import os

aesEncrypt = lambda key,text : key.encrypt(text)
aesDecrypt = lambda key,text : key.decrypt(text)


secret = os.urandom(16)#aes key should be of length 16,32,64
print secret
IV = 16 * '\x00'
cipher = AES.new(secret,AES.MODE_CFB, IV = IV)

encrypted = aesEncrypt(cipher,"password")
print "Encrypted string: ", encrypted


cipher = AES.new(secret,AES.MODE_CFB, IV = IV)
decrypted = aesDecrypt(cipher, encrypted)
print "Decrypted string :", decrypted



'''
Digital Signature
'''


from Crypto.Hash import SHA256
from Crypto.PublicKey import RSA
from Crypto import Random


random_generator = Random.new().read

key = RSA.generate(1024, random_generator)
hash_value = SHA256.new(text).digest()
signature = key.sign(hash_value, '')# signed with private key

#public key is sent over the network. So is the signature
publickey = key.publickey().exportKey("PEM")
rsakey = RSA.importKey(publickey)
verified_value = rsakey.publickey().verify(hash_value, signature)
print "VERIFIED VIA DIGITAL SIGNATURE", verified_value

#Threading cheatsheet

import threading
#Wrapping a Function. The option target points the thread to run the function when started.

def myfunc(*args, **kwargs):
    pass

t = threading.Thread(target = myfunc, \
        args = (), kwargs = {} )

#Invoke the thread
t.start()

#Join when finished
t.join()
#Subclassing threading.Thread: Subclassing a thread involves overloading the run method which will then be called by the start method.

class MyThreadClass(threading.Thread):

    def run(self):
        pass

my_thread = MyThreadClass()

#Invoke
my_thread.start()

#Join when finished
my_thread.join()
#Daemon Mode: Setting daemon mode on a thread will prevent the thread from keeping the program from exiting. Any thread in daemon mode that has not exited will hang preventing the main thread from exiting.

my_thread = MyThreadClass()

#set daemon mode
my_thread.daemon = True

#Thread now started in daemon mode
my_thread.start()


#sockets cheat sheet
#=====================================================
Get Hostname
>>> import socket
>>> socket.gethostname()
'MacBookPro-4380.local'
>>> hostname = socket.gethostname()
>>> socket.gethostbyname(hostname)
'172.20.10.4'
>>> socket.gethostbyname('localhost')
'127.0.0.1'
Transform Host & Network Endian
# little-endian machine
>>> import socket
>>> a = 1 # host endian
>>> socket.htons(a) # network endian
256
>>> socket.htonl(a) # network endian
16777216
>>> socket.ntohs(256) # host endian
1
>>> socket.ntohl(16777216) # host endian
1

# big-endian machine
>>> import socket
>>> a = 1 # host endian
>>> socket.htons(a) # network endian
1
>>> socket.htonl(a) # network endian
1L
>>> socket.ntohs(1) # host endian
1
>>> socket.ntohl(1) # host endian
1L
IP dotted-quad string & byte format convert
>>> import socket
>>> addr = socket.inet_aton('127.0.0.1')
>>> addr
'\x7f\x00\x00\x01'
>>> socket.inet_ntoa(addr)
'127.0.0.1'
Mac address & byte format convert
>>> mac = '00:11:32:3c:c3:0b'
>>> byte = binascii.unhexlify(mac.replace(':',''))
>>> byte
'\x00\x112<\xc3\x0b'
>>> binascii.hexlify(byte)
'0011323cc30b'
Simple TCP Echo Server
import socket

class Server(object):
    def __init__(self,host,port):
        self._host = host
        self._port = port
    def __enter__(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)
        sock.bind((self._host,self._port))
        sock.listen(10)
        self._sock = sock
        return self._sock
    def __exit__(self,*exc_info):
        if exc_info[0]:
            import traceback
            traceback.print_exception(*exc_info)
        self._sock.close()

if __name__ == '__main__':
    host = 'localhost'
    port = 5566
    with Server(host,5566) as s:
        while True:
            conn, addr = s.accept()
            msg = conn.recv(1024)
            conn.send(msg)
            conn.close()
output:

$ nc localhost 5566
Hello World
Hello World
Simple TCP Echo Server Via SocketServer
>>> import SocketServer
>>> bh = SocketServer.BaseRequestHandler
>>> class handler(bh):
...   def handle(self):
...     data = self.request.recv(1024)
...     print self.client_address
...     self.request.sendall(data)
...
>>> host = ('localhost',5566)
>>> s = SocketServer.TCPServer(
...   host, handler)
>>> s.serve_forever()
output:

$ nc -u localhost 5566
Hello World
Hello World
Simple SSL TCP Echo Server
import socket
import ssl

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind(('localhost', 5566))
sock.listen(10)

sslctx = ssl.SSLContext(ssl.PROTOCOL_TLSv1)
sslctx.load_cert_chain(certfile='./root-ca.crt',
                       keyfile='./root-ca.key')

try:
    while True:
        conn, addr = sock.accept()
        sslconn = sslctx.wrap_socket(conn, server_side=True)
        msg = sslconn.recv(1024)
        if msg:
            sslconn.send(msg)
        sslconn.close()
finally:
    sock.close()
output:

# console 1
$ openssl genrsa -out root-ca.key 2048
$ openssl req -x509 -new -nodes -key root-ca.key -days 365 -out root-ca.crt
$ python3 ssl_tcp_server.py

# console 2
$ openssl s_client -connect localhost:5566
...
Hello SSL
Hello SSL
read:errno=0
Simple UDP Echo Server
import socket

class UDPServer(object):
    def __init__(self,host,port):
        self._host = host
        self._port = port

    def __enter__(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.bind((self._host,self._port))
        self._sock = sock
        return sock
   def __exit__(self,*exc_info):
        if exc_info[0]:
            import traceback
            traceback.print_exception(*exc_info)
        self._sock.close()

if __name__ == '__main__':
    host = 'localhost'
    port = 5566
    with UDPServer(host,port) as s:
        while True:
            msg, addr = s.recvfrom(1024)
            s.sendto(msg, addr)
output:

$ nc -u localhost 5566
Hello World
Hello World
Simple UDP Echo Server Via SocketServer
>>> import SocketServer
>>> bh = SocketServer.BaseRequestHandler
>>> class handler(bh):
...   def handle(self):
...     m,s = self.request
...     s.sendto(m,self.client_address)
...     print self.client_address
...
>>> host = ('localhost',5566)
>>> s = SocketServer.UDPServer(
...   host, handler)
>>> s.serve_forever()
output:

$ nc -u localhost 5566
Hello World
Hello World
Simple UDP client - Sender
>>> import socket
>>> import time
>>> sock = socket.socket(
...   socket.AF_INET,
...   socket.SOCK_DGRAM)
>>> host = ('localhost',5566)
>>> while True:
...   sock.sendto("Hello\n",host)
...   time.sleep(5)
...
output:

$ nc -lu localhost 5566
Hello
Hello
Broadcast UDP Packets
>>> import socket
>>> import time
>>> sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
>>> sock.bind(('',0))
>>> sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST,1)
>>> while True:
...   m = '{0}\n'.format(time.time())
...   sock.sendto(m,('<broadcast>',5566))
...   time.sleep(5)
...
output:

$ nc -k -w 1 -ul 5566
1431473025.72
Simple UNIX Domain Socket
import socket
import contextlib
import os

@contextlib.contextmanager
def DomainServer(addr):
    try:
        if os.path.exists(addr):
            os.unlink(addr)
        sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        sock.bind(addr)
        sock.listen(10)
        yield sock
    finally:
        sock.close()
        if os.path.exists(addr):
            os.unlink(addr)

addr = "./domain.sock"
with DomainServer(addr) as sock:
    while True:
        conn, _ = sock.accept()
        msg = conn.recv(1024)
        conn.send(msg)
        conn.close()
output:

$ nc -U ./domain.sock
Hello
Hello
Simple duplex processes communication
import os
import socket

child, parent = socket.socketpair()
pid = os.fork()
try:

    if pid == 0:
        print('chlid pid: {}'.format(os.getpid()))

        child.send(b'Hello Parent')
        msg = child.recv(1024)
        print('p[{}] ---> c[{}]: {}'.format(
            os.getppid(), os.getpid(), msg))
    else:
        print('parent pid: {}'.format(os.getpid()))

        # simple echo server (parent)
        msg = parent.recv(1024)
        print('c[{}] ---> p[{}]: {}'.format(
                pid, os.getpid(), msg))
        parent.send(msg)

except KeyboardInterrupt:
    pass
finally:
    child.close()
    parent.close()
output:

$ python3 socketpair_demo.py
parent pid: 9497
chlid pid: 9498
c[9498] ---> p[9497]: b'Hello Parent'
p[9497] ---> c[9498]: b'Hello Parent'
Simple Asynchronous TCP Server - Thread
>>> from threading import Thread
>>> import socket
>>> def work(conn):
...   while True:
...     msg = conn.recv(1024)
...     conn.send(msg)
...
>>> sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
>>> sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)
>>> sock.bind(('localhost',5566))
>>> sock.listen(5)
>>> while True:
...   conn,addr = sock.accept()
...   t=Thread(target=work,args=(conn,))
...   t.daemon=True
...   t.start()
...
output: (bash 1)

$ nc localhost 5566
Hello
Hello
output: (bash 2)

$ nc localhost 5566
Ker Ker
Ker Ker
Simple Asynchronous TCP Server - select
from select import select
import socket

host = ('localhost',5566)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)
sock.bind(host)
sock.listen(5)
rl = [sock]
wl = []
ml = {}
try:
    while True:
        r, w, _ = select(rl,wl,[])
        # process ready to ready
        for _ in r:
            if _ == sock:
                conn, addr = sock.accept()
                rl.append(conn)
            else:
                msg = _.recv(1024)
                ml[_.fileno()] = msg
                wl.append(_)
        # process ready to write
        for _ in w:
            msg = ml[_.fileno()]
            _.send(msg)
            wl.remove(_)
            del ml[_.fileno()]
except:
    sock.close()
output: (bash 1)

$ nc localhost 5566
Hello
Hello
output: (bash 2)

$ nc localhost 5566
Ker Ker
Ker Ker
High-Level API - selectors
# Pyton3.4+ only
# Reference: selectors
import selectors
import socket
import contextlib

@contextlib.contextmanager
def Server(host,port):
   try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((host,port))
        s.listen(10)
        sel = selectors.DefaultSelector()
        yield s, sel
    except socket.error:
        print("Get socket error")
        raise
    finally:
        if s:
            s.close()

def read_handler(conn, sel):
    msg = conn.recv(1024)
    if msg:
        conn.send(msg)
    else:
        sel.unregister(conn)
        conn.close()

def accept_handler(s, sel):
    conn, _ = s.accept()
    sel.register(conn, selectors.EVENT_READ, read_handler)

host = 'localhost'
port = 5566
with Server(host, port) as (s,sel):
    sel.register(s, selectors.EVENT_READ, accept_handler)
    while True:
        events = sel.select()
        for sel_key, m in events:
            handler = sel_key.data
            handler(sel_key.fileobj, sel)
output: (bash 1)

$ nc localhost 5566
Hello
Hello
output: (bash 1)

$ nc localhost 5566
Hi
Hi
“socketpair” - Similar to PIPE
import socket
import os
import time

c_s, p_s = socket.socketpair()
try:
    pid = os.fork()
except OSError:
    print "Fork Error"
    raise

if pid:
    # parent process
    c_s.close()
    while True:
        p_s.sendall("Hi! Child!")
        msg = p_s.recv(1024)
        print msg
        time.sleep(3)
    os.wait()
else:
    # child process
    p_s.close()
    while True:
        msg = c_s.recv(1024)
        print msg
        c_s.sendall("Hi! Parent!")
output:

$ python ex.py
Hi! Child!
Hi! Parent!
Hi! Child!
Hi! Parent!
...
Sniffer IP packets
from ctypes import *
import socket
import struct

# ref: IP protocol numbers
PROTO_MAP = {
        1 : "ICMP",
        2 : "IGMP",
        6 : "TCP",
        17: "UDP",
        27: "RDP"}

class IP(Structure):
    ''' IP header Structure

    In linux api, it define as below:

    strcut ip {
        u_char         ip_hl:4; /* header_len */
        u_char         ip_v:4;  /* version */
        u_char         ip_tos;  /* type of service */
        short          ip_len;  /* total len */
        u_short        ip_id;   /* identification */
        short          ip_off;  /* offset field */
        u_char         ip_ttl;  /* time to live */
        u_char         ip_p;    /* protocol */
        u_short        ip_sum;  /* checksum */
        struct in_addr ip_src;  /* source */
        struct in_addr ip_dst;  /* destination */
    };
    '''
    _fields_ = [("ip_hl" , c_ubyte, 4), # 4 bit
                ("ip_v"  , c_ubyte, 4), # 1 byte
                ("ip_tos", c_uint8),    # 2 byte
                ("ip_len", c_uint16),   # 4 byte
                ("ip_id" , c_uint16),   # 6 byte
                ("ip_off", c_uint16),   # 8 byte
                ("ip_ttl", c_uint8),    # 9 byte
                ("ip_p"  , c_uint8),    # 10 byte
                ("ip_sum", c_uint16),   # 12 byte
                ("ip_src", c_uint32),   # 16 byte
                ("ip_dst", c_uint32)]   # 20 byte

    def __new__(cls, buf=None):
        return cls.from_buffer_copy(buf)
    def __init__(self, buf=None):
        src = struct.pack("<L", self.ip_src)
        self.src = socket.inet_ntoa(src)
        dst = struct.pack("<L", self.ip_dst)
        self.dst = socket.inet_ntoa(dst)
        try:
            self.proto = PROTO_MAP[self.ip_p]
        except KeyError:
            print "{} Not in map".format(self.ip_p)
            raise

host = '0.0.0.0'
s = socket.socket(socket.AF_INET,
                  socket.SOCK_RAW,
                  socket.IPPROTO_ICMP)
s.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)
s.bind((host, 0))

print "Sniffer start..."
try:
    while True:
        buf = s.recvfrom(65535)[0]
        ip_header = IP(buf[:20])
        print '{0}: {1} -> {2}'.format(ip_header.proto,
                                       ip_header.src,
                                       ip_header.dst)
except KeyboardInterrupt:
    s.close()
output: (bash 1)

python sniffer.py
Sniffer start...
ICMP: 127.0.0.1 -> 127.0.0.1
ICMP: 127.0.0.1 -> 127.0.0.1
ICMP: 127.0.0.1 -> 127.0.0.1
output: (bash 2)

$ ping -c 3 localhost
PING localhost (127.0.0.1): 56 data bytes
64 bytes from 127.0.0.1: icmp_seq=0 ttl=64 time=0.063 ms
64 bytes from 127.0.0.1: icmp_seq=1 ttl=64 time=0.087 ms
64 bytes from 127.0.0.1: icmp_seq=2 ttl=64 time=0.159 ms

--- localhost ping statistics ---
3 packets transmitted, 3 packets received, 0.0% packet loss
round-trip min/avg/max/stddev = 0.063/0.103/0.159/0.041 ms
Sniffer ARP packet
"""
Ehternet Packet Header

struct ethhdr {
    unsigned char h_dest[ETH_ALEN];   /* destination eth addr */
    unsigned char h_source[ETH_ALEN]; /* source ether addr    */
    __be16        h_proto;            /* packet type ID field */
} __attribute__((packed));

ARP Packet Header

struct arphdr {
    uint16_t htype;    /* Hardware Type           */
    uint16_t ptype;    /* Protocol Type           */
    u_char   hlen;     /* Hardware Address Length */
    u_char   plen;     /* Protocol Address Length */
    uint16_t opcode;   /* Operation Code          */
    u_char   sha[6];   /* Sender hardware address */
    u_char   spa[4];   /* Sender IP address       */
    u_char   tha[6];   /* Target hardware address */
    u_char   tpa[4];   /* Target IP address       */
};
"""

import socket
import struct
import binascii

rawSocket = socket.socket(socket.AF_PACKET,
                          socket.SOCK_RAW,
                          socket.htons(0x0003))

while True:

    packet = rawSocket.recvfrom(2048)
    ethhdr = packet[0][0:14]
    eth = struct.unpack("!6s6s2s", ethhdr)

    arphdr = packet[0][14:42]
    arp = struct.unpack("2s2s1s1s2s6s4s6s4s", arphdr)
    # skip non-ARP packets
    ethtype = eth[2]
    if ethtype != '\x08\x06': continue

    print "---------------- ETHERNET_FRAME ----------------"
    print "Dest MAC:        ", binascii.hexlify(eth[0])
    print "Source MAC:      ", binascii.hexlify(eth[1])
    print "Type:            ", binascii.hexlify(ethtype)
    print "----------------- ARP_HEADER -------------------"
    print "Hardware type:   ", binascii.hexlify(arp[0])
    print "Protocol type:   ", binascii.hexlify(arp[1])
    print "Hardware size:   ", binascii.hexlify(arp[2])
    print "Protocol size:   ", binascii.hexlify(arp[3])
    print "Opcode:          ", binascii.hexlify(arp[4])
    print "Source MAC:      ", binascii.hexlify(arp[5])
    print "Source IP:       ", socket.inet_ntoa(arp[6])
    print "Dest MAC:        ", binascii.hexlify(arp[7])
    print "Dest IP:         ", socket.inet_ntoa(arp[8])
    print "------------------------------------------------\n"
output:

$ python arp.py
---------------- ETHERNET_FRAME ----------------
Dest MAC:         ffffffffffff
Source MAC:       f0257252f5ca
Type:             0806
----------------- ARP_HEADER -------------------
Hardware type:    0001
Protocol type:    0800
Hardware size:    06
Protocol size:    04
Opcode:           0001
Source MAC:       f0257252f5ca
Source IP:        140.112.91.254
Dest MAC:         000000000000
Dest IP:          140.112.91.20
------------------------------------------------