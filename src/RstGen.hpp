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
    virtual void visitDataMember(const Slice::DataMemberPtr&);
    virtual void visitSequence(const Slice::SequencePtr&);
    virtual void visitDictionary(const Slice::DictionaryPtr&);
    virtual void visitEnum(const Slice::EnumPtr&);
    virtual void visitConst(const Slice::ConstPtr&);

private:
    std::string tab();
    void genMetadata(const std::list<std::string>& metadata);

    int tabSize_;
    int tabCount_;

    ::IceUtilInternal::Output xmlOut_;
};

#endif
