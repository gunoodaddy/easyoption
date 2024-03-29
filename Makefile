CXX=/usr/bin/gcc
MKDEPEND=/usr/bin/gcc -MM
TARGET=tester
SOURCES=tester.c
OBJECTS=$(SOURCES:.c=.o)
#LIBS=-ltcmalloc
CFLAGS=-g -Wall -Idep -L/usr/local/lib -D_GNU_SOURCE 

$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET) $(OBJECTS) .depend

dep: depend
depend:
	/bin/rm -f .depend
	@for fn in $(SOURCES); \
		do \
		echo "$(MKDEPEND) $(CFLAGS) $$fn >> .depend";\
		pathnm=`dirname $$fn`; \
		if [ -n $$pathnm -a '.' != $$pathnm ]; \
		then \
		echo -n $$pathnm/ >> .depend;\
		fi;\
		$(MKDEPEND) $(CFLAGS) $$fn >> .depend;\
		done;

-include .depend
