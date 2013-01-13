/**
 * Classes for formatting special markup within an Ice slice definition comment
 */
#ifndef _SLICECOMMENTPARSER_HPP_
#define _SLICECOMMENTPARSER_HPP_

#include <list>
#include <map>
#include <sstream>
#include <string>
#include "LinkHandler.hpp"


/**
 * Returns chunks of text based on whether they begin with @.
 * Removes newlines.
 */
class SliceCommentSplitter
{
public:
    /**
     * White space characters
     */
    static const std::string WHITESPACE;

    /**
     * Initialise the comment splitter
     * @param comment The complete comment string
     */
    SliceCommentSplitter(std::string comment);

    /**
     * Check if there is more text to come
     * @return true if there is remaining text
     */
    bool hasNext();

    /**
     * Get the next chunk of text
     * @return the next chunk of text
     */
    std::string next();

private:
    bool readNextLine();

    void getNext();

    bool _good;

    std::stringstream _comment;

    std::string _nextLine;

    std::string _nextChunk;
};




/**
 * Converts special slice comment tags into restructured text
 * http://zeroc.com/doc/Ice-3.3.1/manual/Slice.5.23.html
 * @TODO Implement links ([])
 */
class SliceCommentParser
{
public:
    /**
     * Stores a chunk of text, including tags if present
     */
    struct TagValues
    {
	std::string tag;
	std::string value1;
	std::string value2;
    };

    /**
     * Parse a comment
     * @param comment the complete comment
     */
    SliceCommentParser(const std::string comment, LinkHandler& linker);

    /**
     * Get the list of parsed text
     * @return a list of parsed text including tags if found
     */
    const std::list<TagValues>& getParsedTagValues() const;

    /**
     * Get the restructured text representation of a tag/value without indentation.
     * @return The tag/value formatted as restructured text
     */
    std::string getTagValueText(const TagValues& tv) const;

    /**
     * Get the restructured text representation of a comment
     * @return The comment formatted as restructured text
     */
    std::string getParsedText(std::string prefix) const;

private:
    void parse();

    TagValues parseChunk(const std::string s);

    LinkHandler& _linker;

    SliceCommentSplitter _splitter;

    std::list<TagValues> _tvlist;
};


/**
 * Deal with param definitions specified in the comment
 */
class FunctionParams
{
public:
    /**
     * The documentation of a parameter
     */
    struct ParamDescription
    {
        /**
         * The name of the parameter
         */
        std::string name;

        /**
         * The type of the parameter
         */
        std::string type;

        /**
         * Is this an input or output parameter?
         */
        std::string inout;

        /**
         * Description if provided
         */
        std::string description;
    };

    /**
     * A new object for matching up parameter names, types and descriptions
     */
    FunctionParams();

    /**
     * Add a parameter description from a comment
     * @param name Name of the parameter
     * @description Description of the parameter
     */
    void addCommentParam(std::string name, std::string description);

    /**
     * Add a parameter from the slice parser
     * @param name Name of the parameter
     * @param type The type of the parameter
     * @param inout Is this parameter used for input or output
     */
    void addAutoParam(std::string name, std::string type, std::string inout);

    /**
     * Match up the descriptions from the comment with the automatically found parameters
     * @return the list of parameters
     */
    const std::list<ParamDescription>& combineDescriptions();

private:

    std::map<std::string, std::string> _commentParams;

    std::list<ParamDescription> _params;
};


#endif // _SLICECOMMENTPARSER_HPP_
