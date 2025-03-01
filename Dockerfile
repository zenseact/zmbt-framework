FROM ubuntu:latest

USER root

RUN apt-get update && \
    apt-get install -y \
    gcc make cmake git gdb \
    doxygen graphviz \
    git cmake python3.12 python3-pip python-is-python3 \
    unzip zip wget xz-utils lsb-release \
    software-properties-common gnupg \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Install LLVM+Clang
RUN wget https://apt.llvm.org/llvm.sh
RUN chmod u+x llvm.sh
RUN ./llvm.sh 17 all
RUN clang-17 --version

# Install Bazel
RUN wget https://github.com/bazelbuild/bazel/releases/download/7.4.1/bazel-7.4.1-installer-linux-x86_64.sh
RUN chmod +x bazel-7.4.1-installer-linux-x86_64.sh
RUN ./bazel-7.4.1-installer-linux-x86_64.sh
RUN bazel --version

ENV PATH="/usr/lib/llvm-17/bin:${PATH}"

USER ubuntu

ENV PATH="/home/ubuntu/.local/bin:${PATH}"

ADD requirements.txt /tmp/requirements.txt
# RUN pip3 install --upgrade pip --break-system-packages
RUN pip3 install -r /tmp/requirements.txt --break-system-packages



CMD ["bash"]
