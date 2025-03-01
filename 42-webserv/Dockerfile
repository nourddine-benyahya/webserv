FROM alpine

# Install the required packages
RUN apk add --no-cache \
    g++ \
    gcc \
    make
RUN apk add --no-cache \
    python3 \
    python3-dev 
RUN apk add --no-cache \
    php \
    php-cgi \
    fcgi 

RUN mkdir -p /usr/local/42-webserv

WORKDIR /usr/local/42-webserv

EXPOSE 80

CMD ["run"]

ENTRYPOINT [ "make" ]