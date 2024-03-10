#!/bin/bash

check_last_command() {
    if [ $? -ne 0 ]
    then
        printf "\n\n\n ✨✨✨✨✨✨✨✨✨✨ IT FAILED :((( ✨✨✨✨✨✨✨✨✨✨ \n"
        exit
    fi
}

try_sh() {
    sh $1

    check_last_command
}

try_cd() {
    cd $1

    check_last_command
}

try_cd db-scripts
try_sh create.sh
try_cd ..
try_sh makes.sh --clean
try_sh makes.sh
try_sh run.sh
