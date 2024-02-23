#!/usr/bin/sh

cd src/lib/con2db
make clean
make

cd ../con2redis/src
make clean
make

cd ../../../../customer/managers/add-customer
make clean
make