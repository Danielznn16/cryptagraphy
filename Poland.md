

```python
import random
inW	= ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
wheel = [["E","K","M","F","L","G","D","Q","V","Z","N","T","O","W","Y","H","X","U","S","P","A","I","B","R","C","J"],["A","J","D","K","S","I","R","U","X","B","L","H","W","T","M","C","Q","G","Z","N","P","Y","F","V","O","E"],["B","D","F","H","J","L","C","P","R","T","X","V","Z","N","Y","E","I","W","G","A","K","M","U","S","Q","O"],["E","S","O","V","P","Z","J","A","Y","Q","U","I","R","H","X","L","N","F","T","G","K","D","C","M","W","B"],["V","Z","B","R","G","I","T","Y","U","P","S","D","N","H","L","X","A","W","M","J","Q","O","F","E","C","K"]]
reflec = ["Y","R","U","H","Q","S","L","D","P","X","N","G","O","K","M","I","E","B","F","Z","C","W","V","J","A","T"]
change = [17,5,22,10,0]
inC = {}
for i in range(len(inW)):
	inC[inW[i]] = i

class enigma:
	def __init__(self,wheels,offset,swap=None):
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
# 		print(self.off)
		self.swap = {}
		if swap != None:
			for i in swap:
				self.swap[i] = swap[i]
				self.swap[swap[i]] = i
	def getChar(self,x):
		if x in self.swap:
			x = self.swap[x]
		self.off[0] = (self.off[0] + 1)%26
# 		print("SpinWheel A")
		if self.off[0] == self.flip[0]:
# 			print("SpinWheel B")
			self.off[1] = (self.off[1] + 1)%26
			if self.off[1] == self.flip[1]:
# 				print("SpinWheel C")
				self.off[2] = (self.off[2] + 1)%26
		elif (self.off[1]+1)%26 == self.flip[1]:
			self.off[1] = (self.off[1]+1)%26
			self.off[2] = (self.off[2]+1)%26
		for i in range(3):
			x = inW[(inC[self.wh[i][(inC[x]+self.off[i])%26]]-self.off[i])%26]
			# print(x)
		x = reflec[inC[x]]
		# print(x)
		for i in [2,1,0]:
			x = inW[(self.revert[i][inW[(inC[x]+self.off[i])%26]]-self.off[i])%26]
		if x in self.swap:
			x = self.swap[x]
		return x
	def getStr(self, String):
		re = ""
		String = String.upper()
		for i in String:
			re = re+(self.getChar(i))
		return re
```


```python
chart = {}
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
								tmpChart = [{},{},{}]
								tmpLoopStr = ""
								tmpLoopArr = []
								for l in range(26):
									tmpE = enigma(tmpW,tmpO)
									tmpChar = inW[l]
# 									print(tmpChar+tmpChar+tmpChar+tmpChar+tmpChar+tmpChar)
									tmpStr = tmpE.getStr(tmpChar+tmpChar+tmpChar+tmpChar+tmpChar+tmpChar)
									tmpChart[0][tmpStr[0]] = tmpStr[3]
									tmpChart[1][tmpStr[1]] = tmpStr[4]
									tmpChart[2][tmpStr[2]] = tmpStr[5]
								for z in tmpChart:
									loop = []
									checked = []
									for l in z:
										if l not in checked:
											checked.append(l)
											p = z[l]
											cnt = 1
											while p not in checked:
												cnt+=1
												checked.append(p)
												p = z[p]
# 										if cnt > 2:
# 											print(cnt)
											loop.append(cnt)
									loop.sort()
									tmpLoopArr.append(str(loop))
									tmpLoopStr = tmpLoopStr + str(loop)
# 								if tmpLoopArr[0] == "[1, 1, 12, 12]" and tmpLoopArr[1] == "[1, 1, 2, 2, 10, 10]":
# 									print(str(tmpLoopArr[2]))
								if tmpLoopStr not in chart:
									chart[tmpLoopStr] = []
								chart[tmpLoopStr].append((tmpW,tmpO))

```


