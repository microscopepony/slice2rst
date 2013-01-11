#include <sstream>
#include "SliceCommentParser.hpp"


const std::string SliceCommentSplitter::WHITESPACE = " \f\n\r\t";

SliceCommentSplitter::SliceCommentSplitter(std::string comment):
    _good(true), _comment(comment)
{
}

bool SliceCommentSplitter::hasNext()
{
    if (_good && _nextChunk.empty())
    {
	getNext();
    }

    return !_nextChunk.empty();
}

std::string SliceCommentSplitter::next()
{
    hasNext();
    std::string nc = _nextChunk;
    _nextChunk = "";
    return nc;
}



bool SliceCommentSplitter::readNextLine()
 {
     if (_good)
     {
	 _good = false;
	 if (std::getline(_comment, _nextLine, '\n'))
	 {
	     _good = true;
	 }
     }
     return _good;
 }

void SliceCommentSplitter::getNext()
{
    _nextChunk = "";

    while (true)
    {
	if (_nextLine.empty())
	{
	    readNextLine();
	}

	if (!_good)
	{
	    break;
	}

	size_t p = _nextLine.find_first_not_of(
	    SliceCommentSplitter::WHITESPACE);
	if (p != std::string::npos && _nextLine[p] == '@')
	{
	    if (_nextChunk.empty())
	    {
		_nextChunk = _nextLine;
		_nextLine = "";
	    }
	    else
	    {
		// _nextLine will be the start of the next chunk
		break;
	    }
	}
	else
	{
	    if (!_nextChunk.empty())
	    {
		_nextChunk += ' ';
	    }
	    _nextChunk += _nextLine;
	    _nextLine = "";
	}
    }
}



SliceCommentParser::SliceCommentParser(const std::string comment):
    _splitter(comment)
{
    parse();
}

const std::list<SliceCommentParser::TagValues>&
SliceCommentParser::getParsedTagValues() const
{
    return _tvlist;
}

std::string SliceCommentParser::getParsedText(std::string prefix) const
{
    std::string s = "\n";
    std::string end = "\n\n";
    for (std::list<TagValues>::const_iterator it = _tvlist.begin();
	 it != _tvlist.end(); ++it)
    {
	if (it->tag.empty())
	{
	    s += prefix + it->value1 + end;
	}
	else if (it->tag == "@param")
	{
	    s += prefix + ":param " + it->value1 + ": " +
		it->value2 + end;
	}
	else if (it->tag == "@throws")
	{
	    s += prefix + ":throws " + it->value1 + ": " +
		it->value2 + end;
	}
	else if (it->tag == "@return")
	{
	    s += prefix + ":returns: " + it->value1 + end;
	}
	else
	{
	    s += prefix + it->value1 + end;
	}
    }

    return s;
}



void SliceCommentParser::parse()
{
    while (_splitter.hasNext())
    {
	std::string s = _splitter.next();
	_tvlist.push_back(parseChunk(s));
    }
}

SliceCommentParser::TagValues SliceCommentParser::parseChunk(
    const std::string s)
{
    TagValues tv;

    size_t a = s.find_first_not_of(SliceCommentSplitter::WHITESPACE);
    if (a == std::string::npos || s[a] != '@')
    {
	// Normal text
	tv.value1 = s;
	return tv;
    }

    size_t b = s.find_first_of(SliceCommentSplitter::WHITESPACE, a);
    size_t c = s.find_first_not_of(SliceCommentSplitter::WHITESPACE, b);
    if (b == std::string::npos)
    {
	tv.tag = s.substr(a);
	// No values
	return tv;
    }

    tv.tag = s.substr(a, b - a);

    if (c == std::string::npos)
    {
	// No values
	return tv;
    }

    if (tv.tag == "@param" || tv.tag == "@throws")
    {
	size_t d = s.find_first_of(SliceCommentSplitter::WHITESPACE, c);
	size_t e = s.find_first_not_of(SliceCommentSplitter::WHITESPACE, d);
	if (d == std::string::npos)
	{
	    tv.value1 = s.substr(c);
	    // No value2
	    return tv;
	}

	tv.value1 = s.substr(c, d - c);
	if (e == std::string::npos)
	{
	    return tv;
	}

	tv.value2 = s.substr(e);
	return tv;
    }
    else if (tv.tag == "@return")
    {
	tv.value1 = s.substr(c);
	return tv;
    }
    else
    {
	// Ignore tag, treat as normal text
	tv.tag = "";
	tv.value1 = s;
	return tv;
    }
}


