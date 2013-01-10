#include <vector>
#include <list>
#include <sstream>
#include "RstGen.hpp"

// See http://sphinx-doc.org/markup/desc.html for Sphinx directives

using namespace std;
using namespace Slice;

RstGen::
RstGen() :
    _tabSize(4),
    _tabCount(0)
{
}

bool RstGen::
visitUnitStart(const UnitPtr& p)
{
    // Doesn't seem to work:
    //_file = p->currentFile();
    _file = p->topLevelFile();

    std::string title = _file;
    std::string underline = std::string(title.size(), '=');
    cout << title << "\n"
	 << underline << "\n\n";

    return true;
}

void RstGen::
visitUnitEnd(const UnitPtr&)
{
    cout << tab() << ".. debug: end unit\n\n";
    _file = "";
}

bool RstGen::
visitModuleStart(const ModulePtr& p)
{
    if (!isSameFile(*p))
    {
	return true;
    }

    if (_module.empty())
    {
	_module = p->name();
    }
    else
    {
	_module += "." + p->name();
    }
    //_tabCount++;

    //cout << tab() << ".. module:: " << p->name() << "\n\n";
    //cout << tab() << ".. currentmodule:: " << p->name() << "\n\n";
    cout << tab() << ".. module:: " << _module << "\n\n";
    //cout << tab() << ".. currentmodule:: " << _module << "\n\n";

    //_tabCount++;
    //genMetadata(*p);
    //genStrings(*p);
    //--_tabCount;

    return true;
}

void RstGen::
visitModuleEnd(const ModulePtr& p)
{
    if (!isSameFile(*p))
    {
	return;
    }

    cout << tab() << ".. debug: end module " << _module << "\n\n";

    std::size_t n = _module.find_last_of('.');
    if (n == std::string::npos)
    {
	_module = "";
    }
    else
    {
	_module = _module.substr(0, n);
    }

    //--_tabCount;
}

void RstGen::
visitClassDecl(const ClassDeclPtr& p)
{
}

bool RstGen::
visitClassDefStart(const ClassDefPtr& p)
{
    if (!isSameFile(*p))
    {
	return true;
    }
    //_tabCount++;

    cout << tab() << ".. class:: " << p->name() << "\n\n";

    _tabCount++;
    genBody(*p);
    --_tabCount;

    _tabCount++;
    return true;
}

void RstGen::
visitClassDefEnd(const ClassDefPtr& p)
{
    if (!isSameFile(*p))
    {
	return;
    }

    cout << tab() << ".. debug: end class\n\n";

    --_tabCount;
}

bool RstGen::
visitExceptionStart(const ExceptionPtr& p)
{
    if (!isSameFile(*p))
    {
	return true;
    }
    //_tabCount++;

    cout << tab() << ".. exception:: " << p->name() << "\n\n";

    _tabCount++;
    genBody(*p);
    --_tabCount;

    _tabCount++;
    return true;
}

void RstGen::
visitExceptionEnd(const ExceptionPtr& p)
{
    if (!isSameFile(*p))
    {
	return;
    }

    cout << tab() << ".. debug: end exception\n\n";

    --_tabCount;
}

bool RstGen::
visitStructStart(const StructPtr& p)
{
    if (!isSameFile(*p))
    {
	return true;
    }
    //_tabCount++;

    cout << tab() << ".. class:: " << p->name() << "\n\n";

    _tabCount++;
    genBody(*p);
    --_tabCount;

    _tabCount++;
    return true;
}

void RstGen::
visitStructEnd(const StructPtr& p)
{
    if (!isSameFile(*p))
    {
	return;
    }

    cout << tab() << ".. debug: end struct\n\n";

    --_tabCount;
}

void RstGen::
visitOperation(const OperationPtr& p)
{
    if (!isSameFile(*p))
    {
	cout << tab() << ".. debug: skipping " << p->name() << "\n\n";
	return;
    }

    std::string rtp_s = "void";
    TypePtr rtp = p->returnType();

    //_tabCount++;

    //
    // If the return type ptr is null, return type is void.
    //
    if ( rtp )
        rtp_s = rtp->typeId();

    // TODO: Check if this is a global or class method
    cout << tab() << ".. classmethod:: " << p->name() << "(";

    vector<string> params;
    ParamDeclList paramList = p->parameters();

    for(ParamDeclList::const_iterator q = paramList.begin();
        q != paramList.end(); ++q)
    {
        // TODO: Store the types and output them in the description body
        visitParamDecl(*q);
    }

    cout << ")\n\n";

    _tabCount++;

    for(ParamDeclList::const_iterator q = paramList.begin();
        q != paramList.end(); ++q)
    {
	describeParamDecl(*q);
    }

    cout << tab() << ":rtype: " << rtp_s << "\n\n";

    genBody(*p);

    --_tabCount;

    cout << tab() << ".. debug: end operation\n\n";

    //--_tabCount;
}

