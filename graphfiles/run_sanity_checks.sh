#!/bin/bash

FILES="*.m
*.g"

if [ $# -ne 1 ]
    then
        echo "Usage: $0 <sanity_check_executable>"
        exit
fi

for f in $FILES
do
    echo "Testing file $f..."
    $1 $f
done
