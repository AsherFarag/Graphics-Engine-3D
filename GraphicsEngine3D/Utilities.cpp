#include "Utilities.h"

void DebugPrint( const aiNode* Node, int Indent )
{
	std::string String( Indent, ' ' );
	String += Node->mName.C_Str();
	LOG_NO_TIME( Default, String.c_str() );

	for ( int i = 0; i < Node->mNumChildren; ++i )
	{
		DebugPrint( Node->mChildren[ i ], Indent + 1 );
	}
}
