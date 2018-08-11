#g++ -std=c++17 -I/usr/include/mysql uploadMl.cpp -o upRun  -L/usr/lib/mysql -lmysqlclient
g++ -std=c++17 -I/usr/include/mysql main.cpp -o run -L/usr/lib/mysql -lmysqlclient -pthread
