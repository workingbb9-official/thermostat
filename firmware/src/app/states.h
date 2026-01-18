#ifndef STATES_H
#define STATES_H

#ifndef NULL
    #define NULL ((void *) 0)
#endif

struct state_ops {
    void (*init)(void);
    void (*on_keypress)(void);
    void (*process)(void);
    void (*display)(void);
    void (*send)(void);
    void (*receive)(void);
    void (*exit)(void);
};

extern const struct state_ops state_login;
extern const struct state_ops state_home;
extern const struct state_ops state_stats;

void sys_change_state(const struct state_ops *new_state);

#endif // STATES_H
