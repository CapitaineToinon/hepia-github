FROM ubuntu:20.04

RUN apt update -y
RUN apt install software-properties-common iptables -y
RUN add-apt-repository ppa:deadsnakes/ppa -y
RUN apt update -y
RUN apt install python3.8 -y