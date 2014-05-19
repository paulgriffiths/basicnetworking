/*!
 * \file            messages.h
 * \brief           Interface to chat protocol message functionality.
 * \details         Interface to chat protocol message functionality.
 * \author          Paul Griffiths
 * \copyright       Copyright 2014 Paul Griffiths. Distributed under the terms
 * of the GNU General Public License. <http://www.gnu.org/licenses/>
 */

#ifndef PG_CHAT_CLIENT_MESSAGES_H
#define PG_CHAT_CLIENT_MESSAGES_H

/*!  Enumeration for recognized chat messages  */
enum chatc_msg {
    CHAT_MESSAGE_SAY,       /*!<  Send a message to the entire room     */
    CHAT_MESSAGE_TELL,      /*!<  Send a message to a specific user     */
    CHAT_MESSAGE_NICK,      /*!<  Change nickname                       */
    CHAT_MESSAGE_QUIT,      /*!<  Quit the program                      */
    CHAT_MESSAGE_BADMSG     /*!<  Badly-formed or unrecognized message  */
};

/*!  Structure to contain a message and its parameters, if any  */
struct chat_msg {
    enum chatc_msg msg_id;  /*!<  The message identifier                */
    char ** parameters;     /*!<  A list of parameters                  */
};

/*!
 * \brief           Parses a line of input into a message.
 * \details         Parses a line of input into a message.
 * \param msg       The line of input to parse.
 * \returns         A pointer to a dynamically allocated `struct chat_msg`
 * containing the parsed message. The caller is responsible for `free()`ing
 * this memory by calling `chatc_free_message()`.
 */
struct chat_msg * chatc_parse_message(const char * msg);

/*!
 * \brief           Frees a dynamically allocated `struct chat_msg`.
 * \details         Frees a dynamically allocated `struct chat_msg`.
 * \param msg       The `struct chat_msg` to free. This structure should
 * have been created and returned by `chatc_parse_message()`.
 */ 
void chatc_free_message(struct chat_msg * msg);

#endif      /*  PG_CHAT_CLIENT_MESSAGES_H  */
