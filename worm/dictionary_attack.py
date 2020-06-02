from itertools import permutations

dictionary = ['YueHan', 'Wang', 'YH', '1999',
              '0228', 'oscar', 'Realtek', '@', '_']


possible_passwd = [p[0] + p[1] for p in permutations(dictionary, 2)]

print(possible_passwd)
