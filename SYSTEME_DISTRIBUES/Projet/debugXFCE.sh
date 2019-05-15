#!/bin/bash

xfce4-terminal -e ./serveur.out

for ((i = 1;i <= 4; i ++))
do
  xfce4-terminal -e ./client.out
done
