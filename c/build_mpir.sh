#!/bin/bash

if [ ! -e mpir-2.6.0.tar.bz2 ]; then 
	wget http://www.mpir.org/mpir-2.6.0.tar.bz2;
fi
tar xjf mpir-2.6.0.tar.bz2

pushd mpir-2.6.0
./configure --prefix=`pwd`/../local/
make
make install
popd
