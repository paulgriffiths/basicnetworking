#ifndef PG_CHAT_CLIENT_MESSAGES_H
#define PG_CHAT_CLIENT_MESSAGES_H

enum chatc_msg {
    CHAT_MESSAGE_SAY,
    CHAT_MESSAGE_TELL,
    CHAT_MESSAGE_NICK,
    CHAT_MESSAGE_QUIT,
    CHAT_MESSAGE_BADMSG
};

struct chat_msg {
    enum chatc_msg msg_id;
    char ** parameters;
};

struct chat_msg * chatc_parse_message(const char * msg);
void chatc_free_message(struct chat_msg * msg);

#endif      /*  PG_CHAT_CLIENT_MESSAGES_H  */
