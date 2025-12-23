FROM ubuntu:20.04

USER root


RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get update && \
    apt-get install -y \
    gcc make cmake git gdb \
    doxygen graphviz \
    git cmake python3.12 python3-pip python-is-python3 \
    unzip zip curl wget xz-utils lsb-release \
    software-properties-common gnupg tzdata \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN ln -fs /usr/share/zoneinfo/America/New_York /etc/localtime && \
    dpkg-reconfigure --frontend noninteractive tzdata

# Install LLVM+Clang
RUN wget https://apt.llvm.org/llvm.sh
RUN chmod u+x llvm.sh
RUN ./llvm.sh 17 all
RUN clang-17 --version

ENV PATH="/usr/lib/llvm-17/bin:${PATH}"

# Install Bazel
RUN wget https://github.com/bazelbuild/bazel/releases/download/8.4.2/bazel-8.4.2-installer-linux-x86_64.sh
RUN chmod +x bazel-8.4.2-installer-linux-x86_64.sh
RUN ./bazel-8.4.2-installer-linux-x86_64.sh
RUN bazel --version

# Python 3.12 deps
RUN apt-get install -y zlib1g zlib1g-dev libssl-dev libbz2-dev libsqlite3-dev liblzma-dev libreadline-dev

RUN useradd -m -s /bin/bash ubuntu && echo "ubuntu ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
USER ubuntu

# Install Pyenv, Python 3.12, and packages
RUN curl https://pyenv.run | bash
RUN echo 'export PYENV_ROOT="$HOME/.pyenv"' >> ~/.bashrc
RUN echo '[[ -d $PYENV_ROOT/bin ]] && export PATH="$PYENV_ROOT/bin:$PATH"' >> ~/.bashrc
RUN echo 'eval "$(pyenv init - bash)"' >> ~/.bashrc

RUN $HOME/.pyenv/bin/pyenv install -s 3.12
RUN $HOME/.pyenv/bin/pyenv global 3.12
ADD requirements.txt /tmp/requirements.txt
RUN $($HOME/.pyenv/bin/pyenv which pip) install --upgrade pip
RUN $($HOME/.pyenv/bin/pyenv which pip) install -r /tmp/requirements.txt

CMD ["bash"]
