all:server client

server:server.c
	gcc $< -o $@ -l sqlite3

client:client.c
	gcc $< -o $@
	
clean:
	rm server client	