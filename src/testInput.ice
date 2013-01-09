#ifndef _TRY_ICE_
#define _TRY_ICE_

#include <Ice/BuiltinSequences.ice>

["mod1Metadata"] module One
{

	["enumMetadata"] enum TheEnum
	{
		TheEnumOne,
		TheEnumTwo,
		TheEnumThree,
	};


	["structMetadata"] struct S
	{
		["SMetadata"] int i;
		float f;
	};

	["seqMetadata"] sequence<S> sS;
	["seqDictMetadata"] dictionary<int,S> dS;


	["excMD"] exception E
	{
		["moreMD"] string reason;
	};

	["interMD"] interface IOne
	{
		["ami", "Metadata", "Metadata", "Metadata"] float method(["pOneMetadata"] int pOne, out float pTwo);
		void method2(out ["test='lkdf'"] sS something);

		void method3(Object* o);

		void method4(Ice::ByteSeq bs);
	};

	interface ITwo
	{
		void method3();
	};
};

#endif
