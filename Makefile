CC = gcc
CFLAGS = -g -pedantic -Wall -Werror -I ./inc/
OBJS = $(OBJ)ClientApp.o $(OBJ)ChatManager.o $(OBJ)ClientNet.o $(OBJ)UI.o $(OBJ)HashMap.o $(OBJ)list.o $(OBJ)ServerApp.o $(OBJ)GroupDB.o $(OBJ)UserDB.o $(OBJ)Protocol.o $(OBJ)User.o $(OBJ)Group.o $(OBJ)multicast.o $(OBJ)ServerRequestHandler.o

CLI = ./Client/
CHT = ./Chat/
SER = ./Server/
DTS = ./Data_Structures/
OBJ = ./obj/
PRO = ./Protocol/
TCP = ./TCP/
INC = ./inc/

#EXECUTABLES
ui: $(CLI)ClientApp.c $(CHT)ChatManager.c $(TCP)ClientNet.c $(PRO)Protocol.c $(CLI)UI.c $(DTS)HashMap.c $(DTS)list.c $(DTS)list_functions.c $(DTS)list_itr.c
	$(CC) $(CFLAGS) -lm $(CLI)ClientApp.c $(CHT)ChatManager.c $(TCP)ClientNet.c $(PRO)Protocol.c $(CLI)UI.c $(DTS)HashMap.c $(DTS)list.c $(DTS)list_functions.c $(DTS)list_itr.c  -o ui -lm

serve : $(TCP)server.c $(SER)ServerApp.c $(SER)GroupDB.c $(SER)UserDB.c $(PRO)Protocol.c $(SER)User.c $(SER)Group.c $(DTS)HashMap.c $(DTS)list.c $(DTS)list_functions.c $(DTS)list_itr.c
	$(CC) $(CFLAGS) -lm $(TCP)server.c $(SER)ServerApp.c $(SER)GroupDB.c $(SER)UserDB.c $(PRO)Protocol.c $(SER)User.c $(SER)Group.c $(DTS)HashMap.c $(DTS)list.c $(DTS)list_functions.c $(DTS)list_itr.c $(SER)ServerRequestHandler.c -o ./serve -lm

messanger : $(CHT)multicast.c
	$(CC) $(CFLAGS) -lm $(CHT)multicast.c -o ./messanger

#OBJECTS
$(OBJ)ClientApp.o : $(CLI)ClientApp.c $(INC)ClientApp.h
	$(CC) $(CFLAGS) -c -o $(OBJ)ClientApp.o $(CLI)ClientApp.c

$(OBJ)ChatManager.o : $(CHT)ChatManager.c $(INC)ChatManager.h
	$(CC) $(CFLAGS) -c -o $(OBJ)ChatManager.o $(CHT)ChatManager.c

$(OBJ)ClientNet.o : $(TCP)ClientNet.c $(INC)ClientNet.h
	$(CC) $(CFLAGS) -c -o $(OBJ)ClientNet.o $(TCP)ClientNet.c

$(OBJ)server.o : $(TCP)server.c $(INC)server.h
	$(CC) $(CFLAGS) -c -o $(OBJ)server.o $(TCP)server.c

$(OBJ)UI.o : $(CLI)UI.c $(INC)UI.h
	$(CC) $(CFLAGS) -c -o $(OBJ)UI.o $(CLI)UI.c

$(OBJ)HashMap.o : $(DTS)HashMap.c $(INC)HashMap.h
	$(CC) $(CFLAGS) -c -o $(OBJ)HashMap.o $(DTS)HashMap.c

$(OBJ)list.o : $(DTS)list.c $(INC)list.h
	$(CC) $(CFLAGS) -c -o $(OBJ)list.o $(DTS)list.c

$(OBJ)ServerApp.o : $(SER)ServerApp.c $(INC)ServerApp.h
	$(CC) $(CFLAGS) -c -o $(OBJ)ServerApp.o $(SER)ServerApp.c

$(OBJ)ServerRequestHandler.o : $(SER)ServerRequestHandler.c $(INC)ServerRequestHandler.h
	$(CC) $(CFLAGS) -c -o $(OBJ)ServerRequestHandler.o $(SER)ServerRequestHandler.c

$(OBJ)GroupDB.o : $(SER)GroupDB.c $(INC)GroupDB.h
	$(CC) $(CFLAGS) -c -o $(OBJ)GroupDB.o $(SER)GroupDB.c

$(OBJ)UserDB.o : $(SER)UserDB.c $(INC)UserDB.h
	$(CC) $(CFLAGS) -c -o $(OBJ)UserDB.o $(SER)UserDB.c

$(OBJ)Protocol.o : $(PRO)Protocol.c $(INC)Protocol.h
	$(CC) $(CFLAGS) -c -o $(OBJ)Protocol.o $(PRO)Protocol.c

$(OBJ)User.o : $(SER)User.c $(INC)User.h
	$(CC) $(CFLAGS) -c -o $(OBJ)User.o $(SER)User.c

$(OBJ)Group.o : $(SER)Group.c $(INC)Group.h
	$(CC) $(CFLAGS) -c -o $(OBJ)Group.o $(INC)Group.c

$(OBJ)multicast.o : $(CHT)multicast.c
	$(CC) $(CFLAGS) -c -o $(OBJ)multicast.o $(INC)multicast.c

all :
	make clean
	make $(OBJS)
	make messanger
	make ui
	make serve

clean:
	rm -f *.o
	rm -f ui
	rm -f serve
	rm -f messanger
