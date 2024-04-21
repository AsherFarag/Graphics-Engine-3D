#include "RSkeleton.h"
#include "Gizmos.h"

RSkeleton::RSkeleton()
{
}

void RSkeleton::Draw()
{
	for ( auto& bone : m_Bones )
	{
		mat4 parentWorldTransform = mat4(1);

		int parent = bone.Parent;
		while ( parent != NO_PARENT_INDEX )
		{
			parentWorldTransform *= glm::inverse( m_Bones[ parent ].LocalTransform );
			parent = m_Bones[parent].Parent;
		}
		parentWorldTransform = glm::inverse( parentWorldTransform );
		
		vec4 color{1, 0, 0, 1};
		// If bone is the Root Bone
		if ( &bone - m_Bones.data() == 0 )
		{
			color = { 1,1,0,1 };
		}

		vec3 p0 = parentWorldTransform[ 3 ];
		vec3 p1 = ( parentWorldTransform * bone.LocalTransform )[ 3 ];

		aie::Gizmos::addSphere( p1, 0.01f, 2, 4, color );
		aie::Gizmos::addLine( p0, p1, color );
	}
}
