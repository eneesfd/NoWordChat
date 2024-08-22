# Use Ubuntu 20.04 as the base image
FROM ubuntu:20.04

# Update the package lists
RUN apt-get update

# set timezone for istanbul
RUN ln -fs /usr/share/zoneinfo/Europe/Istanbul /etc/localtime
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y tzdata

# Install necessary tools
RUN apt-get install -y sudo

# Install necessary dependencies
RUN apt-get install -y build-essential cmake libboost-all-dev libjsoncpp-dev libssl-dev git wget unzip pkg-config gcc g++ uuid-dev zlib1g-dev openssl libssl-dev

# Engineer adlı kullanıcıyı oluştur ve şifresini ayarla
RUN useradd -ms /bin/bash engineer && echo "engineer:123123" | chpasswd

# Engineer kullanıcısını sudoers dosyasına ekle (root yetkisi ile)
RUN echo "engineer ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

# Engineer kullanıcısını sudo grubuna ekle
RUN usermod -aG sudo engineer

# Engineer kullanıcısını varsayılan kullanıcı olarak ayarla
USER engineer

WORKDIR /home/engineer

# Install Drogon
RUN git clone https://github.com/drogonframework/drogon
WORKDIR /home/engineer/drogon
RUN git submodule update --init
RUN mkdir build
WORKDIR /home/engineer/drogon/build
RUN cmake ..
RUN make -j 
RUN sudo make install

# Expose the port your application listens on
EXPOSE 8080

# Set the working directory
WORKDIR /home/engineer

COPY . .