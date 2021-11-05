#ifndef HOOKING_H
#define HOOKING_H

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

typedef enum {
    VOI_HOOK_INBOUND = 0,
    VOI_HOOK_OUTBOUND
} voi_hook_direction_t;


static inline char *voi_direction_label(voi_hook_direction_t direction) {
    switch (direction) {
        case VOI_HOOK_INBOUND:    return "Inbound";
        case VOI_HOOK_OUTBOUND:   return "Outbound";
        default:                  return "Undefined";
    }
}

unsigned int voi_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state, voi_hook_direction_t direction);
unsigned int voi_in_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state);
unsigned int voi_out_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state);
void voi_register_hooks(void);
void voi_unregister_hooks(void);


#endif /* HOOKING_H */
