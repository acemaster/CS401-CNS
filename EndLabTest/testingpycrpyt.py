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