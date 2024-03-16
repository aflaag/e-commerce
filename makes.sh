#!/bin/bash

ROOT=$(pwd)
CURRENT_ROOT=""

CLEAN=false

check_arguments() {
    if [ "$1" = "--clean" ] || [ "$1" = "-c" ]
    then
        CLEAN=true
    fi
}

check_last_command() {
    if [ $? -ne 0 ]
    then
        printf "\n\n\n!!!!!!!!!! An error occurred while trying to run the script !!!!!!!!!!\n"
        exit
    fi
}

try_cd() {
    cd $1

    check_last_command
}

set_current_root() {
    CURRENT_ROOT=$1

    try_cd "$ROOT/$CURRENT_ROOT"
}

try_make() {
    if $CLEAN = true
    then
        make clean
    else
        make
    fi

    check_last_command
}

make_folder() {
    try_cd $1

    printf "INSIDE $1\n"

    try_make

    printf "DONE $1\n\n"

    try_cd "$ROOT/$CURRENT_ROOT"
}

check_arguments $1

printf "\n\n\n########## Started make script ##########\n\n\n\n"

printf "############## LIB ##############\n\n"

set_current_root src/lib

make_folder con2db
make_folder con2redis/src

printf "############## UTILS ##############\n\n"

set_current_root src

make_folder utils/src

printf "############## CLASSES ##############\n\n"

set_current_root src

make_folder classes/src

printf "############## SERVER ##############\n\n"

set_current_root src

make_folder server/src

printf "############## NON FUNCTIONAL REQ ##############\n\n"

set_current_root src

make_folder non-functional-req/src

printf "############## CUSTOMER ##############\n\n"

set_current_root src/customer

make_folder handler/src

make_folder managers/add-address/src
make_folder managers/add-card/src
make_folder managers/add-customer/src
make_folder managers/add-purchase/src
make_folder managers/add-rating/src
make_folder managers/add-refund-request/src
make_folder managers/delete-purchase/src
make_folder managers/search-products/src
make_folder managers/view-order/src
make_folder managers/view-refund/src

printf "############## SUPPLIER ##############\n\n"

set_current_root src/supplier

make_folder handler/src

make_folder managers/add-product/src
make_folder managers/add-restock/src
make_folder managers/add-supplier/src
make_folder managers/update-product/src
make_folder managers/view-statistic/src

printf "############## COURIER ##############\n\n"

set_current_root src/courier

make_folder handler/src

make_folder managers/take-purchase-delivery/src
make_folder managers/update-assigned-delivery/src
make_folder managers/update-refund-request/src
make_folder managers/view-available-purchase/src

printf "\n\n\n########## Make script completed ###########\n\n\n\n"
