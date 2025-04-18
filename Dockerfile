FROM ubuntu:22.04

# 必要なパッケージをまとめてインストール
RUN apt update && apt install -y \
    curl \
    git \
    unzip \
    gcc \
    g++ \
    libusb-1.0-0-dev \
    python3 \
    python3-pip \
    python3-venv \
    vim \
    build-essential

# PlatformIOのインストール
RUN pip3 install platformio

# 作業ディレクトリ（適宜プロジェクトパスに）
WORKDIR /workspace

CMD ["bash"]
