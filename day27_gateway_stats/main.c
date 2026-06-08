#include <stdio.h>
#include <unistd.h>
#include <mosquitto.h>

#include "config.h"
#include "data_source.h"
#include "sensor_parser.h"
#include "log_writer.h"
#include "mqtt_client.h"
#include "app_log.h"
#include "gateway_stats.h"

int main(void)
{
    int i;
    int ret;

    char raw_buf[128];
    char json_buf[128];

    SensorData sensor;
    GatewayStats stats;

    struct mosquitto *mosq = NULL;

    gateway_stats_init(&stats);

    printf("gateway stats start\n");
    app_log_info("gateway stats start");

    ret = data_source_init();
    if (ret != 0)
    {
        printf("data source init failed\n");
        app_log_error("data source init failed");
        return 1;
    }

    app_log_info("data source init success");

    ret = mqtt_client_init(&mosq);
    if (ret != 0)
    {
        printf("mqtt client init failed\n");
        app_log_error("mqtt client init failed");
        data_source_cleanup();
        return 1;
    }

    app_log_info("mqtt client init success");

    for (i = 0; i < LOOP_COUNT; i++)
    {
        printf("------------------------------\n");

        ret = data_source_read(raw_buf, sizeof(raw_buf));
        if (ret != 0)
        {
            printf("read data source failed\n");
            app_log_warn("read data source failed");
            continue;
        }

        stats.total_count++;

        printf("raw data: %s\n", raw_buf);

        ret = parse_sensor_data(raw_buf, &sensor);
        if (ret != 0)
        {
            stats.parse_fail_count++;

            printf("parse sensor data failed\n");
            app_log_warn("parse sensor data failed");
            continue;
        }

        stats.parse_success_count++;

        printf("parse success\n");
        app_log_info("parse sensor data success");

        ret = build_json(&sensor, json_buf, sizeof(json_buf));
        if (ret != 0)
        {
            printf("build json failed\n");
            app_log_error("build json failed");
            continue;
        }

        printf("json: %s\n", json_buf);

        ret = write_log(json_buf);
        if (ret != 0)
        {
            printf("write sensor log failed\n");
            app_log_error("write sensor log failed");
            continue;
        }

        printf("write log success\n");
        app_log_info("write sensor log success");

        ret = mqtt_publish_json(mosq, json_buf);
        if (ret != 0)
        {
            stats.mqtt_fail_count++;

            printf("mqtt publish json failed\n");
            app_log_error("mqtt publish json failed");
            continue;
        }

        stats.mqtt_success_count++;

        printf("mqtt publish success\n");
        app_log_info("mqtt publish success");
    }

    gateway_stats_print(&stats);

    ret = gateway_stats_save_report(&stats);
    if (ret != 0)
    {
        printf("save gateway report failed\n");
        app_log_error("save gateway report failed");
    }
    else
    {
        printf("save gateway report success\n");
        app_log_info("save gateway report success");
    }

    mqtt_client_cleanup(mosq);
    data_source_cleanup();

    app_log_info("gateway stats end");
    printf("gateway stats end\n");

    return 0;
}
