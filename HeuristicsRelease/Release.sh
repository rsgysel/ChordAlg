#!/bin/bash

printf "\n"
echo ...Directory contents...
ls -al

printf "\n"
echo ...Initial destination contents...
ls -al ../../../Conferences\ \&\ Workshops/2013\ ICCABS\ \(Heuristics\)/heuristics\ \(source\ code\)/

printf "\n"
echo ...Copying to DropBox folder...
rm -f ../../../Conferences\ \&\ Workshops/2013\ ICCABS\ \(Heuristics\)/heuristics\ \(source\ code\)/*
cp * ../../../Conferences\ \&\ Workshops/2013\ ICCABS\ \(Heuristics\)/heuristics\ \(source\ code\)/

printf "\n"
echo ...Final destination contents...
ls -al ../../../Conferences\ \&\ Workshops/2013\ ICCABS\ \(Heuristics\)/heuristics\ \(source\ code\)/
