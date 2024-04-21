#include "RSkeleton.h"
#include "Gizmos.h"

RSkeleton::RSkeleton()
{
}

void RSkeleton::Draw()
{
	for ( auto& bone : m_Bones )
	{
		mat4 worldTransform = mat4(1);

		int parent = bone.Parent;
		while ( parent != NO_PARENT_INDEX )
		{
			worldTransform *= m_Bones[ parent ].BindTransform == mat4( 1 ) ? m_Bones[ parent ].BindTransform : glm::inverse( m_Bones[ parent ].BindTransform );
			parent = m_Bones[parent].Parent;
		}
		worldTransform = worldTransform == mat4( 1 ) ? worldTransform : glm::inverse( worldTransform );
		
		vec4 color{1, 0, 0, 1};
		if ( &bone - m_Bones.data() == 0 )
		{
			color = { 1,1,0,1 };
		}

		vec3 p0 = worldTransform[ 3 ];
		vec3 p1 = ( worldTransform * bone.BindTransform )[ 3 ];

		aie::Gizmos::addSphere( p1, 1, 5, 5, color );
		aie::Gizmos::addLine( p0, p1, color );
	}
}
