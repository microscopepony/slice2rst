#include <vector>
#include <list>
#include <sstream>
#include "LinkHandler.hpp"
#include "RstGen.hpp"
#include "SliceCommentParser.hpp"

// See http://sphinx-doc.org/markup/desc.html for Sphinx directives

using namespace std;
using namespace Slice;

RstGen::
RstGen(LinkHandler& linker):
    _linker(linker),
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

    // If the return type ptr is null, return type is void.
    TypePtr rtp = p->returnType();
    std::string rtype = "void";
    if (rtp)
    {
        rtype = rtp->typeId();
    }

    //_tabCount++;

    // TODO: Check if this is a global or class method
    cout << tab() << ".. classmethod:: " << p->name() << "(";

    ParamDeclList paramList = p->parameters();
    FunctionParams params;

    for(ParamDeclList::const_iterator q = paramList.begin();
        q != paramList.end(); ++q)
    {
        const ParamDeclPtr& d = *q;

        std::string name = d->name();
        std::string inout = "in";
        if (d->isOutParam())
        {
            inout = "out";
        }
        TypePtr tp = d->type();

        params.addAutoParam(name, tp->typeId(), inout);

        cout << name << ", ";
    }

    cout << ")\n\n";

    _tabCount++;

    genOperationBody(*p, params, rtype);

    --_tabCount;

    cout << tab() << ".. debug: end operation\n\n";

    //--_tabCount;
}

void RstGen::
visitParamDecl(const ParamDeclPtr& p)
{
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
    cout << tab() << ":type " << p->name() << ": "
	 << _linker.type(tp->typeId()) << "\n\n";

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

    cout << tab() << "Sequence type: " << _linker.type(tp->typeId()) << "\n";
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

    cout << tab() << "Dictionary keytype: " << _linker.type(keyType->typeId())
         << " value-type: " << _linker.type(valueType->typeId()) << "\n";
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
    cout << formatComment(c.comment(), tab()) << "\n";
    cout << tab() << "slice2rst debug info::\n\n";

    ++_tabCount;
    genMetadata(c);
    genStrings(c);
    --_tabCount;
}

void RstGen::
genOperationBody(const Slice::Contained& c, FunctionParams& params, std::string rtype)
{

    SliceCommentParser scp(c.comment(), _linker);
    const std::list<SliceCommentParser::TagValues>& tvs = scp.getParsedTagValues();
    std::string stag;
    std::string scomment;

    for (std::list<SliceCommentParser::TagValues>::const_iterator it = tvs.begin();
         it != tvs.end(); ++it)
    {
        if (it->tag == "@param")
        {
            params.addCommentParam(it->value1, it->value2);
        }
        else if (!it->tag.empty())
        {
            stag += tab() + scp.getTagValueText(*it) + "\n";
        }
        else
        {
            scomment += tab() + scp.getTagValueText(*it) + "\n";
        }
    }

    const std::list<FunctionParams::ParamDescription>& pds = params.combineDescriptions();
    for (std::list<FunctionParams::ParamDescription>::const_iterator it = pds.begin();
         it != pds.end(); ++it)
    {
        cout << tab() << ":param " + it->name << ": "
             << _linker.inlineLinkParser(it->description) << "\n"
             << tab() << ":type " + it->name << ": "
             << _linker.type(it->type) << "\n";
    }
    cout << tab() << ":rtype: " << _linker.type(rtype) << "\n\n";

    cout << stag << "\n"
         << scomment << "\n";

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

// The Ice parse removes empty lines... need to put them back in for some
// sphinx tags
// TODO: Move this into a separate formatting class

std::string RstGen::
formatComment(const std::string comment, const std::string indent)
{
    SliceCommentParser scp(comment, _linker);
    //scp.getParsedTagValues();
    std::string text = scp.getParsedText(indent);
    return text;
}

