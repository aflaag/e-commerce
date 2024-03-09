#!/bin/bash

cd db-scripts

sh create.sh

cd ..

sh makes.sh --clean
sh makes.sh
sh run.sh
