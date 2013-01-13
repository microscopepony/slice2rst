#ifndef _LINKHANDLER_H_
#define _LINKHANDLER_H_

#include <string>


/**
 * Methods for dealing with in-line links
 */
class LinkHandler
{
public:
    virtual ~LinkHandler() {
    }

    /**
     * This method will be called when a string which should contain a typename needs to
     * be handled or formatted. No validation is done on the supplied string.
     * @param s A string which should contain a typename
     * @return The formatted typename string
     */
    virtual std::string type(std::string s) {
        return s;
    }

    /**
     * Any comment text forming part of a description will be passed to this method, so
     * that inline links can be parsed and converted if required.
     * @param s A comment string which may contain links
     * @return The comment string with links formatted as required
     */
    virtual std::string inlineLinkParser(std::string s) {
        return s;
    }
};



/**
 * A very basic link handler.
 * Assumes all typenames are classes.
 * Assumes all [link] style links are classes.
 */
class BasicClassLinkHandler: public LinkHandler
{
public:
    virtual std::string type(std::string s);

    virtual std::string inlineLinkParser(std::string s);
};


#endif
