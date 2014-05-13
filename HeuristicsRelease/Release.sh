#!/bin/bash
IFS='%'

destdir=../../../Conferences\ \&\ Workshops/2013\ \[A\]\ ICCABS\ \(Heuristics\)/heuristics\ \(source\ code\)/

printf ${destdir}

printf "\n"
echo ...Directory contents...
ls -al

printf "\n"
echo ...Initial destination contents...
ls -al ${destdir}

printf "\n"
echo ...Copying to DropBox folder...
rm -f ${destdir}*
filepatterns[0]="*.cpp"
filepatterns[1]="*.hpp"
filepatterns[2]="*.h"
filepatterns[3]="ReadMe"
filepatterns[4]="Makefile"
for filepattern in "${filepatterns[@]}"
do
	printf "copying ${filepattern} to ${destdir} \n"
	cp ${filepattern} ${destdir}
done

printf "\n"
echo ...Final destination contents...
ls -al ${destdir}
