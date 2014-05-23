#!/bin/bash
if  [ -z $ARCH ]; then
	ARCH=i386
fi

echo "Pachetising for the $ARCH architecture"

rm -rf package && mkdir package
cd package
mkdir -p ps_rest_controller
curl http://peerstreamer.org/files/release/barepackage.tgz| tar xz -C ps_rest_controller
sed -i "s/ARCHITECTURE/$ARCH/g" ps_rest_controller/DEBIAN/control
sed -i "s/VERSION/0.0.0.0.0.1/g" ps_rest_controller/DEBIAN/control
cp -r ../public_html ps_rest_controller/opt/peerstreamer
cp ../rest_streamer_controller ps_rest_controller/opt/peerstreamer
cp ../streamer-udp-grapes-static ps_rest_controller/opt/peerstreamer
fakeroot dpkg --build ps_rest_controller

