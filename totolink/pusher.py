#!/usr/bin/python

import getpass
import sys
import telnetlib
import binascii
import time

def main():
	HOST = sys.argv[1]
	PORT = sys.argv[2]
	FILE = sys.argv[3]

	user = raw_input("Enter your remote account: ")
	password = getpass.getpass()

	tn = telnetlib.Telnet(HOST, PORT)

	tn.read_until("login: ")
	tn.write(user + "\n")
	if password:
		tn.read_until("Password: ")
		tn.write(password + "\n")

	#tn.write("ls\n")

	#print tn.read_all()

	with open(FILE, 'rb') as file:
		content = file.read()
		hexed_content = binascii.hexlify(content)

	head_message = 'echo -ne \"\" > /tmp/wget\n'
	tn.write(head_message)
	#tn.write('exit')
	sys.stdout.write(head_message)

	body_message = ''	
	for i in range(0, len(hexed_content), 2):
		body_message = body_message + '\\x' + hexed_content[i:i+2]

	send_payload(tn, body_message)

def send_payload(fd, message):
	BUFFSIZE = 40
	for i in range(0, len(message), BUFFSIZE):
		packet = 'echo -ne \"' + message[i:i + BUFFSIZE] + '\" >> /tmp/wget\n'
		fd.write(packet)	
		sys.stdout.write(packet)
		time.sleep(0.1)

if __name__ == "__main__":
	main()
