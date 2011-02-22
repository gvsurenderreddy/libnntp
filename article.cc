#include "nntp.h"
#include "group.h"
#include "article.h"
#include "exceptions.h"

namespace nntp
{
    // load and cache all headers
    void article::load_headers()
    {
        char        command[64];// command to send
        char        line[1024]; // buffer for response line
        char        *separator; // pointer to the separator between name and value
        std::string name;       // header name
        std::string value;      // header value

        // create command line
        sprintf(command, "HEAD %s\n", msg_id);

        // make sure we are running in the right group
        nntp_group->activate();

        // cancel if we are not getting the right response
        if (connection->process_command(command) != 221)
            throw server_exception("Unexpected reply from server.");

        // keep going till all the headers are in
        while (connection->read_multiline(line))
        {
            // find the : and cut the line
            separator   =   strchr(line, ':');
            *separator  =   '\0';
            separator   +=  2;

            // make them into std::strings
            name        =   line;
            value       =   separator;

            // and put them in the map
            headers[name]=  value;
        }
    }

    // load and cache body content
    void article::load_content()
    {
        char    line[64];   // buffer for line to send

        // if we already have content, return immediately
        if (content != NULL)
            return;

        // build the command
        sprintf(line, "BODY %s\n", msg_id);

        // make sure we are running in the right group
        nntp_group->activate();

        // send it to the server
        length  =   connection->process_block_command(line, 222, &content);
    }

    // construct article based on connection, group and article number
    article::article(nntp *connection, group_ptr nntp_group, long number, const char *article_id) :
        connection(connection),
        nntp_group(nntp_group),
        number(number),
        content(NULL),
        length(0),
        references(0)
    {
        // allocate memory for message id and copy it
        msg_id      =   new char [strlen(article_id) + 1];
        strcpy(msg_id, article_id);
    }

    // clean up
    article::~article()
    {
        // delete message id
        delete [] msg_id;

        // and delete content if necessary
        if (content != NULL)
            delete [] content;
    }

    // get a header by name
    bool article::header(const std::string& name, std::string& value)
    {
        // if we do not have any headers yet, cache them now
        if (headers.empty())
            load_headers();

        // find the header
        header_iterator =   headers.find(name);

        // check if the header does not exist
        if (header_iterator == headers.end())
            return false;

        // copy the value
        value   =   header_iterator->second;
        return true;
    }

    // get the articles undecoded content
    void article::body(std::string& value)
    {
        // check if we already have the contents
        if (content == NULL)
            load_content();

        // copy the body contents into the provided string
        value   =   content;
    }

    // get the decoded articles content
    decoded_article_ptr article::decode()
    {
        // if we already cached the results, return them
        if (decoded != NULL)
            return decoded;

        // check if we already have the contents or if we can get them
        if (content == NULL)
            load_content();

        // create a new decoded article
        decoded =   new decoded_article(content, length);

        // return the result
        return decoded;
    }
}