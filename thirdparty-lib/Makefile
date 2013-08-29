BOOST_LIB = $(CURDIR)/boost
MHTTPD_LIB = $(CURDIR)/mhttpd

lib_prepare: boost mhttpd

boost:
	wget http://garr.dl.sourceforge.net/project/boost/boost/1.54.0/boost_1_54_0.tar.bz2
	tar --bzip2 -xf *.tar.bz2  && rm *.tar.bz2
	cd boost_1_54_0 && ./bootstrap.sh --prefix=$(BOOST_LIB)
	cd boost_1_54_0 && ./b2 install
	rm -rf boost_1_54_0

mhttpd:
	wget http://ftpmirror.gnu.org/libmicrohttpd/libmicrohttpd-0.9.26.tar.gz
	tar xzf libmicrohttpd-0.9.26.tar.gz && rm libmicrohttpd-0.9.26.tar.gz
	cd libmicrohttpd-0.9.26 && ./configure --prefix=$(MHTTPD_LIB)
	make -C  libmicrohttpd-0.9.26
	make -C  libmicrohttpd-0.9.26 install
	rm -rf libmicrohttpd-0.9.26
	
clean:
	rm -rf $(BOOST_LIB) $(MHTTPD_LIB)