void RstGen::
visitParamDecl(const ParamDeclPtr& p)
{
    if (!isSameFile(*p))
    {
	return;
    }

    std::string name = p->name();
    //TypePtr tp = p->type();
    //cout << tp->typeId() << " " << p->name() << ", ";
    cout << p->name() << ", ";
}

void RstGen::
describeParamDecl(const ParamDeclPtr& p)
{
    std::string inout = "in";
    std::string name = p->name();
    TypePtr tp = p->type();

    if ( p->isOutParam() )
        inout = "out";

    cout << tab() <<
        ":param " << name << " "
	 << formatType(tp->typeId())
	 << ": (" << inout << ")\n\n";

    _tabCount++;

    genMetadata(*p);
    genStrings(*p);

    --_tabCount;
}

void RstGen::
visitDataMember(const DataMemberPtr& p)
{
    if (!isSameFile(*p))
    {
	return;
    }
    //_tabCount++;

    TypePtr tp = p->type();

    cout << tab() << ".. attribute:: " << p->name() << "\n\n";

    _tabCount++;
    cout << tab() << ":type " << p->name() << ": " << tp->typeId() << "\n\n";

    genBody(*p);

    --_tabCount;

    cout << tab() << ".. debug: end dataMember\n\n";

    //--_tabCount;
}

void RstGen::
visitSequence(const SequencePtr& p)
{
    if (!isSameFile(*p))
    {
	return;
    }
    //_tabCount++;

    TypePtr tp = p->type();

    cout << tab() << ".. class:: " << p->name() << "\n\n";

    _tabCount++;

    cout << tab() << "Sequence type: " << tp->typeId() << "\n";
    genBody(*p);

    --_tabCount;

    cout << tab() << ".. debug: end sequence\n\n";

    //--_tabCount;
}

void RstGen::
visitDictionary(const DictionaryPtr& p)
{
    if (!isSameFile(*p))
    {
	return;
    }
    //_tabCount++;

    TypePtr keyType = p->keyType();
    TypePtr valueType = p->valueType();

    cout << tab() << ".. class:: " << p->name() << "\n\n";

    _tabCount++;

    cout << tab() << "Dictionary keytype: " << keyType->typeId()
         << " value-type: " << valueType->typeId() << "\n";
    genBody(*p);

    --_tabCount;

    cout << tab() << ".. debug: end dictionary\n\n";

    //--_tabCount;
}

void RstGen::
visitEnum(const EnumPtr& p)
{
    if (!isSameFile(*p))
    {
	return;
    }
    //_tabCount++;

    cout << tab() << ".. class:: " << p->name() << "\n\n";

    _tabCount++;

    EnumeratorList el = p->getEnumerators();

    for ( EnumeratorList::const_iterator i=el.begin(); i!=el.end(); i++)
    {
        cout << tab() << ".. attribute: " << (*i)->name() << "\n";
    }

    genBody(*p);

    --_tabCount;

    cout << tab() << ".. debug: end enum\n\n";

    //--_tabCount;
}

void RstGen::
visitConst(const ConstPtr&)
{
    cout << "Constant\n\n";
}

std::string RstGen::
tab()
{
    return std::string(_tabCount * _tabSize, ' ');
}

void RstGen::
genBody(const Slice::Contained& c)
{
    std::stringstream ss(c.comment());
    std::string line;
    while (std::getline(ss, line, '\n')) {
        cout << tab() << line << "\n";
    }

    cout << "\n";

    cout << tab() << "slice2rst debug info::\n\n";

    ++_tabCount;
    genMetadata(c);
    genStrings(c);
    --_tabCount;
}

void RstGen::
genMetadata(const Slice::Contained& c)
{
    std::list<std::string> metadata = c.getMetaData();

    //cout << tab() << "\n"
    // << tab() << "metadata::\n\n";

    //++_tabCount;
    for (std::list<std::string>::const_iterator i = metadata.begin();
          i != metadata.end(); ++i)
    {
        cout << tab() << "metadata: " << *i << "\n";
    }
    //--_tabCount;
}

void RstGen::
genStrings(const Slice::Contained& c)
{
    //cout << tab() << "\n"
    // << tab() << "strings::\n\n";

    //++_tabCount;
    cout
        << tab() << "name: " << c.name() << "\n"
        << tab() << "scopedname: " << c.scoped() << "\n"
        << tab() << "scope: " << c.scope() << "\n"
        << tab() << "flattenedScope: " << c.flattenedScope() << "\n"
        << tab() << "file: " << c.file() << "\n"
        << tab() << "line: " << c.line() << "\n\n";
    //--_tabCount;
    //--_tabCount;
}

bool RstGen::
isSameFile(const Slice::Contained& c)
{
    return c.file() == _file;
}

std::string RstGen::
formatType(std::string s)
{
    std::size_t p = 0;

    while (true)
    {
	p = s.find("::");
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
	    //s.replace(p, p + 2, ".");
	    s = s.substr(0, p) + "." + s.substr(p + 2);
	}
    }
    return s;
}

