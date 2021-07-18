FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    # apt-get upgrade && \
    apt-get -y install sudo git make software-properties-common
    
RUN add-apt-repository ppa:tkchia/build-ia16 && \
    apt-get update
RUN apt-get -y install gcc-ia16-elf libi86-ia16-elf