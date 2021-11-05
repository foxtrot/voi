#ifndef FILTER_H
#define FILTER_H

#include "../packet/packet.h"


#define VOI_PACKET_ALLOWED 0
#define VOI_PACKET_DENIED 1
#define VOI_PACKET_PENDING 2


typedef struct voi_filter_counters {
    unsigned long long allowed;
    unsigned long long denied;
} voi_filter_counters_t;

extern voi_filter_counters_t *voi_filter_count;

int voi_packet_filter_init(void);
int voi_packet_filter_stop(void);
int voi_packet_filter_count_reset(void);
int voi_packet_filter_check(voi_packet_t *voi_pkt);


#endif /* HELPERS_H */
