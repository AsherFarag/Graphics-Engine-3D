#include "RResource.h"

void RResource::ConstuctResourceInfo( const string& a_Path )
{
	SetFilePath( a_Path );

	// Get the name of the File with the file extension
	string fileNameWithExt = a_Path.substr( a_Path.find_last_of( '/' ) + 1 );
	SetResourceName( fileNameWithExt.substr( 0, fileNameWithExt.find_last_of( '.' ) ) );

	// Get the file extension
	SetFileType( a_Path.substr( a_Path.find_last_of( '.' ) ) );
}

void RResource::ConstuctResourceInfo( const string& a_Path, const string& a_ResourceName )
{
	SetFilePath( a_Path );

	SetResourceName( a_ResourceName );

	// Get the file extension
	SetFileType( a_Path.substr( a_Path.find_last_of( '.' ) ) );
}

void RResource::SetFilePath( const string& a_Path )
{
	m_FilePath = a_Path;
}

void RResource::SetResourceName( const string& a_ResourceName )
{
	m_ResourceName = a_ResourceName;
}

void RResource::SetFileType( const string& a_FileType )
{
	m_FileType = a_FileType;
}
