inW	= ["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
inC = {}
for i in range(len(inW)):
	inC[inW[i]] = i
stat = {}
for i in inC:
	stat[i] = 0
strEnc = 'EMGLOSUDCGDNCUSWYSFHNSFCYKDPUMLWGYICOXYSIPJCKQPKUGKMGOLICGINCGACKSNISACYKZSCKXECJCKSHYSXCGOIDPKZCNKSHICGIWYGKKGKGOLDSILKGOIUSIGLEDSPWZUGFZCCNDGYYSFUSZCNXEOJNCGYEOWEUPXEZGACGNFGLKNSACIGOIYCKXCJUCIUZCFZCCNDGYYSFEUEKUZCSOCFZCCNCIACZEJNCSHFZEJZEGMXCYHCJUMGKUCY'
for i in strEnc:
	stat[i]+=1/len(strEnc)
stat = sorted(stat.items(),key=lambda x: x[1],reverse=True)
print(stat)

# Decode Stage 1
print("Stage 1")
strEnc = strEnc.replace('C','e').replace('F','w')

print(strEnc)


# Decode Stage 2
print("Stage 2")
strEnc = strEnc.replace('Z','h').replace('N','l')

print(strEnc)

# Decode Stage 3
print("Stage 3")
strEnc = strEnc.replace('U','t')

print(strEnc)

# Decode Stage 4
print("Stage 4")
strEnc = strEnc.replace('S','o').replace('O','n')

print(strEnc)


# Decode Stage 5
print("Stage 5")
strEnc = strEnc.replace('Y','r')

print(strEnc)

# Decode Stage 6
print("Stage 6")
strEnc = strEnc.replace('H','f')

print(strEnc)


# Decode Stage 7
print("Stage 7")
strEnc = strEnc.replace('W','g')

print(strEnc)


# Decode Stage 8
print("Stage 8")
strEnc = strEnc.replace('G','a')

print(strEnc)


# Decode Stage 9
print("Stage 9")
strEnc = strEnc.replace('E','i')

print(strEnc)

# Decode Stage 10
print("Stage 10")
strEnc = strEnc.replace('M','m').replace('L','y')

print(strEnc)


# Decode Stage 11
print("Stage 11")
strEnc = strEnc.replace('I','d')

print(strEnc)


# Decode Stage 12
print("Stage 12")
strEnc = strEnc.replace('A','v').replace('K','s')

print(strEnc)


# Decode Stage 13
print("Stage 13")
strEnc = strEnc.replace('J','c').replace('X','p')

print(strEnc)


# Decode Stage 14
print("Stage 14")
strEnc = strEnc.replace('D','b').replace('P','u')

print(strEnc)

