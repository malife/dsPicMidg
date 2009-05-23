# Makefile for midgSimParserTest
#
# make             makes midgSimParserTest
# make clean       removes object files
#

midgSimParserTest: midgSimParserTest.o midgSim.o circBuffer.o midg.o
	gcc -o midgSimParserTest midgSimParserTest.o midgSim.o circBuffer.o midg.o

midgSimParserTest.o: midgSimParserTest.c
	gcc -c -Wall midgSimParserTest.c

midgSim.o: midgSim.h midgSim.c
	gcc -c -Wall midgSim.c

circBuffer.o: circBuffer.h circBuffer.c
	gcc -c -Wall circBuffer.c
	
midg.o: midg.c midg.h apDefinitions.h circBuffer.h
	gcc -c -Wall midg.c

binaryCorruptor: binaryCorruptor.o
	gcc -o binaryCorruptor binaryCorruptor.o

binaryCorruptor.o: binaryCorruptor.c
	gcc -c -Wall binaryCorruptor.c

Parser: Parser.c
	gcc -o Parser -Wall Parser.c

clean:
	rm -f midgSimParserTest midgSimParserTest.o midgSim.o circBuffer.o midg.o