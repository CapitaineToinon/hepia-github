#!/usr/bin/env sh
set -eu

# set nginx variables to match the ones in .env
envsubst '${API_HOST} ${API_PORT}' < /etc/nginx/conf.d/default.conf.template > /etc/nginx/conf.d/default.conf

exec "$@"