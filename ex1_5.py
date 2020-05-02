inW	= ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
inC = {}
for i in range(len(inW)):
	inC[inW[i]] = i
secure = "BEEAKFYDJXUQYHYJIQRYHTYJIQFBQDUYJIIKFUHCQD"
scureToNum = []
for a in secure:
	scureToNum.append(inC[a])
for i in range(26):
	print("offset %s"%(i))
	testStr = ""
	for j in scureToNum:
		testStr = testStr+inW[(j+i)%26]
	print("result: %s"%(testStr))