# Image officielle GCC, version figée (pas de tag « latest »).
# GCC 16.2.0 sur Debian 13 « trixie ».
FROM gcc:16.2.0-trixie

ENV LANG=C.UTF-8 \
    LC_ALL=C.UTF-8 \
    DEBIAN_FRONTEND=noninteractive

# L'image gcc fournit gcc/g++/make mais pas de débogueur.
RUN apt-get update \
 && apt-get install -y --no-install-recommends \
        gdb \
        valgrind \
        git \
        less \
        nano \
        ca-certificates \
 && rm -rf /var/lib/apt/lists/*

# Autorise gdb à désactiver l'ASLR dans le conteneur (sinon warning au démarrage).
RUN printf 'set auto-load safe-path /\n' > /root/.gdbinit

WORKDIR /workspaces/c-lang
