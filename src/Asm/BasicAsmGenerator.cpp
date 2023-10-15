#include "Asm/BasicAsmGenerator.hpp"
#include "Parse/ASTNode.hpp"


void BasicAsmGenerator::Analyze(TSourceContext& InContext)
{
	if (InContext.ASTRoot())
	{
		InContext.ASTRoot()->BuildIR(InContext.AsmBuilder());
	}
}
