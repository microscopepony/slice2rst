#include "SliceCommentParser.hpp"
#include <iostream>

int testCommentParser()
{
    std::string comment =
	"Lorem ipsum dolor sit amet, consectetuer adipiscing elit,\n"
	"sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna\n"
	"aliquam erat volutpat.\n"
	"\n"
	"@param a First parameter\n"
	"@param b Second parameter\n"
	"@return Ut wisi enim ad minim veniam, quis nostrud exerci tation\n"
	"        ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo\n"
	"        consequat.\n"
	"@throws Exception Raised if something goes wrong\n";

    SliceCommentParser scp(comment);
    //scp.getParsedTagValues();
    std::string prefix = "    ";
    std::string text = scp.getParsedText(prefix);

    std::cout << text;
    return 0;
}

int main()
{
    return testCommentParser();
}




