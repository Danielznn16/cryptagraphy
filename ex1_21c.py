inW	= ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
inC = {}
for i in range(len(inW)):
	inC[inW[i]] = i
stat = {}
for i in inC:
	stat[i] = 0
strEnc = 'KQEREJEBCPPCJCRKIEACUZBKRVPKRBCIBQCARBJCVECUPKRIOFKPACUZQEPBKRXPEIIEABDKPBCPFCDCCAFIEABDKPBCPFEQPKAZBKRHAIBKAPCCIBURCCDKDCCJCIDFUIXPAFFERBICZDFKABICBBENEFCUPJCVKABPCYDCCDPKBCOCPERKIVKSCPICBRKIJPKABI'
for i in strEnc:
	stat[i]+=1/len(strEnc)
stat = sorted(stat.items(),key=lambda x: x[1],reverse=True)
print(stat)

re = ""
for i in strEnc:
	re = re+inW[(inC[i]*11+8)%26]
print(re)