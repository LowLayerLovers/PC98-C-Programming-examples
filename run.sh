#!/bin/bash

IMG=gcc-ia16:1

echo "Container Name:"
read NAME

sudo docker run \
  --rm -it -d \
  -v ${HOME}:${HOME} \
  -e "HOME=${HOME}" \
  --workdir=`pwd` \
  --name $NAME $IMG /bin/bash

sudo docker exec -it $NAME bash