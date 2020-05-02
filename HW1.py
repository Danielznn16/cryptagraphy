import random
inW    = ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
wheel = [["E","K","M","F","L","G","D","Q","V","Z","N","T","O","W","Y","H","X","U","S","P","A","I","B","R","C","J"],["A","J","D","K","S","I","R","U","X","B","L","H","W","T","M","C","Q","G","Z","N","P","Y","F","V","O","E"],["B","D","F","H","J","L","C","P","R","T","X","V","Z","N","Y","E","I","W","G","A","K","M","U","S","Q","O"],["E","S","O","V","P","Z","J","A","Y","Q","U","I","R","H","X","L","N","F","T","G","K","D","C","M","W","B"],["V","Z","B","R","G","I","T","Y","U","P","S","D","N","H","L","X","A","W","M","J","Q","O","F","E","C","K"]]
reflec = ["Y","R","U","H","Q","S","L","D","P","X","N","G","O","K","M","I","E","B","F","Z","C","W","V","J","A","T"]
change = [17,5,22,10,0]
inC = {}
for i in range(len(inW)):
	inC[inW[i]] = i

class enigma:
	def __init__(self,wheels,offset):
		self.wh = []
		self.revert = [{},{},{}]
		self.flip = []
		self.off = []
		for i in wheels:
			self.flip.append(change[i])
			tmp = []
			for j in wheel[i]:
				tmp.append(j)
			self.wh.append(tmp)
		for i in range(26):
			for j in range(3):
				self.revert[j][self.wh[j][i]] = i
		if offset is not None:
			for k in offset:
				self.off.append(0+k)
		else:
			self.off = [0,0,0]
	def getChar(self,x):
		self.off[0] = (self.off[0] + 1)%26
		if self.off[0] == self.flip[0]:
			self.off[1] = (self.off[1] + 1)%26
			if self.off[1] == self.flip[1]:
				self.off[2] = (self.off[2] + 1)%26
		for i in range(3):
			x = inW[(inC[self.wh[i][(inC[x]+self.off[i])%26]]-self.off[i])%26]
			# print(x)
		x = reflec[inC[x]]
		# print(x)
		for i in [2,1,0]:
			x = inW[(self.revert[i][inW[(inC[x]+self.off[i])%26]]-self.off[i])%26]
		return x
	def getStr(self, String):
		re = ""
		String = String.upper()
		for i in String:
			re = re+(self.getChar(i))
		return re
en = enigma(wheels = [2,1,0], offset = [0,0,0])
en.getStr('test')


# Simulate encription
SecureWheel = [0,1,2]
SecureOffset = []
random.shuffle(SecureWheel)
for i in range(3):
	SecureOffset.append(random.randint(0,25))
possW = []
possO = []
possKey = []
passW = ["xyzxyz","abcabc"]
print("before Encripte Wheel: %s\tOffset: %s"%(SecureWheel,SecureOffset))
print("Keys: %s"%(passW))

#Encripte Key
encriptPassW = []
for i in passW:
	tmpEn = enigma(wheels = SecureWheel, offset = SecureOffset)
	encriptPassW.append(tmpEn.getStr(i))
print("Encripted Key")
print(encriptPassW)

# Decription
for i in range(3):
	for j in range(3):
		if j != i:
			for k in range(3):
				if k != i and k != j:
					tmpW = [i,j,k]
					for q in range(26):
						for w in range(26):
							for e in range(26):
								tmpO = [q,w,e]
								keepGoing = True
								l = list(range(len(encriptPassW)))
								keyDecript = []
								while keepGoing and len(l)!=0:
									tryEnig = enigma(wheels = tmpW,offset = tmpO)
									tmpStr = tryEnig.getStr(encriptPassW[l[0]])
									l.pop(0)
									keyDecript.append(tmpStr)
									keepGoing = (tmpStr[0]==tmpStr[3]) and (tmpStr[1]==tmpStr[4]) and (tmpStr[2]==tmpStr[5])
								if keepGoing:
									possW.append(tmpW)
									possO.append(tmpO)
									possKey.append(keyDecript)
print(possW)
print("possible results:")
for i in range(len(possW)):
	print("wheels: %s\toffsets: %s\tkeys: %s"%(possW[i],possO[i],possKey[i]))