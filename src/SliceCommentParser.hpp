/**
 * Classes for formatting special markup within an Ice slice definition comment
 */
#ifndef _SLICECOMMENTPARSER_HPP_
#define _SLICECOMMENTPARSER_HPP_

#include <list>
#include <string>


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
    SliceCommentParser(const std::string comment);

    /**
     * Get the list of parsed text
     * @return a list of parsed text including tags if found
     */
    const std::list<TagValues>& getParsedTagValues() const;

    /**
     * Get the restructured text representation of a comment
     * @return The comment formatted as restructured text
     */
    std::string getParsedText(std::string prefix) const;

private:
    void parse();

    TagValues parseChunk(const std::string s);

    SliceCommentSplitter _splitter;

    std::list<TagValues> _tvlist;
};

#endif // _SLICECOMMENTPARSER_HPP_
