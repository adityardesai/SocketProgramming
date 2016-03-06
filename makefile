all:serverA serverB serverC serverD client client2

client:	client.o 
	gcc -o client -g client.o -lresolv -lnsl -lsocket

client.o: client.c constants.h
	gcc -g -c -Wall client.c -lresolv -lnsl -lsocket

client2:	client2.o 
	gcc -o client2 -g client2.o -lresolv -lnsl -lsocket

client2.o: client2.c constants.h
	gcc -g -c -Wall client2.c -lresolv -lnsl -lsocket
 
serverD: serverD.o 
	gcc -o serverD -g serverD.o -lresolv -lnsl -lsocket

serverD.o: serverD.c constants.h
	gcc -g -c -Wall serverD.c -lresolv -lnsl -lsocket

serverC: serverC.o 
	gcc -o serverC -g serverC.o -lresolv -lnsl -lsocket

serverC.o: serverC.c constants.h
	gcc -g -c -Wall serverC.c -lresolv -lnsl -lsocket

serverB: serverB.o 
	gcc -o serverB -g serverB.o -lresolv -lnsl -lsocket

serverB.o: serverB.c constants.h
	gcc -g -c -Wall serverB.c -lresolv -lnsl -lsocket

serverA: serverA.o 
	gcc -o serverA -g serverA.o  -lresolv -lnsl -lsocket

serverA.o: serverA.c constants.h
	gcc -g -c -Wall serverA.c -lresolv -lnsl -lsocket

clean:
	rm -f *.o serverA serverB serverC serverD client client2
