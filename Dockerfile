# Simple & minimalistic
# Dockerfile for the
# "Vine" text editor.

FROM gcc:latest

COPY . /vine

WORKDIR /vine/

RUN gcc -o vine src/vine.c

CMD [“./vine”]
