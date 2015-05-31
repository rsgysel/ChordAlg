#!/bin/bash

FILES="*.m"

if [ $# -ne 1 ]
    then
        echo "Usage: $0 <sanity_check_executable>"
        exit
fi

for f in $FILES
do
    echo "========================================="
    echo "Testing file $f..."
    $1 $f
    echo "========================================="
    echo ""
done