```python
# Simulate encription
SecureWheel = [0,1,2]
SecureOffset = []
SecureSwap = {}
random.shuffle(SecureWheel)
for i in range(3):
	SecureOffset.append(random.randint(0,25))
SecureSwaped = []
for i in range(5):
	a = inW[random.randint(0,25)]
	while a in SecureSwaped:
		a = inW[random.randint(0,25)]
	SecureSwaped.append(a)
	b = inW[random.randint(0,25)]
	while b in SecureSwaped:
		b = inW[random.randint(0,25)]
	SecureSwaped.append(b)
	SecureSwap[a] = b
print("before Encripte Wheel: %s\tOffset: %s"%(SecureWheel,SecureOffset))
print("swap set:")
for i in SecureSwap:
    print("%s<->%s"%(i,SecureSwap[i]))

#Encripte Key

passW = []
for i in range(10000):
    tmpStr = inW[random.randint(0,25)] + inW[random.randint(0,25)] + inW[random.randint(0,25)]
    tmpStr = tmpStr+tmpStr
    passW.append(tmpStr)
print("Keys: %s..."%(passW[:20]))

encriptPassW = []
for i in passW:
	tmpEn = enigma(wheels = SecureWheel, offset = SecureOffset,swap=SecureSwap)
	encriptPassW.append(tmpEn.getStr(i))
print("Encripted Key: %s..."%(encriptPassW[:20]))
```

    before Encripte Wheel: [1, 2, 0]	Offset: [24, 10, 17]
    swap set:
    P<->B
    K<->J
    A<->H
    V<->U
    G<->Q
    Keys: ['TSXTSX', 'JLNJLN', 'UBUUBU', 'AHSAHS', 'LLRLLR', 'QCCQCC', 'OVDOVD', 'DGGDGG', 'JBGJBG', 'FQUFQU', 'DIUDIU', 'IQPIQP', 'QMVQMV', 'TLJTLJ', 'OSKOSK', 'WGIWGI', 'JPMJPM', 'YCBYCB', 'TPKTPK', 'QRCQRC']...
    Encripted Key: ['HIHBPU', 'SBTYRQ', 'XLAMIX', 'GWCGTT', 'QBWHRL', 'LESEZJ', 'KXOROZ', 'YRICKH', 'SLIYIH', 'CKAVFX', 'YSACBX', 'ZKEWFW', 'LZZEXE', 'HBFBRC', 'KIQRPO', 'NRGIKA', 'SOYYSB', 'DELJZM', 'HOQBSO', 'LGSELJ']...



```python
# Calc Loop
loop = [{},{},{}]
for i in encriptPassW:
    for j in range(3):
        loop[j][i[j]] = i[j+3]
chartIndex = ""
for j in range(3):
    cycle = []
    checked = []
    for i in inW:
        if i not in checked:
            checked.append(i)
            l = loop[j][i]
            cnt = 1
            while l not in checked:
                checked.append(l)
                l = loop[j][l]
                cnt+=1
            cycle.append(cnt)
    cycle.sort()
    chartIndex = chartIndex + str(cycle)
```


