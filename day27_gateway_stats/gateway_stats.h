#ifndef GATEWAY_STATS_H
#define GATEWAY_STATS_H

typedef struct
{
  int total_count;
  int parse_success_count;
  int parse_fail_count;
  int mqtt_success_count;
  int mqtt_fail_count;
}GatewayStats;

void gateway_stats_init(GatewayStats *stats);
void gateway_stats_print(const GatewayStats *stats);
int gateway_stats_save_report(const GatewayStats *stats);

#endif
