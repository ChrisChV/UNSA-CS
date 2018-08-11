import MySQLdb

db = MySQLdb.connect(host="localhost",    # your host, usually localhost
                     user="root",         # your username
                     passwd="root",  # your password
                     db="ml")        # name of the data base

# you must create a Cursor object. It will let
#  you execute all the queries you need
cur1 = db.cursor()
cur2 = db.cursor()

# Use all the SQL you like

numUsers = 672

outFile = open("expPyOut", "w");

for i in range(1,numUsers):
	print(str(i) + "/" + str(numUsers))
	cur1.execute("select MovieId from ratings where userId = " + str(i))
	for row1 in cur1.fetchall():
		cur2 = db.cursor()
		cur2.execute("call recommendSlopeTest(" + str(i) + "," + str(row1[0]) + ")")
		for row2 in cur2.fetchall():
			outFile.write(str(i) + " " + str(row1[0]) + " " + str(row2[3]))
		cur2.close()

outFile.close()
	


db.close()