CC=g++
binPath=../bin/

all:
	make clean
	mkdir ${binPath}
	${CC} -o ${binPath}noop noop.cpp
	${CC} -o ${binPath}number number.cpp
	${CC} -o ${binPath}delayedremovetag delayedremovetag.cpp
	${CC} -o ${binPath}removetag removetag.cpp
	${CC} -o ${binPath}removetag0 removetag0.cpp
	cp /bin/ls ${binPath}
	cp /bin/cat ${binPath}
	
clean:
	rm -rf ${binPath}