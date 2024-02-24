#!/usr/bin/sh

cd src/lib/con2db
echo "INSIDE con2db"
make clean
make
echo "DONE con2db"

cd ../con2redis/src
echo "INSIDE con2redis/src"
make clean
make
echo "DONE con2redis/src"

cd ../../../customer/managers/add-customer/src
echo "INSIDE add-customer/src"
make clean
make
echo "DONE add-customer/src"