CFLAGS = -Wall -g -std=c++11 -Wextra

client: client.cpp buffer.cpp connection.cpp message.cpp
	g++ $(CFLAGS) client.cpp buffer.cpp connection.cpp message.cpp -o client

.PHONY: clean client

run_client:
	./client

clean:
	rm -f client