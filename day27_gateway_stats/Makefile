CC = gcc
TARGET = gateway_stats
OBJS = main.o data_source.o sensor_parser.o log_writer.o mqtt_client.o app_log.o gateway_stats.o
LIBS = -lmosquitto

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

main.o: main.c config.h data_source.h sensor_parser.h log_writer.h mqtt_client.h app_log.h gateway_stats.h
	$(CC) -c main.c

data_source.o: data_source.c data_source.h config.h
	$(CC) -c data_source.c

sensor_parser.o: sensor_parser.c sensor_parser.h
	$(CC) -c sensor_parser.c

log_writer.o: log_writer.c log_writer.h
	$(CC) -c log_writer.c

mqtt_client.o: mqtt_client.c mqtt_client.h
	$(CC) -c mqtt_client.c

app_log.o: app_log.c app_log.h
	$(CC) -c app_log.c

gateway_stats.o: gateway_stats.c gateway_stats.h
	$(CC) -c gateway_stats.c

clean:
	rm -f $(OBJS) $(TARGET)
