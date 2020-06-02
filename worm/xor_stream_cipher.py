### crake a file which applied one-byte xor stream cipher

# do the xor
def cipher(ct, key):
	dt = ''
	for c in ct:
		dt += chr( ord(c) ^ key )
	return dt

# open the file to be decipher
f = open('crack_me.log', 'r')
cipherText = f.readlines()[0]
f.close()

# guess the one-byte key
# for i in range(256):
	# print('key', i, 'plainText', cipher(cipherText, i))

# use the found key to decrypt and encrypt again after replace
key = 133
decipherText = cipher(cipherText, key)

print('original encrypted text', cipherText)
print('decrypted text with key ', key, ': ', decipherText)

# replace the specific string
decipherText = decipherText.replace('1234567', '0516016')
print('decrypted text with replacement', decipherText)

# encrypt with the same key again
cdt = cipher(decipherText, key)
print('encrypted validation after replace: ', cdt)

# write to the file
f = open('task1_result.log', 'w')
f.write(cdt)
f.close()

# validate
print('\n------------------------------\n')
f = open('task1_result.log', 'r')
c = f.readlines()[0]
f.close()
print('validation:')
print('encrypted text', c)
print('decrypted text', cipher(c, key))
