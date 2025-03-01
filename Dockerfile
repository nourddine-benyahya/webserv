FROM alpine

# Install the required packages
RUN apk add --no-cache \
    g++ \
    gcc \
    make \
    python3 \
    python3-dev \
    php \
    php-cgi \
    fcgi 

WORKDIR /usr/local/42-webserv

EXPOSE 80

CMD ["./webserv"]

ENTRYPOINT [ "make" ]