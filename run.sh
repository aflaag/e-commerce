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

run_main() {
    try_cd $1

    printf "INSIDE $1\n"

    ./main &

    printf "DONE $1\n\n"

    try_cd "$ROOT/$CURRENT_ROOT"
}

check_arguments $1

printf "\n\n\n🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥 🍳🍳🥩🥩 HOLD ON, LEMME COOK. 🥩🥩🍳🍳 🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥\n\n\n\n"


printf "############## NON FUNCTIONAL REQ ##############\n\n"

set_current_root src

run_main non-functional-req/bin

printf "############## CUSTOMER ##############\n\n"

set_current_root src/customer

run_main handler/bin

run_main managers/add-address/bin
run_main managers/add-card/bin
run_main managers/add-customer/bin
run_main managers/add-purchase/bin
run_main managers/add-rating/bin
run_main managers/add-refund-request/bin
run_main managers/delete-purchase/bin
run_main managers/search-products/bin
run_main managers/view-order/bin
run_main managers/view-refund/bin

printf "############## SUPPLIER ##############\n\n"

set_current_root src/supplier

run_main handler/bin

run_main managers/add-product/bin
run_main managers/add-restock/bin
run_main managers/add-supplier/bin
run_main managers/update-product/bin
run_main managers/view-statistic/bin

printf "############## COURIER ##############\n\n"

set_current_root src/courier

run_main handler/bin

run_main managers/take-purchase-delivery/bin
run_main managers/update-assigned-delivery/bin
run_main managers/update-refund-request/bin
run_main managers/view-available-purchase/bin

printf "\n\n\n🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥 🍳🍳🥩🥩 FINISHED COOKIN' 🥩🥩🍳🍳 🔥🔥🔥🔥🔥🔥🔥🔥🔥🔥\n\n\n\n"

