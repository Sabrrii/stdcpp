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
RUN apt-get update && apt-get install -y doxygen graphviz g++ build-essential

# Build binary
RUN make