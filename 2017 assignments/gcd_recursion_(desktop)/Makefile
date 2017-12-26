OBJS= exercise7.o gcd.o #make objects
PROGRAM= exercise7 #final program name
FLAGS = -c -Wall #flags at the end for g++

$(PROGRAM): $(OBJS) #compile main program
	g++ $(OBJS) -o $(PROGRAM)

#compile the objects: 
exercise7.o: exercise7.cpp gcd.h
	g++ exercise7.cpp $(FLAGS)

gcd.o: gcd.cpp gcd.h
	g++ gcd.cpp $(FLAGS)

clean: #remove program objects and executable
	@rm -f $(OBJS)
	@rm -f exercise7
