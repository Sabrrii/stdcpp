# A Dockerfile is a simple text-file that contains a list of commands
# that the Docker client calls while creating an image.
# It's a simple way to automate the image creation process.
# The best part is that the commands you write in a Dockerfile are almost
# identical to their equivalent Linux commands. This means you don't really
# have to learn new syntax to create your own dockerfiles.
# Use debian as base image, it will be downloaded automatically

FROM debian:latest
MAINTAINER COUDERT Sebastien "sebastien.coudert@ganil.fr"

# Update and install
RUN apt-get update && apt-get install -y g++ build-essential  doxygen graphviz git

# Working directory
#RUN pwd
RUN mkdir -p /home/user
WORKDIR /home/user
RUN ls -lah /home

# Build binary
ADD Makefile hello.cpp ./
#RUN ls -lah && g++ ArgParse.cpp -o ArgParseG++
RUN make && ./ArgParse && ls -lah

# Build documentation, convert and push to wiki
ADD Doxyfile ./
RUN doxygen && ls -lah doc/html
#RUN git add 
#RUN git push

# File check
RUN du -shc doc/html 
RUN ls -lah

