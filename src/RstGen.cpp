#include <vector>
#include <list>
#include <sstream>
#include "RstGen.hpp"

// See http://sphinx-doc.org/markup/desc.html for Sphinx directives

using namespace std;
using namespace Slice;

RstGen::
RstGen() :
    tabSize_(4),
    tabCount_(0)
{
}

bool RstGen::
visitUnitStart(const UnitPtr& p)
{
    //cout << tab() << "start unit: " << p->currentFile() << "\n\n";
    std::string title = "start unit: " + p->currentFile();
    std::string underline = std::string(title.size(), '=');
    cout << title << "\n"
	 << underline << "\n\n";
    return true;
}

void RstGen::
visitUnitEnd(const UnitPtr&)
{
    cout << tab() << "end unit\n\n";
}

bool RstGen::
visitModuleStart(const ModulePtr& p)
{
    //tabCount_++;

    cout << tab() << ".. module:: " << p->name() << "\n\n";
    //cout << tab() << ".. currentmodule:: " << p->name() << "\n\n";

    //tabCount_++;
    //genMetadata(*p);
    //genStrings(*p);
    //--tabCount_;

    return true;
}

void RstGen::
visitModuleEnd(const ModulePtr& p)
{
    cout << tab() << "end module\n\n";
    //--tabCount_;
}

void RstGen::
visitClassDecl(const ClassDeclPtr& p)
{
}

bool RstGen::
visitClassDefStart(const ClassDefPtr& p)
{
    //tabCount_++;

    cout << tab() << ".. class:: " << p->name() << "\n\n";

    tabCount_++;
    genMetadata(*p);
    genStrings(*p);
    --tabCount_;

    tabCount_++;
    return true;
}

void RstGen::
visitClassDefEnd(const ClassDefPtr& p)
{
    cout << tab() << "end class\n\n";

    --tabCount_;
}

bool RstGen::
visitExceptionStart(const ExceptionPtr& p)
{
    //tabCount_++;

    cout << tab() << ".. exception:: " << p->name() << "\n\n";

    tabCount_++;
    genMetadata(*p);
    genStrings(*p);
    --tabCount_;

    tabCount_++;
    return true;
}

void RstGen::
visitExceptionEnd(const ExceptionPtr& p)
{
    cout << tab() << "end exception\n\n";

    --tabCount_;
}

bool RstGen::
visitStructStart(const StructPtr& p)
{
    //tabCount_++;

    cout << tab() << ".. class:: " << p->name() << "\n\n";

    tabCount_++;
    genMetadata(*p);
    genStrings(*p);
    --tabCount_;

    tabCount_++;
    return true;
}

void RstGen::
visitStructEnd(const StructPtr& p)
{
    cout << tab() << "end struct\n\n";

    --tabCount_;
}

void RstGen::
visitOperation(const OperationPtr& p)
{
    std::string rtp_s = "void";
    TypePtr rtp = p->returnType();

    //tabCount_++;

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

    tabCount_++;

    cout << tab() << ":rtype: " << rtp_s << "\n\n";

    genMetadata(*p);
    genStrings(*p);

    --tabCount_;

    cout << tab() << "end operation\n\n";

    //--tabCount_;
}

void RstGen::
visitParamDecl(const ParamDeclPtr& p)
{
    std::string inout = "in";
    std::string name = p->name();
    TypePtr tp = p->type();

    if ( p->isOutParam() )
        inout = "out";

    /*
    cout << tab() <<
        "<parameter name=\"" << name << "\" type=\"" << tp->typeId() <<
        "\" inout=\"" << inout << "\"" <<
        ">\n";

    tabCount_++;

    genMetadata(*p);
    genStrings(*p);

    --tabCount_;

    cout << tab() << "</parameter>\n";
    */
    cout << tp->typeId() << " " << p->name() << ", ";
}

void RstGen::
visitDataMember(const DataMemberPtr& p)
{
    //tabCount_++;

    TypePtr tp = p->type();

    cout << tab() << ".. attribute:: " << p->name() << "\n\n";

    tabCount_++;
    cout << tab() << ":type " << p->name() << ": " << tp->typeId() << "\n";

    genMetadata(*p);
    genStrings(*p);

    --tabCount_;

    cout << tab() << "end dataMember\n\n";

    //--tabCount_;
}

void RstGen::
visitSequence(const SequencePtr& p)
{
    //tabCount_++;

    TypePtr tp = p->type();

    cout << tab() << ".. class:: " << p->name() << "\n\n";

    tabCount_++;

    cout << tab() << "Sequence type: " << tp->typeId() << "\n";
    genMetadata(*p);
    genStrings(*p);

    --tabCount_;

    cout << tab() << "end sequence\n\n";

    //--tabCount_;
}

void RstGen::
visitDictionary(const DictionaryPtr& p)
{
    //tabCount_++;

    TypePtr keyType = p->keyType();
    TypePtr valueType = p->valueType();

    cout << tab() << ".. class:: " << p->name() << "\n\n";

    tabCount_++;

    cout << tab() << "Dictionary keytype: " << keyType->typeId()
         << " value-type: " << valueType->typeId() << "\n";
    genMetadata(*p);
    genStrings(*p);

    --tabCount_;

    cout << tab() << "end dictionary\n\n";

    //--tabCount_;
}

void RstGen::
visitEnum(const EnumPtr& p)
{
    //tabCount_++;

    cout << tab() << ".. class:: " << p->name() << "\n\n";

    tabCount_++;

    EnumeratorList el = p->getEnumerators();

    for ( EnumeratorList::const_iterator i=el.begin(); i!=el.end(); i++)
    {
        cout << tab() << ".. attribute: " << (*i)->name() << "\n";
    }

    genMetadata(*p);
    genStrings(*p);

    --tabCount_;

    cout << tab() << "end enum\n\n";

    //--tabCount_;
}

void RstGen::
visitConst(const ConstPtr&)
{
    cout << "Constant\n\n";
}

std::string RstGen::
tab()
{
    return std::string(tabCount_ * tabSize_, ' ');
}

void RstGen::
genMetadata(const Slice::Contained& c)
{
    std::list<std::string> metadata = c.getMetaData();

    cout << tab() << "\n"
	 << tab() << "metadata::\n\n";

    ++tabCount_;
    for (std::list<std::string>::const_iterator i = metadata.begin();
          i != metadata.end(); ++i)
    {
        cout << tab() << *i << "\n";
    }
    --tabCount_;
}

void RstGen::
genStrings(const Slice::Contained& c)
{
    cout << tab() << "\n"
	 << tab() << "strings::\n\n";

    ++tabCount_;
    cout
        << tab() << "name: " << c.name() << "\n"
        << tab() << "scopedname: " << c.scoped() << "\n"
        << tab() << "scope: " << c.scope() << "\n"
        << tab() << "flattenedScope: " << c.flattenedScope() << "\n"
        << tab() << "file: " << c.file() << "\n"
        << tab() << "line: " << c.line() << "\n\n";
    --tabCount_;

    //cout << tab() << "comment:\n\n";

    // Indent each line of the comment
    std::stringstream ss(c.comment());
    std::string line;
    while (std::getline(ss, line, '\n')) {
        cout << tab() << line << "\n";
    }

    cout << "\n";

    //--tabCount_;
}

