def suma (X):
	return sum(X)

X = [[1,1],[1,2],[1,3],[1,4],[1,5]]
Y = [X[i][1] for i in range(0,len(X))]
print Y