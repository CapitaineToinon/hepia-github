Setup for MACOS

```
sudo ifconfig lo0 alias 127.0.0.* up
```

# How to setup

- Copy `.env.example` to `.env`
- Ensure you're logged in to your google cloud storage account
- set the `GOOGLE_PROJECT` env variable to match your google project name

# Commands

## main

```
Usage:
  main [OPTIONS] <client | server>

Help Options:
  -h, --help  Show this help message

Available commands:
  client
  server
```

## server

```
Usage:
  main [OPTIONS] server [server-OPTIONS]

Help Options:
  -h, --help        Show this help message

[server command options]
          --config= config file to load
          --port=   the port to use (default: 3000)
```

## client

```
Usage:
  main [OPTIONS] client <command>

Help Options:
  -h, --help      Show this help message

Available commands:
  create
  fake
  get
  vote
```

## client create

```
Usage:
  main [OPTIONS] client create [create-OPTIONS]

Help Options:
  -h, --help          Show this help message

[create command options]
          --ip=       the ip to send the message to
          --port=     the port to use (default: 3000)
          --sender=
          --receiver=
          --amount=
```

## client vote

```
Usage:
  main [OPTIONS] client vote [vote-OPTIONS]

Help Options:
  -h, --help          Show this help message

[vote command options]
          --ip=       the ip to send the message to
          --port=     the port to use (default: 3000)
          --id=
          --sender=
          --receiver=
          --amount=
```

## client fake

```
Usage:
  main [OPTIONS] client fake [fake-OPTIONS]

Help Options:
  -h, --help          Show this help message

[fake command options]
          --ip=       the ip to send the message to
          --port=     the port to use (default: 3000)
          --id=
          --sender=
          --receiver=
          --amount=
```

## client get

```
Usage:
  main [OPTIONS] client get [get-OPTIONS]

Help Options:
  -h, --help      Show this help message

[get command options]
          --ip=   the ip to send the message to
          --port= the port to use (default: 3000)
```
