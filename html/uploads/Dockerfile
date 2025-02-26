FROM debian:latest

RUN apt-get update && apt-get install -y \
    php-cgi \
    php-common \
    php-fpm \
    php-redis \
    php-mbstring \
    php-mysql \
    php-zip

COPY ./setup.sh .

CMD ["sh", "setup.sh"]