FROM gcc:latest
RUN apt-get -y update && apt-get -y install libboost-all-dev
WORKDIR /usr/src/tfg