TRDPARTYLIB = thirdparty-lib
LIB_DIR = -L$(TRDPARTYLIB)/mhttpd/lib -L$(TRDPARTYLIB)/boost/lib
INCLUDE = -I$(TRDPARTYLIB)/boost/include -I$(TRDPARTYLIB)/mhttpd/include -I.

LIBS = -lmicrohttpd -lpthread -lrt -lboost_filesystem
CXX = g++
CXX_FLAGS = -static 
EXE = rest_streamer_controller 

$(EXE): lib_prepare
	$(CXX) $(CXX_FLAGS) -o $(EXE) httphandler.cpp strutil.cpp  api.cpp executor.cpp $(INCLUDE) $(LIB_DIR) $(LIBS) 

lib_prepare:
	make -C $(TRDPARTYLIB)

clean:
	rm -f $(EXE)

distclean: clean
	make -C $(TRDPARTYLIB) clean
