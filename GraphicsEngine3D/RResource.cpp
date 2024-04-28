#include "RResource.h"

void RResource::ConstuctResourceInfo( const string& a_Path )
{
	SetFilePath( a_Path );

	size_t offIndex;

	// Get the name of the File with the file extension
	string fileName = a_Path.substr( a_Path.find_last_of( '/' ) + 1 );
	offIndex = fileName.find_last_of( '.' );
	if ( offIndex < fileName.size() )
		fileName = fileName.substr( 0, offIndex );
	SetResourceName( fileName );

	offIndex = a_Path.find_last_of( '.' );
	// Get the file extension
	if ( offIndex < a_Path.size() )
		SetFileType( a_Path.substr( offIndex ) );
}

void RResource::ConstuctResourceInfo( const string& a_Path, const string& a_ResourceName )
{
	SetFilePath( a_Path );

	SetResourceName( a_ResourceName );

	// Get the file extension
	size_t offIndex = a_Path.find_last_of( '.' );
	if ( offIndex < a_Path.size() )
		SetFileType( a_Path.substr( offIndex ) );
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
