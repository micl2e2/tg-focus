FROM debian:12 AS builder

ARG TARGETARCH
ARG TARGETOS

RUN mkdir /opt/tg-focus
WORKDIR /opt/tg-focus

RUN apt-get update && \
    apt-get install -y wget tar && \
    rm -rf /var/lib/apt/lists/*

RUN case ${TARGETARCH} in \
    "amd64") ARCH="x86_64" ;; \
    "arm64") ARCH="aarch64" ;; \
    *) echo "Unsupported architecture: ${TARGETARCH}" && exit 1 ;; \
    esac && \
    wget https://github.com/micl2e2/tg-focus/releases/latest/download/tg-focus-3.0-linux-${ARCH}.tar.gz && \
    tar xvf tg-focus-3.0-linux-${ARCH}.tar.gz --strip-components=1


FROM debian:12

ARG TARGETARCH
ARG TARGETOS
COPY --from=builder /opt/tg-focus/tgfocus /usr/local/bin/tgfocus

ENTRYPOINT ["/usr/local/bin/tgfocus"]