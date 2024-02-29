#!/usr/bin/sh

cd src/lib/con2db
echo "INSIDE con2db"
make clean
make
printf "DONE con2db\n\n"

cd ../con2redis/src
echo "INSIDE con2redis/src"
make clean
make
printf "DONE con2redis/src\n\n"

cd ../../../utils/src
echo "INSIDE utils/src"
make clean
make
printf "DONE utils/src\n\n"

cd ../../customer/managers/add-card/src
echo "INSIDE add-card/src"
make clean
make
printf "DONE add-card/src\n\n"

cd ./../add-customer/src
echo "INSIDE add-customer/src"
make clean
make
printf "DONE add-customer/src\n\n"

cd ./../add-address/src
echo "INSIDE add-address/src"
make clean
make
printf "DONE add-address/src\n\n"

cd ./../add-purchase/src
echo "INSIDE add-purchase/src"
make clean
make
printf "DONE add-purchase/src\n\n"

cd ./../add-rating/src
echo "INSIDE add-rating/src"
make clean
make
printf "DONE add-rating/src\n\n"

cd ./../delete-purchase/src
echo "INSIDE delete-purchase/src"
make clean
make
printf "DONE delete-purchase/src\n\n"

cd ./../add-refund-request/src
echo "INSIDE add-refund-request/src"
make clean
make
printf "DONE add-refund-request/src\n\n"


