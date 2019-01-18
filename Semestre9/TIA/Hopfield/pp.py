from mnist import MNIST

mndata = MNIST('samples')

images, labels = mndata.load_training()
# or
#images, labels = mndata.load_testing()

index = 0
print(mndata.display(images[index]))
print(len(images[0]))

#file = open("test", "w")

#for i in range(0,len(images)):
#	print(i)
#	for j in range(0,len(images[i])):
#		if images[i][j] != 0:
#			file.write("1 ")
#		else:
#			file.write("-1 ")
#	file.write("\n")
#file.close()

#for i in range(0,len(images)):
