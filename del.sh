#!/bin/bash

Sharedfile="~\/":

echo "Remove container!!"

echo "Container Name:"
read NAME

sudo docker stop $NAME
sudo docker rm $NAME