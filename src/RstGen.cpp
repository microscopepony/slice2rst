#include <vector>
#include "RstGen.hpp"

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
	cout << tab() << "<unit file=\"" << p->currentFile() << "\">\n";

	return true;
}

void RstGen::
visitUnitEnd(const UnitPtr&)
{
	cout << tab() << "</unit>\n";
}

bool RstGen::
visitModuleStart(const ModulePtr& p)
{
	tabCount_++;

	cout << tab() << "<module name=\"" << p->name() << "\">\n";

	tabCount_++;
	std::list<std::string> metadata = p->getMetaData();
	genMetadata(metadata);
	--tabCount_;

	return true;
}

void RstGen::
visitModuleEnd(const ModulePtr& p)
{
	cout << tab() << "</module>\n";
	--tabCount_;
}

void RstGen::
visitClassDecl(const ClassDeclPtr& p)
{
}

bool RstGen::
visitClassDefStart(const ClassDefPtr& p)
{
	tabCount_++;

	cout << tab() << "<interface name=\"" << p->name() << "\">\n";

	tabCount_++;
	std::list<std::string> metadata = p->getMetaData();
	genMetadata(metadata);
	--tabCount_;

	return true;
}

void RstGen::
visitClassDefEnd(const ClassDefPtr& p)
{
	//printf("visitClassDefEnd: name = (%s)\n", p->name().c_str());

	cout << tab() << "</interface>\n";

	--tabCount_;
}

bool RstGen::
visitExceptionStart(const ExceptionPtr& p)
{
	tabCount_++;

	cout << tab() << "<exception name=\"" << p->name() << "\">\n";

	tabCount_++;
	std::list<std::string> metadata = p->getMetaData();
	genMetadata(metadata);
	--tabCount_;

	return true;
}

void RstGen::
visitExceptionEnd(const ExceptionPtr& p)
{
	cout << tab() << "</exception>\n";

	--tabCount_;
}

bool RstGen::
visitStructStart(const StructPtr& p)
{
	tabCount_++;

	cout << tab() << "<struct name=\"" << p->name() << "\">\n";

	tabCount_++;
	std::list<std::string> metadata = p->getMetaData();
	genMetadata(metadata);
	--tabCount_;

	return true;
}

void RstGen::
visitStructEnd(const StructPtr& p)
{
	cout << tab() << "</struct>\n";

	--tabCount_;
}

void RstGen::
visitOperation(const OperationPtr& p)
{
	std::string rtp_s = "void";
	TypePtr rtp = p->returnType();

	tabCount_++;

	//
	// If the return type ptr is null, return type is void.
	//
	if ( rtp )
		rtp_s = rtp->typeId();

	cout << tab() << "<operation name=\"" << p->name() << "\"" <<
		" returnType=\"" << rtp_s <<
		"\">\n";


	tabCount_++;

	std::list<std::string> metadata = p->getMetaData();
	genMetadata(metadata);

	vector<string> params;
	ParamDeclList paramList = p->parameters();

    for(ParamDeclList::const_iterator q = paramList.begin();
		q != paramList.end(); ++q)
    {
		visitParamDecl(*q);
	}

	--tabCount_;

	cout << tab() << "</operation>\n";

	--tabCount_;
}

void RstGen::
visitParamDecl(const ParamDeclPtr& p)
{
	std::string inout = "in";
	std::string name = p->name();
	TypePtr tp = p->type();

	if ( p->isOutParam() )
		inout = "out";

	cout << tab() <<
		"<parameter name=\"" << name << "\" type=\"" << tp->typeId() <<
		"\" inout=\"" << inout << "\"" <<
		">\n";

	tabCount_++;

	std::list<std::string> metadata = p->getMetaData();
	genMetadata(metadata);

	--tabCount_;

	cout << tab() << "</parameter>\n";
}

void RstGen::
visitDataMember(const DataMemberPtr& p)
{
	tabCount_++;

	TypePtr tp = p->type();

	cout << tab() <<
		"<dataMember name=\"" << p->name() << "\" type=\"" << tp->typeId() <<
			"\">\n";

	tabCount_++;

	std::list<std::string> metadata = p->getMetaData();
	genMetadata(metadata);

	--tabCount_;

	cout << tab() << "</dataMember>\n";

	--tabCount_;
}

void RstGen::
visitSequence(const SequencePtr& p)
{
	tabCount_++;

	TypePtr tp = p->type();

	cout << tab() <<
		"<sequence name=\"" << p->name() << "\" type=\"" << tp->typeId() <<
			"\">\n";

	tabCount_++;
	std::list<std::string> metadata = p->getMetaData();
	genMetadata(metadata);
	--tabCount_;
	
	cout << tab() << "</sequence>\n";

	--tabCount_;
}

void RstGen::
visitDictionary(const DictionaryPtr& p)
{
	tabCount_++;

	TypePtr keyType = p->keyType();
	TypePtr valueType = p->valueType();

	cout << tab() <<
		"<dictionary name=\"" <<
			p->name() << "\" keyType=\"" << keyType->typeId() <<
			"\" valueType=\"" << valueType->typeId() <<
			"\">\n";

	tabCount_++;

	std::list<std::string> metadata = p->getMetaData();
	genMetadata(metadata);

	--tabCount_;

	cout << tab() << "</dictionary>\n";

	--tabCount_;
}

void RstGen::
visitEnum(const EnumPtr& p)
{
	tabCount_++;

	cout << tab() << "<enum name=\"" << p->name() << "\">\n";

	tabCount_++;
	std::list<std::string> metadata = p->getMetaData();
	genMetadata(metadata);

	EnumeratorList el = p->getEnumerators();

	for ( EnumeratorList::const_iterator i=el.begin(); i!=el.end(); i++)
	{
		cout << tab() << "<enumerator name=\"" <<
			(*i)->name() << "\"/>\n";
	}

	--tabCount_;

	cout << tab() << "</enum>\n";

	--tabCount_;
}

void RstGen::
visitConst(const ConstPtr&)
{
	printf("visitConst\n");
}

std::string RstGen::
tab()
{
	return std::string(tabCount_*tabSize_, ' ');
}

void RstGen::
genMetadata(const std::list<std::string>& metadata)
{
	for ( std::list<std::string>::const_iterator i = metadata.begin();
		i != metadata.end(); ++i)
	{
		//cout << tab() << "<metadata \"" << *i << "\"/>\n";
		cout << tab() << "<metadata " << *i << "/>\n";
	}
}
