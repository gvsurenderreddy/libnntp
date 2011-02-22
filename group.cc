#include "group.h"

namespace nntp
{
    // initialize the group with connection, low and high water mark
    group::group(const std::string& name, nntp *connection, long low, long high) :
        connection(connection),
        low(low),
        high(high),
        group_name(name),
        references(0)
    {}

    // clean up
    group::~group()
    {
    }

    // make sure group is active
    void group::activate()
    {
        connection->activate_group(this);
    }

    // get the group name
    const std::string& group::name()
    {
        return group_name;
    }

    // fetch an article based on it's numeric id in this group
    article_ptr group::fetch_article(long number)
    {
        char    command[64];    // command to send to the server
        char    response[512];  // response from usenet server
        char    *msg_id;        // pointer to message id

        // if the article number is not in range, we have nothing to fetch
        if (number < low || number > high)
            return article_ptr(NULL);

        // article not in cache yet, see if it exists
        sprintf(command, "STAT %ld\n", number);

        // make sure our group is the active one
        activate();

        // send the command to the server
        if (connection->process_command(command, response) != 223)
            return article_ptr(NULL);

        // message id begins at the < character
        msg_id      =   strchr(response, '<');

        // construct new article
        return article_ptr(new article(connection, group_ptr(this), number, msg_id));
    }

    // fetch an article based on it's message id
    article_ptr group::fetch_article(const std::string& msg_id)
    {
        char    command[64];    // command to send to the server
        char    id[64];         // message id
        char    response[512];  // response from usenet server
        long    number;         // message number in group

        // check if the message id is surrounded by <>'s
        if (msg_id.at(0) == '<')
        {
            sprintf(command, "STAT %s\n", msg_id.c_str());
            strcpy(id, msg_id.c_str());
        }
        else
        {
            sprintf(command, "STAT <%s>\n", msg_id.c_str());
            sprintf(id, "<%s>", msg_id.c_str());
        }

        // make sure our group is the active one
        activate();

        // send the command to the server
        if (connection->process_command(command, response) != 223)
            return article_ptr(NULL);

        // get the number from the response
        number  =   atol(response);

        // construct new article
        return article_ptr(new article(connection, group_ptr(this), number, id));
    }
}
