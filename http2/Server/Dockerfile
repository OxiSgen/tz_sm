FROM ubuntu:20.04


RUN apt-get -y update && apt-get install -y


RUN apt-get -y install software-properties-common


RUN add-apt-repository ppa:pistache+team/unstable


RUN apt-get -y update
RUN apt-get -y install g++ cmake pkg-config libpistache-dev

# boost
RUN apt-get install wget -y
RUN cd . && wget http://downloads.sourceforge.net/project/boost/boost/1.78.0/boost_1_78_0.tar.gz \
  && tar xfz boost_1_78_0.tar.gz \
  && rm boost_1_78_0.tar.gz \
  && cd boost_1_78_0 \
  && ./bootstrap.sh --prefix=/usr/local --with-libraries=program_options \
  && ./b2 install \
  && cd . \
  && rm -rf boost_1_78_0


COPY . .

# Рабочая дериктория
WORKDIR .


RUN mkdir build
RUN cmake -B/build -S/
RUN cmake --build /build

COPY file_student.txt .
COPY file_student_2.txt .

EXPOSE 9080

CMD ./build/httpserver_run
