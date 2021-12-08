FROM nginx:1.15-alpine
COPY ./docker/nginx/nginx-default.conf.template /etc/nginx/conf.d/default.conf.template
COPY ./docker/nginx/docker-entrypoint.sh /
COPY ./docker/nginx/certs /certs
ENTRYPOINT ["/docker-entrypoint.sh"]
CMD ["nginx-debug", "-g", "daemon off;"]