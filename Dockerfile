# A Dockerfile is a simple text-file that contains a list of commands
# that the Docker client calls while creating an image.
# It's a simple way to automate the image creation process.
# The best part is that the commands you write in a Dockerfile are almost
# identical to their equivalent Linux commands. This means you don't really
# have to learn new syntax to create your own dockerfiles.
# Use ubuntu as base image, it will be downloaded automatically

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
ADD Makefile main.cpp ./
#RUN ls -lah && g++ hello.cpp -o helloG++
RUN make && ./hello.C++ && ls -lah

# Build documentation, convert and push to wiki
ADD Doxyfile ./
RUN doxygen && ls -lah doc/html
## fail: no .git ##
##RUN git branch 
###push to current repo. (instead of wiki)
##RUN git add doc
##RUN git commit -am 'add doc'
##RUN git push

# File check
RUN pwd
RUN du -shc doc/html 
RUN ls -lah && ./hello.C++
