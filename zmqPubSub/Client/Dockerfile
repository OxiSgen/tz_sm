FROM ubuntu:20.04

# ZMQ

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y git build-essential libtool autoconf automake pkg-config unzip libkrb5-dev
RUN cd /tmp && git clone https://github.com/jedisct1/libsodium.git && cd libsodium && git checkout e2a30a && ./autogen.sh && ./configure && make check && make install && ldconfig
RUN cd /tmp && git clone --depth 1 --branch v4.3.4 https://github.com/zeromq/libzmq.git && cd libzmq && ./autogen.sh && ./configure && make
# RUN cd /tmp/libzmq && make check
RUN cd /tmp/libzmq && make install && ldconfig
RUN rm /tmp/* -rf

# libssl (требуется Cmake)

RUN apt-get install libssl-dev

# CMake

RUN apt-get install wget -y
RUN wget https://cmake.org/files/v3.23/cmake-3.23.0.tar.gz -P /tmp/
RUN tar xzf /tmp/cmake-3.23.0.tar.gz -C /tmp/
RUN cd /tmp/cmake-3.23.0 && ./configure --prefix=/opt/cmake && make -j4 && make install
RUN /opt/cmake/bin/cmake -version
RUN rm /tmp/* -rf

# boost

RUN cd /tmp && wget http://downloads.sourceforge.net/project/boost/boost/1.78.0/boost_1_78_0.tar.gz \
  && tar xfz boost_1_78_0.tar.gz \
  && rm boost_1_78_0.tar.gz \
  && cd boost_1_78_0 \
  && ./bootstrap.sh --prefix=/usr/local --with-libraries=program_options \
  && ./b2 install \
  && cd /tmp \
  && rm -rf boost_1_78_0

# cppzqm (hpp привязка для libZMQ)
RUN cd /tmp && git clone --depth 1 --branch v4.8.1 https://github.com/zeromq/cppzmq.git && cp cppzmq/zmq.hpp /usr/local/include/zmq.hpp

# Установка приложения

RUN mkdir -p /opt/ps
WORKDIR /opt/ps

COPY . /opt/ps

RUN /opt/cmake/bin/cmake .
RUN make

EXPOSE 5555

CMD ./zmq_ps