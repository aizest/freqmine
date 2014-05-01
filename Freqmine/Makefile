PREFIX=${PARSECDIR}/pkgs/apps/freqmine/inst/${PARSECPLAT}

TARGET = freqmine

CXXFLAGS += -Wno-deprecated

all: $(TARGET)

$(TARGET): fpgrowth.o data.o fp_tree.o buffer.o fsout.o fp_node.o wtime.o fp_thread_manager.o fp_thread.o threadjob.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) fpgrowth.o data.o fp_tree.o buffer.o fsout.o fp_node.o wtime.o fp_thread_manager.o fp_thread.o threadjob.o -lpthread $(LIBS) -o $(TARGET)

wtime.o: wtime.cpp wtime.h
	$(CXX) $(CXXFLAGS) -c wtime.cpp -o wtime.o

fpgrowth.o: fpmax.cpp data.h fp_tree.h common.h fsout.h
	$(CXX) $(CXXFLAGS) -c fpmax.cpp -o fpgrowth.o

data.o: data.cpp data.h
	$(CXX) $(CXXFLAGS) -c data.cpp

fp_tree.o: fp_tree.cpp data.h fp_tree.h common.h fsout.h
	$(CXX) $(CXXFLAGS) -c fp_tree.cpp

buffer.o: buffer.cpp data.h buffer.h
	$(CXX) $(CXXFLAGS) -c buffer.cpp

fsout.o: fsout.cpp fsout.h common.h
	$(CXX) $(CXXFLAGS) -c fsout.cpp

fp_node.o: fp_node.cpp fp_node.h fsout.h common.h
	$(CXX) $(CXXFLAGS) -c fp_node.cpp
	
fp_thread_manager.o: fp_thread_manager.cpp fp_thread_manager.h
	$(CXX) $(CXXFLAGS) -c fp_thread_manager.cpp
	
fp_thread.o: fp_thread.cpp fp_thread.h
	$(CXX) $(CXXFLAGS) -c fp_thread.cpp
	
threadjob.o: ThreadJob.cpp ThreadJob.h
	$(CXX) $(CXXFLAGS) -c ThreadJob.cpp

#thread_test.o: thread_test.cpp
	#$(CXX) $(CXXFLAGS) -c thread_test.cpp

clean:
	rm -f *.o $(TARGET)

install:
	mkdir -p $(PREFIX)/bin
	cp -f $(TARGET) $(PREFIX)/bin/$(TARGET)

