def decode(s,m,n):
	r = ""
	for k in range(0,len(s),m*n):
		for i in range(n):
			for j in range(m):
				r = r+s[k+j*n+i]
	return r

print(decode("ctaropyghpry",4,3))
# cryptography
s='myamraruyiqtenctorahroywdsoyeouarrgdernogw'
print(decode(s, 3, 2))
# marymaryquitecontraryhowdoesyourgardengrow