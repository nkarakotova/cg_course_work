FROM ubuntu:latest

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update

RUN apt-get install -y libgtest-dev cmake qtbase5-dev qt5-qmake build-essential ffmpeg python3-matplotlib cppcheck doxygen

RUN cd /usr/src/gtest && cmake CMakeLists.txt && make && cp /usr/src/gtest/lib/*.a /usr/lib

ENV QT_QPA_PLATFORM=offscreen
