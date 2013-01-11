#ifndef _RSTGEN_H_
#define _RSTGEN_H_

#include <Slice/Parser.h>
#include <IceUtil/OutputUtil.h>

class RstGen : public Slice::ParserVisitor
{
public:
    RstGen();

    virtual bool visitUnitStart(const Slice::UnitPtr&);
    virtual void visitUnitEnd(const Slice::UnitPtr&);
    virtual bool visitModuleStart(const Slice::ModulePtr&);
    virtual void visitModuleEnd(const Slice::ModulePtr&);
    virtual void visitClassDecl(const Slice::ClassDeclPtr&);
    virtual bool visitClassDefStart(const Slice::ClassDefPtr&);
    virtual void visitClassDefEnd(const Slice::ClassDefPtr&);
    virtual bool visitExceptionStart(const Slice::ExceptionPtr&);
    virtual void visitExceptionEnd(const Slice::ExceptionPtr&);
    virtual bool visitStructStart(const Slice::StructPtr&);
    virtual void visitStructEnd(const Slice::StructPtr&);
    virtual void visitOperation(const Slice::OperationPtr&);
    virtual void visitParamDecl(const Slice::ParamDeclPtr&);
    void describeParamDecl(const Slice::ParamDeclPtr&);
    virtual void visitDataMember(const Slice::DataMemberPtr&);
    virtual void visitSequence(const Slice::SequencePtr&);
    virtual void visitDictionary(const Slice::DictionaryPtr&);
    virtual void visitEnum(const Slice::EnumPtr&);
    virtual void visitConst(const Slice::ConstPtr&);

private:
    std::string tab();
    void genBody(const Slice::Contained& c);
    void genMetadata(const Slice::Contained& c);
    void genStrings(const Slice::Contained& c);
    bool isSameFile(const Slice::Contained& c);
    std::string formatType(std::string s);

    // This should be in a separate class
    std::string formatComment(const std::string comment,
			      const std::string indent);

    int _tabSize;
    int _tabCount;

    std::string _module;
    std::string _file;

    //::IceUtilInternal::Output xmlOut_;
};

#endif
