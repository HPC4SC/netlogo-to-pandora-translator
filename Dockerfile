FROM gcc:4.9
RUN apt-get -y update && apt-get -y install libboost-all-dev 
COPY ./src/ /usr/src/tfg
WORKDIR /usr/src/tfg
RUN g++ -std=c++14 -o main main.cpp