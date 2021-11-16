#get a base image
FROM golang
WORKDIR /go/src/app
COPY ./ ./
RUN go get -d -v
RUN go build -v
CMD ["./myApp"]