LIB_DIR = -L../boost/lib -L../microhttpd/lib
LIBS = -lmicrohttpd -lboost_regex -lpthread -lrt
INCLUDE = -I../boost/include -I../microhttpd/include -I.
CXX = g++
CXX_FLAGS = -static 
EXE = rest_streamer_controller 

all:
	$(CXX) $(CXX_FLAGS) -o $(EXAMPLE_EXE) httphandler.cpp strutil.cpp  api.cpp executor.cpp $(INCLUDE) $(LIB_DIR) $(LIBS) 

clean:
	rm -f $(EXE)
