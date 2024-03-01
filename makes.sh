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
        printf "\n\n\n😭😭😭😭😭😭😭😭😭😭 FAILED COOKIN' 😭😭😭😭😭😭😭😭😭😭\n"
        printf "\n\n\n🗣️🗣️🗣️🗣️🗣️🗣️🗣️🗣️ WE 👏 ARE 👏 SO 👏 BACK 🗣️🗣️🗣️🗣️🗣️🗣️🗣️🗣️\n\n\n\n"
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

printf "\n\n\n🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥 🍳🍳🥩🥩 STARTED COOKIN' 🥩🥩🍳🍳 🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥\n\n\n\n"

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

printf "############## CUSTOMER ##############\n\n"

set_current_root src/customer/managers

make_folder add-card/src
make_folder add-customer/src
make_folder add-address/src
make_folder add-purchase/src
make_folder add-rating/src
make_folder add-refund-request/src
make_folder delete-purchase/src
make_folder search-products/src

printf "############## SUPPLIER ##############\n\n"

set_current_root src/supplier/managers

make_folder add-supplier/src
make_folder add-restock/src
make_folder add-product/src
make_folder update-product/src

printf "\n\n\n🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥 🍳🍳🥩🥩 FINISHED COOKIN' 🥩🥩🍳🍳 🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥\n\n\n\n"
