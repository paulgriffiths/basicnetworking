#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "messages.h"
#include "test_messages.h"
#include "test_logging.h"

void test_messages(void) {
    test_message_id("say hello world", CHAT_MESSAGE_SAY, true);
    test_message_id("SAY hello", CHAT_MESSAGE_SAY, true);
    test_message_id("say", CHAT_MESSAGE_SAY, false);
    test_message_id("say", CHAT_MESSAGE_BADMSG, true);
    test_message_id(" tell bob eat these rats\n", CHAT_MESSAGE_TELL, true);
    test_message_id("tELl dave whut??", CHAT_MESSAGE_TELL, true);
    test_message_id("tell doug", CHAT_MESSAGE_TELL, false);
    test_message_id("  tell", CHAT_MESSAGE_TELL, false);
    test_message_id("tell doug", CHAT_MESSAGE_BADMSG, true);
    test_message_id("tell", CHAT_MESSAGE_BADMSG, true);
    test_message_id("nick topsy  ", CHAT_MESSAGE_NICK, true);
    test_message_id("    NicK   ", CHAT_MESSAGE_NICK, false);
    test_message_id(" nick johnny three names  ", CHAT_MESSAGE_NICK, false);
    test_message_id(" nick billy bob  ", CHAT_MESSAGE_NICK, false);
    test_message_id("    nick   ", CHAT_MESSAGE_BADMSG, true);
    test_message_id(" nick johnny three names  ", CHAT_MESSAGE_BADMSG, true);
    test_message_id(" nick billy bob  ", CHAT_MESSAGE_BADMSG, true);
    test_message_id(" QUIT  ", CHAT_MESSAGE_QUIT, true);
    test_message_id("Quit", CHAT_MESSAGE_QUIT, true);
    test_message_id("Quit extra", CHAT_MESSAGE_QUIT, false);
    test_message_id("Quit extra", CHAT_MESSAGE_BADMSG, true);
    test_message_id("shizzle", CHAT_MESSAGE_BADMSG, true);
    test_message_id("where's the bridge?", CHAT_MESSAGE_BADMSG, true);

    test_message_param("say hello", 0, "hello", true);
    test_message_param("SAY hello", 1, NULL, true);
    test_message_param("  say hello world  ", 0, "hello world", true);
    test_message_param("sAy hello world", 1, NULL, true);
    test_message_param("say hello world", 0, "hello", false);
    test_message_param("TEll   billy what's up?", 0, "billy", true);
    test_message_param("tELL billy what's up?   ", 1, "what's up?", true);
    test_message_param("tell billy what's up?", 2, NULL, true);
    test_message_param("nick bonzo", 0, "bonzo", true);
    test_message_param("NICK donkeyman", 1, NULL, true);
}

bool test_message_id(const char * str,
                     const enum chatc_msg id,
                     const bool reverse) {
    struct chat_msg * msg = chatc_parse_message(str);

    bool test_result;
    if ( msg->msg_id == id ) {
        test_result = reverse ? true : false;
    }
    else {
        test_result = reverse ? false : true;
    }

    tests_log_test(test_result, "test_message_id");

    chatc_free_message(msg);
    return test_result;
}

bool test_message_param(const char * str,
                        const int param,
                        const char * expected,
                        const bool reverse) {
    struct chat_msg * msg = chatc_parse_message(str);

    bool test_result;
    if ( msg->parameters[param] ) {
        if ( !strcmp(msg->parameters[param], expected ) ) {
            test_result = reverse ? true : false;
        }
        else {
            test_result = reverse ? false : true;
        }
    }
    else {
        if ( !msg->parameters[param] ) {
            test_result = reverse ? true : false;
        }
        else {
            test_result = reverse ? false : true;
        }
    }

    tests_log_test(test_result, "test_message_param");

    chatc_free_message(msg);
    return test_result;
}
