
run :
	docker-compose up -d

stop :
	docker-compose down
	
rmi :
	docker rmi -f webserv

logs :
	docker logs webserv_c

fclean :
	make fclean -C 42-webserv