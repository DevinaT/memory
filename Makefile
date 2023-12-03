CC = gcc
CARGS = -Wall -std=c11

all: bench

bench: bench.o getmem.o freemem.o mem_utils.o
	$(CC) $(CARGS) -o bench $^

%.o: %.c mem.h mem_impl.h
	$(CC) $(CARGS) -c $<

debug: CARGS += -g -D DEBUG
debug: bench

noassert: CARGS += -D NDEBUG
noassert: bench

test: debug
	# ./bench 1000 10
	./bench 5
	# ./bench 50000 10
	# ./bench 2 10 100
	# ./bench 18 5 100 16
	# ./bench 3 100 100 500 2000 1
	# ./bench 4 50 50 500 2000 1
	# ./bench 1 50 50 500 2000 1

clean:
	rm -rf bench *.o *~

# CC = gcc
# CARGS = -Wall -std=c11

# all: bench

# bench: bench.o getmem.o freemem.o mem_utils.o
# 	$(CC) $(CARGS) -o bench $^

# getmem: getmem.o mem_utils.o
# 	$(CC) $(CARGS) -o getmem $^

# freemem: freemem.o mem_utils.o
# 	$(CC) $(CARGS) -o freemem $^

# mem_utils: mem_utils.o
# 	$(CC) $(CARGS) -o mem_utils $^

# %.o: %.c mem.h mem_impl.h
# 	$(CC) $(CARGS) -c $<

# debug: CARGS += -g -D DEBUG
# debug: bench getmem freemem mem_utils

# noassert: CARGS += -D NDEBUG
# noassert: bench getmem freemem mem_utils

# test: debug
# 	./bench 1000 10
# 	./bench 5
# 	./bench 50000 10
# 	./bench 2 10 100
# 	./bench 18 5 100 16
# 	./bench 3 100 100 500 2000 1
# 	./bench 4 50 50 500 2000 1
# 	./bench 1 50 50 500 2000 1

# clean:
# 	rm -rf bench getmem freemem mem_utils *.o *~
