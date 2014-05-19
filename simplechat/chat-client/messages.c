#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <ctype.h>
#include "messages.h"
#include "string_functions.h"
#include "logging.h"

struct known_msg {
    enum chatc_msg msg_id;
    const char * name;
    int num_params;
};

static struct known_msg known_msgs[] = {
    {CHAT_MESSAGE_SAY, "say", 1},
    {CHAT_MESSAGE_TELL, "tell", 2},
    {CHAT_MESSAGE_NICK, "nick", 1},
    {CHAT_MESSAGE_QUIT, "quit", 0},
    {CHAT_MESSAGE_BADMSG, NULL, 0}
};

static const char * get_next_word(const char * input, char * buffer);
static enum chatc_msg translate_message_name(const char * msg_name);
static char ** create_string_list(const size_t length);
static void free_string_list(char ** slist);
static char ** parse_args(const char * buffer,
                          const size_t num_args,
                          const bool residual);

struct chat_msg * chatc_parse_message(const char * msg) {
    struct chat_msg * new_msg = malloc(sizeof *new_msg);
    if ( !new_msg ) {
        perror("chat-client: couldn't allocate message");
        exit(EXIT_FAILURE);
    }
    
    if ( *msg == '\0' ) {

        /*  Return bad message if the msg string is empty  */

        new_msg->msg_id = CHAT_MESSAGE_BADMSG;
        new_msg->parameters = NULL;
    }
    else {

        /*  Allocate and trim a temporary buffer to
         *  avoid messing with the supplied msg string  */

        char * buffer = strdup(msg);
        if ( !buffer ) {
            perror("chat-client: couldn't allocate message");
            exit(EXIT_FAILURE);
        }
        trim(buffer);
        log_msg("Whole trimmed buffer is [%s]\n", buffer);

        /*  Set up a temporary scratch buffer to hold the words  */

        char * scratch = malloc(strlen(buffer) + 1);
        if ( !scratch ) {
            perror("chat-client: couldn't allocate message");
            exit(EXIT_FAILURE);
        }

        /*  Get first word from msg and check it
         *  against list of accepted commands     */

        const char * next = get_next_word(buffer, scratch);
        //log_msg("Current word is: %s\n", scratch);
        //log_msg("Next word is: %s\n", (next ? next : "[none]"));
        new_msg->msg_id = translate_message_name(scratch);

        switch ( new_msg->msg_id ) {
            case CHAT_MESSAGE_SAY:
                new_msg->parameters = parse_args(next, 1, true);
                if ( !new_msg->parameters ) {
                    new_msg->parameters = NULL;
                    new_msg->msg_id = CHAT_MESSAGE_BADMSG;
                    log_msg("Say is flipping to bad message...\n");
                }
                break;

            case CHAT_MESSAGE_TELL:
                new_msg->parameters = parse_args(next, 2, true);
                if ( !new_msg->parameters ) {
                    new_msg->parameters = NULL;
                    new_msg->msg_id = CHAT_MESSAGE_BADMSG;
                    log_msg("Tell is flipping to bad message...\n");
                }
                break;

            case CHAT_MESSAGE_NICK:
                new_msg->parameters = parse_args(next, 1, false);
                if ( !new_msg->parameters ) {
                    new_msg->parameters = NULL;
                    new_msg->msg_id = CHAT_MESSAGE_BADMSG;
                    log_msg("Nick is flipping to bad message...\n");
                }
                break;

            case CHAT_MESSAGE_QUIT:
                new_msg->parameters = NULL;
                if ( next ) {
                    log_msg("Quit is flipping to bad message...\n");
                    new_msg->msg_id = CHAT_MESSAGE_BADMSG;
                }
                break;

            case CHAT_MESSAGE_BADMSG:
                new_msg->parameters = NULL;
                log_msg("Bad message!\n");
                break;

            default:
                assert(false);
                break;
        }
        free(buffer);
        free(scratch);
    }

    return new_msg;
}

void chatc_free_message(struct chat_msg * msg) {
    free_string_list(msg->parameters);
    free(msg);
}

