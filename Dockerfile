FROM ubuntu:20.04
COPY . /root/kbang
ENV TZ=Europe/Prague
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt update && apt install -y build-essential qt5-default qtmultimedia5-dev
RUN mkdir -p /root/build && cd /root/build && qmake /root/kbang/kbang.pro && make -j 4
EXPOSE 6543
CMD /root/build/kbang-server
