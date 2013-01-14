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




SliceCommentParser::SliceCommentParser(const std::string comment, LinkHandler& linker):
    _linker(linker), _splitter(comment)
{
    parse();
}

const std::list<SliceCommentParser::TagValues>&
SliceCommentParser::getParsedTagValues() const
{
    return _tvlist;
}

std::string SliceCommentParser::getTagValueText(const TagValues& tv) const
{
    if (tv.tag.empty())
    {
        return _linker.inlineLinkParser(tv.value1);
    }
    if (tv.tag == "@param")
    {
        return ":param " + tv.value1 + ": " + _linker.inlineLinkParser(tv.value2);
    }
    if (tv.tag == "@throws")
    {
        return ":raises: " + _linker.type(tv.value1) + ": " +
	    _linker.inlineLinkParser(tv.value2);
    }
    if (tv.tag == "@return")
    {
        return ":returns: " + _linker.inlineLinkParser(tv.value1);
    }

    // @todo output a warning
    return _linker.inlineLinkParser(tv.value1);
}

std::string SliceCommentParser::getParsedText(std::string prefix) const
{
    std::string s = "\n";
    std::string end = "\n\n";
    for (std::list<TagValues>::const_iterator it = _tvlist.begin();
	 it != _tvlist.end(); ++it)
    {
        s += prefix + getTagValueText(*it) + end;
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




FunctionParams::FunctionParams()
{
}

void FunctionParams::addCommentParam(std::string name, std::string description)
{
    _commentParams[name] = description;
}

void FunctionParams::addAutoParam(std::string name, std::string type, std::string inout)
{
    ParamDescription p;
    p.name = name;
    p.type = type;
    p.inout = inout;
    _params.push_back(p);
}

const std::list<FunctionParams::ParamDescription>& FunctionParams::combineDescriptions()
{
    if (_commentParams.empty())
    {
        return _params;
    }

    // Run through the automatically found parameters, and see if a description was
    // provided in the comment. Then add any additional parameters from the comment.

    for (std::list<ParamDescription>::iterator p = _params.begin();
         p != _params.end(); ++p)
    {
        std::map<std::string, std::string>::iterator c = _commentParams.find(p->name);
        if (c != _commentParams.end())
        {
            p->description = c->second;
            _commentParams.erase(c);
        }
    }

    for (std::map<std::string, std::string>::const_iterator c = _commentParams.begin();
         c != _commentParams.end(); ++c)
    {
        ParamDescription d;
        d.name = c->first;
        d.description = c->second;
        _params.push_back(d);
    }
    _commentParams.clear();

    return _params;
}


