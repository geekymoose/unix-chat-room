CC			= gcc
CF_FLAG		= -Wall -g
LIBS_FLAG	= -pthread
VPATH		= src src/wunixlib
BIN			= bin

WUNIXLIB_OBJ= sighandler.o stream.o network.o assets.o linkedlist.o


# ------------------------------------------------------------------------------
# General rules
# ------------------------------------------------------------------------------
.PHONY: all
all: server.exe client.exe


server.exe: server.o helper.o messaging.o $(WUNIXLIB_OBJ) server_data.o messaging_server.o user.o room.o
	$(CC) $(CF_FLAGS) $(LIBS_FLAG) -o $@ $^
client.exe: client.o helper.o messaging.o $(WUNIXLIB_OBJ) client_data.o commands.o messaging_client.o
	$(CC) $(CF_FLAGS) $(LIBS_FLAG) -o $@ $^


# ------------------------------------------------------------------------------
# project compilation
# ------------------------------------------------------------------------------
client.o: client.c
	$(CC) $(CF_FLAGS) $(LIBS_FLAG) $< -c
client_data.o: client_data.c
	$(CC) $(CF_FLAGS) $(LIBS_FLAG) $< -c
commands.o: commands.c
	$(CC) $(CF_FLAGS) $(LIBS_FLAG) $< -c
helper.o: helper.c
	$(CC) $(CF_FLAGS) $(LIBS_FLAG) $< -c
messaging.o: messaging.c
	$(CC) $(CF_FLAGS) $(LIBS_FLAG) $< -c
messaging_server.o: messaging_server.c
	$(CC) $(CF_FLAGS) $(LIBS_FLAG) $< -c
messaging_client.o: messaging_client.c
	$(CC) $(CF_FLAGS) $(LIBS_FLAG) $< -c
room.o: room.c
	$(CC) $(CF_FLAGS) $(LIBS_FLAG) $< -c
server.o: server.c
	$(CC) $(CF_FLAGS) $(LIBS_FLAG) $< -c
server_data.o: server_data.c
	$(CC) $(CF_FLAGS) $(LIBS_FLAG) $< -c
user.o: user.c
	$(CC) $(CF_FLAGS) $(LIBS_FLAG) $< -c


# ------------------------------------------------------------------------------
# wunixlib compilation
# ------------------------------------------------------------------------------
sighandler.o: sighandler.c sighandler.h
	$(CC) $(CF_FLAGS) $< -c
stream.o: stream.c stream.h
	$(CC) $(CF_FLAGS) $< -c
network.o: network.c network.h
	$(CC) $(CF_FLAGS) $< -c
assets.o: assets.c assets.h
	$(CC) $(CF_FLAGS) $< -c
linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CF_FLAGS) $< -c


# ------------------------------------------------------------------------------
# Asset functions
# ------------------------------------------------------------------------------
.PHONY: clean
clean:
	-rm -rf $(BIN)
	-rm *.o
	-rm *.out
	-rm *.exe