static const char * get_next_word(const char * input, char * buffer) {

    /*  Skip leading whitespace  */

    while ( *input && isspace(*input) ) {
        ++input;
    }

    if ( *input ) {

        /*  There is a token in the input, so copy it to buffer  */

        while ( *input && !isspace(*input) ) {
            *buffer++ = *input++;
        }
        *buffer = '\0';

        /*  Skip past trailing whitespace  */

        while ( *input && isspace(*input) ) {
            ++input;
        }

        if ( *input ) {

            /*  There is a following token, so return its address  */

            return input;
        }
        else {

            /*  There is no following token  */

            return NULL;
        }
    }
    else {

        /*  There is no token in the input  */

        *buffer = '\0';
        return NULL;
    }
}

static enum chatc_msg translate_message_name(const char * msg_name) {
    size_t i = 0;
    while ( known_msgs[i].name ) {
        //log_msg("Testing [%s] and [%s]...\n", msg_name, known_msgs[i].name);
        if ( !strcasecmp(known_msgs[i].name, msg_name) ) {
            //log_msg("Matched message: %s\n", known_msgs[i].name);
            return known_msgs[i].msg_id;
        }
        ++i;
    }
    return known_msgs[i].msg_id;
}

static char ** create_string_list(const size_t length) {
    assert(length > 0);

    char ** new_list = malloc((length + 1) * sizeof *new_list);
    if ( !new_list ) {
        perror("chat-client: couldn't allocate string list");
        exit(EXIT_FAILURE);
    }

    for ( size_t i = 0; i <= length; ++i ) {
        new_list[i] = NULL;
    }

    return new_list;
}

static void free_string_list(char ** slist) {
    if ( slist ) {
        size_t i = 0;

        while ( slist[i] ) {
            free(slist[i++]);
        }

        free(slist);
    }
}

static char ** parse_args(const char * buffer,
                          const size_t num_args,
                          const bool residual) {
    if ( !buffer ) {
        return NULL;
    }

    char * scratch = malloc(strlen(buffer) + 1);
    if ( !scratch ) {
        perror("chat-client: couldn't allocate memory");
        exit(EXIT_FAILURE);
    }

    const char * next_arg = buffer;
    char ** args = create_string_list(num_args);
    bool missing_arg = false;

    for ( size_t i = 0; i < num_args; ++i ) {
        if ( (i == (num_args - 1)) && residual ) {

            /*  This is the last argument, and we want to take
             *  the residual (i.e. the entire remaining string,
             *  not just the next word.                          */

            if ( next_arg ) {
                args[i] = strdup(next_arg);
                if ( !args[i] ) {
                    perror("chat-client: couldn't allocate memory");
                    exit(EXIT_FAILURE);
                }

                /*  We are not calling get_next_word(), so set
                 *  next to NULL manually to signify that there
                 *  are no erroneous additional arguments (there
                 *  cannot be any by definition when the last
                 *  argument is the whole of the remaining string).  */

                next_arg = NULL;
            }
            else {

                /*  There is no remaining string, so the expected
                 *  number of arguments were not supplied.         */

                missing_arg = true;
            }
        }
        else {

            /*  This is not the last argument, or we don't
             *  want the residual, and the last argument
             *  should be a single word, so treat it like
             *  any other.                                  */

            next_arg = get_next_word(next_arg, scratch);
            if ( *scratch ) {

                /*  An argument was found as expected, so copy it  */

                args[i] = strdup(scratch);
                if ( !args[i] ) {
                    perror("chat-client: couldn't allocate memory");
                    exit(EXIT_FAILURE);
                }
            }
            else {

                /*  The expected argument was not found, so
                 *  exit loop with error condition.          */

                missing_arg = true;
                break;
            }
        }
    }

    if ( next_arg || missing_arg ) {

        /*  There were too many arguments, or not enough, so
         *  empty parameters and return with error in either case.  */

        free_string_list(args);
        args = NULL;
    }

    free(scratch);
    return args;
}

