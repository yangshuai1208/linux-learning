CC = gcc
TARGET = gateway_module
OBJS = main.o sensor_parser.o log_writer.o mqtt_client.o
LIBS = -lmosquitto

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

main.o: main.c sensor_parser.h log_writer.h mqtt_client.h
	$(CC) -c main.c

sensor_parser.o: sensor_parser.c sensor_parser.h
	$(CC) -c sensor_parser.c

log_writer.o: log_writer.c log_writer.h
	$(CC) -c log_writer.c

mqtt_client.o: mqtt_client.c mqtt_client.h
	$(CC) -c mqtt_client.c

clean:
	rm -f $(OBJS) $(TARGET)
