FROM gcc:latest
RUN apt-get -y update && apt-get -y install libboost-all-dev autoconf automake autopoint bash bison bzip2 flex g++ g++-multilib gettext git gperf intltool libc6-dev-i386 libgdk-pixbuf2.0-dev libltdl-dev libssl-dev libtool-bin libxml-parser-perl lzip make openssl p7zip-full patch perl pkg-config python ruby sed unzip wget xz-utils
WORKDIR /usr/src/tfg