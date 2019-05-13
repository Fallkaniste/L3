#!/bin/bash

gnome-terminal -- ./serveur.out

for ((i = 1;i <= 4; i ++))
do
  gnome-terminal -- "./client.out"
done
