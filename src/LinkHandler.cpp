#include "LinkHandler.hpp"



std::string BasicClassLinkHandler::type(std::string s)
{
    std::size_t p = 0;

    while (true)
    {
	p = s.find("::", p);
	if (p == std::string::npos)
	{
	    break;
	}

	if (p == 0)
	{
	    s = s.substr(2);
	}
	else
	{
	    s = s.substr(0, p) + "." + s.substr(p + 2);
	}
    }

    s = ":class:`" + s + "`";
    return s;
}

std::string BasicClassLinkHandler::inlineLinkParser(std::string s)
{
    std::string t;

    // p: Start of the substring under consideration
    // q: Opening [
    // r: Closing ]
    size_t p = 0, q = 0, r = 0;

    while (true)
    {
	q = s.find_first_of('[', p);
	if (q == std::string::npos)
	{
	    t += s.substr(p);
	    break;
	}
	t += s.substr(p, q - p);

	if (q > 0 && s[q - 1] == '\\')
	{
	    // @todo handle \\[
	    t += s.substr(p, q - p + 1);
	    p = q + 1;
	    continue;
	}

	r = s.find_first_of(']', q);
	if (r == std::string::npos)
	{
	    t += s.substr(p);
	    break;
	}

	if (r - q == 1)
	{
	    // []
	    t += s.substr(p, r - p + 1);
	    p = r + 1;
	    continue;
	}

	// Finally, we have a link
	t += " :class:`" + s.substr(q + 1, r - q - 1) + "` ";
	p = r + 1;
    }

    return t;
}
