# docker build -t vrstunnel .
# docker run --rm -it --entrypoint bash vrstunnel

ARG DISTRO=debian
ARG IMAGE_VERSION=bullseye
ARG IMAGE_VARIANT=slim
FROM $DISTRO:$IMAGE_VERSION-$IMAGE_VARIANT AS VRStunnel

LABEL maintainer="Julien Ancelin<julien.ancelin@inrae.fr>"

RUN apt-get -qq update --fix-missing && apt-get -qq --yes upgrade

RUN set -eux \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get update \
    && apt-get -y install \
        gcc cmake  libpq-dev git build-essential libgtest-dev cpputest lcov xsltproc libaio-dev libc6-dev pv
RUN ls -la ./
RUN git clone https://github.com/jancelin/VrsTunnel.git
WORKDIR ./VrsTunnel/Src
RUN cmake .
RUN make VERBOSE=1
#RUN make install

# Cleanup resources
#RUN apt-get -y --purge autoremove  \
#    && apt-get clean \
#    && rm -rf /var/lib/apt/lists/*

# start env parameters
COPY docker-entrypoint.sh .
RUN chmod +x ./docker-entrypoint.sh

ENTRYPOINT ./docker-entrypoint.sh
