inW	= ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
inC = {}
pi = [4,1,6,2,7,3,8,5]
for i in range(len(pi)):
	pi[i]-=1
print(pi)
back = {}
for i in range(len(pi)):
	back[pi[i]] = i
for i in range(len(inW)):
	inC[inW[i]] = i
cipher = "TGEEMNELNNTDROEOAAHDOETCSHAEIRLM"
code = list(cipher)
for i in range(int(len(cipher)/8)):
	for j in range(8):
		code[i*8+j] = cipher[i*8+back[j]]
print(('').join(code))