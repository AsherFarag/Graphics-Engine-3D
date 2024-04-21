#include "RResource.h"

void RResource::ConstuctResourceInfo( const string& a_Path )
{
	m_FilePath = a_Path;

	// Get the name of the File
	m_ResourceName = a_Path.substr( a_Path.find_last_of( '/' ) + 1 );
	// Subtract the file extention from the Resource Name
	m_ResourceName = m_ResourceName.substr( 0, m_ResourceName.find_last_of( '.' ) );

	// Get the file extension
	m_FileType = a_Path.substr( a_Path.find_last_of( '.' ) );
}
