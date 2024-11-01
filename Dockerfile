FROM gcc:latest

COPY . /vine
WORKDIR /vine/
RUN gcc -o vine dockerworld.c

CMD ["./vine"]
