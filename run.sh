#!/bin/bash

ROOT=$(pwd)
CURRENT_ROOT=""

check_arguments() {
    if [ "$1" = "--kill" ] || [ "$1" = "-k" ]
    then
        pkill main
        exit
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

try_main() {
    ./main &

    check_last_command
}

run_main() {
    try_cd $1

    printf "INSIDE $1\n"

    try_main

    printf "DONE $1\n\n"

    try_cd "$ROOT/$CURRENT_ROOT"
}

check_arguments $1

printf "\n\n\n########## Started run script ##########\n\n\n\n"

pkill main

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

printf "\n\n\n########## Run script completed ###########\n\n\n\n"
