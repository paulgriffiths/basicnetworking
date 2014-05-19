/*!
 * \file            messages.h
 * \brief           Implementation of chat protocol message functionality.
 * \details         Implementation of chat protocol message functionality.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

/*!  POSIX feature test macro  */
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

/*!  Structure to contain information about supported messages  */
struct known_msg {
    enum chatc_msg msg_id;  /*!<  The message id                            */
    const char * name;      /*!<  A string name for the message             */
    int num_params;         /*!<  The number of parameters the message has  */
};

/*!  File scope variable to contain a list of supported messages  */
static struct known_msg known_msgs[] = {
    {CHAT_MESSAGE_SAY, "say", 1},
    {CHAT_MESSAGE_TELL, "tell", 2},
    {CHAT_MESSAGE_NICK, "nick", 1},
    {CHAT_MESSAGE_QUIT, "quit", 0},
    {CHAT_MESSAGE_BADMSG, NULL, 0}
};

/*!
 * \brief           Gets the next word from input.
 * \details         Gets the next word from input.
 * \param input     The input from which to get the next word.
 * \param buffer    A buffer in which to store the next word. The caller
 * is responsible for ensuring that this buffer is large enough to contain
 * the word.
 * \returns         A pointer to the start of the word *after* the next word
 * which will be stored in `buffer`. This pointer can be passed as `input`
 * on a subsequent call to this function, to allow successive words to be
 * retrieved. `NULL` will be returned if there is no following word,
 * or if `input` contains no words at all. If there are no words at all,
 * `buffer` will be set to the empty string, and can be checked to determine
 * for which of the two possible reasons `NULL` was returned.
 */
static const char * get_next_word(const char * input, char * buffer);

/*!
 * \brief           Translates a message name into a message ID.
 * \details         Translates a message name into a message ID.
 * \param msg_name  The message name to translate.
 * \returns         The message ID corresponding to the message name.
 * `CHAT_MESSAGE_BADMSG` will be returned if the message name is not
 * recognized.
 */
static enum chatc_msg translate_message_name(const char * msg_name);

/*!
 * \brief           Creates a dynamically allocated list of strings.
 * \details         This function dynamically created a list of pointers
 * to `char` of length `length`. The pointers are set to `NULL`. One more
 * element than requested is always allocated, which should remain set to
 * `NULL` to act as a sentinel.
 * \param length    The number of elements in the list.
 * \returns         A pointer to the newly created list. The caller is
 * responsible for `free()`ing this pointer with a call to
 * `free_string_list()`.
 */
static char ** create_string_list(const size_t length);

/*!
 * \brief           Frees a dynamically allocated list of strings.
 * \details         This function will `free()` all the elements of
 * the list until it encounters the *first* element set to `NULL`, and
 * then `free()` the list itself. The caller is responsible for ensuring
 * that there are no elements after the first `NULL` which need to be
 * `free()`d, as this function will not `free()` them.
 */ 
static void free_string_list(char ** slist);

/*!
 * \brief           Parses a list of arguments from a buffer.
 * \details         Parses a list of arguments from a buffer.
 * \param buffer    The buffer from which to parse the arguments.
 * \param num_args  The expected number of arguments.
 * \param residual  Set to `true` if the last argument should be set to
 * the entire remainder of the string. All arguments other than the last
 * can be no more than a single word. Set to `false` to ensure the last
 * argument is also a single world.
 * \returns         A pointer to a list of strings containing the arguments.
 * The caller is responsible for `free()`ing this list by passing it to
 * `free_string_list()`. `NULL` will be returned if fewer arguments
 * than `num_args` can be parsed from `buffer()`, or if more arguments
 * than `num_args` can be parsed and `residual` is not set to `true`. If
 * residual is set to `true`, then the rest of the line will be set as the
 * final argument, so there will be no extra arguments by definition.
 */
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

