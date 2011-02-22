CFLAGS:=-c -Wall -g -O3 -fpic
CC:=g++

all: libnntp.so libnntp.a

install: install-shared install-static

install-shared: libnntp.so
	cp libnntp.so /usr/local/lib/

install-static: libnntp.a
	cp libnntp.a /usr/local/lib/

install-headers:
	mkdir -p /usr/local/include/nntp/
	cp article.h decoded_article.h exceptions.h group.h intrusive_ptr.h nntp.h socket_wrapper.h /usr/local/include/nntp/

libnntp.so: socket_wrapper.o nntp.o group.o article.o decoded_article.o
	$(CC) -shared -fpic -o libnntp.so nntp.o socket_wrapper.o group.o article.o decoded_article.o -lboost_system -lssl

libnntp.a: socket_wrapper.o nntp.o group.o article.o decoded_article.o
	ar rs libnntp.a nntp.o socket_wrapper.o group.o article.o decoded_article.o

socket_wrapper.o: socket_wrapper.cc socket_wrapper.h intrusive_ptr.h
	$(CC) $(CFLAGS) socket_wrapper.cc

nntp.o: nntp.cc nntp.h group.h socket_wrapper.o
	$(CC) $(CFLAGS) nntp.cc

group.o: group.cc group.h nntp.h article.h intrusive_ptr.h
	$(CC) $(CFLAGS) group.cc

article.o: article.cc article.h group.h nntp.h decoded_article.h
	$(CC) $(CFLAGS) article.cc

decoded_article.o: decoded_article.cc decoded_article.h 
	$(CC) $(CFLAGS) decoded_article.cc
