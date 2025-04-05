FROM gcc:13

WORKDIR /app
COPY . .

RUN apt update && apt install -y make git
RUN make

CMD ["./bin/discordtool"]