#ifndef ARTICLE_H
#define ARTICLE_H 1

#include "decoded_article.h"
#include <boost/intrusive_ptr.hpp>
#include "intrusive_ptr.h"

namespace nntp
{
    // typedefs
    typedef std::map<std::string, std::string>      header_list;
    typedef boost::intrusive_ptr<decoded_article>   decoded_article_ptr;

    /**
      * @class  nntp::article
      *
      * This class represents a single usenet article. Headers can be retrieved with the header() function, data can be
      * retrieved with the body function. To decode binary data, use the decode() function. See the nntp::decoded_article
      * class for more information on how to work with decoded binaries.
      */
    class article
    {
        private:
            nntp                    *connection;        // usenet connection
            group_ptr               nntp_group;         // group article belongs to
            long                    number;             // article number in group
            char                    *msg_id;            // message id
            header_list             headers;            // headers for this article
            header_list::iterator   header_iterator;    // iterator for cached headers
            char                    *content;           // pointer to body contents
            int                     length;             // length of content
            decoded_article_ptr     decoded;            // pointer to decoded article
            size_t                  references;         // reference count to this object

            friend void ::boost::intrusive_ptr_add_ref<>(article *p);
            friend void ::boost::intrusive_ptr_release<>(article *p);

            /**
              * Load all the headers in this article
              *
              * @throws network_exception, server_exception
              */
            void load_headers();
        public:
            /**
              * Construct article based on it's message id and number in the group
              *
              * @param  connection      connection to our usenet server
              * @param  nntp_group      group that the article is in
              * @param  number          article number
              * @param  article_id      globally unique message id
              */
            article(nntp *connection, group_ptr nntp_group, long number, const char *article_id);

            /**
              * Destructor
              */
            ~article();

            /**
              * Make sure content is downloaded. All functions working with content will load their content
              * automatically when needed, so this function is only useful when you want the downloading and
              * decoding of posts in a different thread.
              *
              * @throws network_exception, server_exception
              */
            void load_content();

            /**
              * Get a header
              *
              * @throws network_exception, server_exception
              *
              * @param  name    name of the header to retrieve
              * @param  value   string to put the header in
              * @return does the header exist
              */
            bool header(const std::string& name, std::string& value);

            /**
              * Get the contents of the article, undecoded
              *
              * @throws network_exception, server_exception
              *
              * @param  contents    string to put the contents in
              */
            void body(std::string& contents);

            /**
              * Get the contents of the article, decoded
              *
              * @throws network_exception, server_exception, decode_exception
              *
              * @return the decoded article
              */
            decoded_article_ptr decode();
    };
}

#endif /* ARTICLE_H */