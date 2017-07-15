import socket
import sys

USAGE = "Usage: totolink_backdoor.py [-i target_ip -p target_port]"
DEFAULT_IP = "192.168.1.1"
DEFAULT_PORT = 5555

def main():

	target_ip = DEFAULT_IP
	target_port = DEFAULT_PORT

	for i in range(len(sys.argv)):
		if sys.argv[i] == "-i":
			target_ip = sys.argv[i+1]
			i = i + 1
		if sys.argv[i] == "-p":
			target_port = sys.argv[i+1]
			i = i + 1

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	try:
		s.connect((target_ip, int(target_port)))
	except socket.error as msg:
		print "Socket Error:Totolink target is not vulnerable to backdoor, try another... good luck!" 
		return 0

	test_string = "Hello there"
	s.sendall(test_string)
	data = s.recv(1024)
	
	s.close()
	if data == test_string:
		print "Totolink target is vulnerable to backdoor"
		return 1
	else:
		print "Totolink target is not vulnerable to backdoor, try another... good luck!"
		return 0
if __name__ == "__main__":
	main()
