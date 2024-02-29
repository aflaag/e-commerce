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

cd ../../../customer/manager/add-card/src
echo "INSIDE add-card/src"
make clean
make
echo "DONE add-card/src"

cd ./../add-customer/src
echo "INSIDE add-customer/src"
make clean
make
echo "DONE add-customer/src"

cd ./../add-address/src
echo "INSIDE add-address/src"
make clean
make
echo "DONE add-address/src"

cd ./../add-purchase/src
echo "INSIDE add-purchase/src"
make clean
make
echo "DONE add-purchase/src"

cd ./../add-rating/src
echo "INSIDE add-rating/src"
make clean
make
echo "DONE add-rating/src"

cd ./../delete-purchase/src
echo "INSIDE delete-purchase/src"
make clean
make
echo "DONE delete-/src"