```python
# Crack the PassWord With Poland's Method
possW = []
possO = []
possKey = []
possibility = []
guessSwapped = []
for (tmpW,tmpO) in chart[chartIndex]:
	keepGoing = True
	l = list(range(len(encriptPassW)))
	guess = {}
	for j in inW:
		guess[j] = 0
	keyDecript = []
	cnt = 0
	while len(l)!=0:
		tryEnig = enigma(wheels = tmpW,offset = tmpO)
		tmpStr = tryEnig.getStr(encriptPassW[l[0]])
		l.pop(0)
		keyDecript.append(tmpStr)
		if (tmpStr[0]==tmpStr[3]) and (tmpStr[1]==tmpStr[4]) and (tmpStr[2]==tmpStr[5]):
			cnt +=1
		else:
			for i in range(3):
				if (tmpStr[i]!=tmpStr[i+3]):
					guess[tmpStr[i]]+=1
					guess[tmpStr[i+3]]+=1
	if cnt>0:
		possW.append(tmpW)
		possO.append(tmpO)
		possKey.append(keyDecript)
		possibility.append(cnt)
		guessSwapped.append(guess)
# print(possW)
print("possible results:")
#''',sorted(guessSwapped[i].items(), key=lambda d: d[1],reverse=True)'''
for i in range(len(possW)):
	print("wheels: %s\toffsets: %s\tkeys: %s\tmatched key num: %s \n"%(possW[i],possO[i],possKey[i][:20],possibility[i]))
```

    possible results:
    wheels: [0, 2, 1]	offsets: [13, 3, 3]	keys: ['ZUVCTA', 'BKRVFS', 'OGDFXL', 'PAEZPI', 'VKSKFX', 'DZWLCH', 'IYXIMB', 'CJZBEJ', 'BGZVXJ', 'YBDYRL', 'CDDBDL', 'HBCJRN', 'DEILIY', 'ZKBCFK', 'IUKITG', 'MJUREU', 'BPLVHZ', 'LZYWCV', 'ZPKCHG', 'DLWLYH']	matched key num: 8 
    
    wheels: [1, 0, 2]	offsets: [18, 12, 25]	keys: ['TCOXBO', 'AMMCOG', 'PEULUE', 'WXFWXB', 'ZMEUOY', 'KLRRFF', 'LWHERR', 'ESBYMV', 'AEBCUV', 'UDUOZE', 'ERUYPE', 'QDWGZK', 'KUGRTX', 'TMCXOM', 'LCLEBU', 'OSZJMP', 'APDCLT', 'BLQIFC', 'TPLXLU', 'KVRRSF']	matched key num: 3 
    
    wheels: [1, 0, 2]	offsets: [25, 20, 9]	keys: ['MFFEZE', 'EYKLWI', 'VRWUYB', 'UHNSJJ', 'AYANWZ', 'DJBBPT', 'WAVWLL', 'NLDVCM', 'ERDLYM', 'PCWCBB', 'NOWVFB', 'ICLRBY', 'DMMBQU', 'MYHEWG', 'WFPWZP', 'YLYXCK', 'ESGLMX', 'LJEDPH', 'MSPEMP', 'DTBBOT']	matched key num: 2 
    
    wheels: [1, 2, 0]	offsets: [2, 13, 24]	keys: ['BUOLBM', 'VLNRIV', 'TBEKRE', 'IGBTZP', 'ULUFIO', 'MAXWTN', 'EZHYNA', 'CMRSDY', 'VBRRRY', 'YJEJGE', 'CHESPE', 'NJAEGB', 'MXMWLX', 'BLGLIR', 'EUPYBL', 'ZMFZDZ', 'VQKRHW', 'JADVTU', 'BQPLHL', 'MWXWXN']	matched key num: 2 
    
    wheels: [1, 2, 0]	offsets: [10, 19, 22]	keys: ['NOJSNR', 'CQSFWW', 'AULPBJ', 'JDETJB', 'KQDRWG', 'MZTLFX', 'QKFHYY', 'BGPDQC', 'CUPFBC', 'SXLAZJ', 'BHLDIJ', 'UXCKZQ', 'MERLCF', 'NQOSWH', 'QOGHNP', 'HGQJQD', 'CIVFGT', 'VZAIFI', 'NIGSGP', 'MRTLHX']	matched key num: 1 
    
    wheels: [1, 2, 0]	offsets: [24, 10, 17]	keys: ['QSVSIE', 'KONKLA', 'VPXVPV', 'LASEAS', 'HORQLR', 'GCCGCO', 'SUDOUD', 'DQQDYI', 'KPQKPI', 'FYXMGV', 'DIXDSV', 'IYBIGB', 'GMUGMU', 'QOKSLK', 'SSIOIJ', 'WQJWYQ', 'KBMKBW', 'YCPNCP', 'QBISBJ', 'GJCGRO']	matched key num: 570 
    
    wheels: [2, 1, 0]	offsets: [19, 1, 9]	keys: ['CVWFNG', 'VRLZBZ', 'GPMDYL', 'XDDTLP', 'ERHRBX', 'ZKQQAK', 'BMUHWQ', 'JBVJUB', 'VPVZYB', 'HEMIXL', 'JZMJRL', 'LERPXN', 'ZSKQFY', 'CRPFBM', 'BVSHNR', 'RBYVUS', 'VCGZGH', 'OKTCAC', 'CCSFGR', 'ZJQQTK']	matched key num: 2 
    

