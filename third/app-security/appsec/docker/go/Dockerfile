# build frontend
FROM node:16
WORKDIR /node/src/app
COPY .env .
COPY ./frontend ./frontend
WORKDIR /node/src/app/frontend
RUN yarn 
RUN yarn build

# build backend
FROM golang
WORKDIR /go/src/app
COPY ./ ./
RUN go get -d -v
RUN go build -v

# move statically generated frontend to be served by the backend
COPY --from=0 /node/src/app/frontend/dist ./frontend/dist

# start the backend
CMD ["./myApp"]