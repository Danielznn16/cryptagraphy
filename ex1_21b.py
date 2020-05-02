import numpy as np

inW	= ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
inC = {}
for i in range(len(inW)):
	inC[inW[i]] = i

string='KCCPKBGUFDPHQTYAVINRRTMVGRKDNBVFDETDGILTXRGUDDKOTFMBPVGEGLTGCKQRACQCWDNAWCRXIZAKFTLEWRPTYCQKYVXCHKFTPONCQQRHJVAJUWETMCMSPKQDYHJVDAHCTRLSVSKCGCZQQDZXGSFRLSWCWSJTBHAFSIASPRJAHKJRJUMVGKMITZHFPDISPZLVLGWTFPLKKEBDPGCEBSHCTJRWXBAFSPEZQNRWXCVYCGAONWDDKACKAWBBIKFTIOVKCGGHJVLNHIFFSQESVYCLACNVRWBBIREPBBVFEXOSCDYGZWPFDTKFQIYCWHJVLNHIQIBTKHJVNPIST'
def rate(stringTmp):
	p = np.zeros(26)
	for i in stringTmp:
		p[inC[i]] += 1
	return (p * (p-1)).sum() / (p.sum() * (p.sum() - 1))
for m in range(1, 10):
	print(m, end=': ')
	for i in range(m):
		print('%.6f' % rate(string[i::m]), end=' ')
	print("\n")

def cosine_distance(x,y):
	return 1 - np.dot(x,y)/(np.linalg.norm(x)*np.linalg.norm(y))
table = np.array([0.0856,0.0139,0.0297,0.0378,0.1304,0.0289,0.0199,0.0528,0.0627,0.0013,0.0042,0.0339,0.0249,0.0707,0.0797,0.0199,0.0012,0.0677,0.0607,0.1045,0.0249,0.0092,0.0149,0.0017,0.0199,0.0008])
for l in range(6):
	tmpStr = string[l::6]
	re = {}
	for j in range(26):
		st = [0]*26
		for i in tmpStr:
			st[(inC[i]-j)%26] +=1/len(tmpStr)
		# print(st)
		re[inW[j]] = cosine_distance(table,np.array(st))
	print(sorted(re.items(),key= lambda x : x[1])[0])