#pragma once
#include "Animation.h"
#include "RMesh.h"

struct BoneAnimation
{
	PositionAnimationTrack PositionTrack;
	RotationAnimationTrack RotationTrack;
	ScaleAnimationTrack ScaleTrack;
	BoneAnimation( const aiNodeAnim* a_NodeAnim = nullptr, const TimeType a_InverseTicksPerSecond = (TimeType)-1.0);
	void ExtractKeys( const aiNodeAnim* a_NodeAnim, const TimeType a_InverseTicksPerSecond );
	void Evaluate( mat4& o_Transform, const TimeType a_Time );
};

struct AssimpNodeData
{
	mat4 Transformation;
	string Name;
	int ChildrenCount;
	std::vector<AssimpNodeData> Children;
};

class RSkeletalAnim :
    public RAnimation
{
	friend class AnimationLoader;
	friend class ImGui_ResourceManager;

protected:
    std::map< string, BoneAnimation > m_BoneAnimations;

	void ReadHeirarchyData( AssimpNodeData& dest, const aiNode* src );
	void ReadMissingBones( const aiAnimation* a_Animation, MeshHandle a_SkMesh );

public:
	const auto& GetRootNode() { return m_RootNode; }
	BoneAnimation* GetBone( const string& a_BoneName );
	const auto& GetBoneIDMap() { return m_BoneInfoMap; }
    bool GetBoneMatrix( const string& a_BoneName, mat4& o_BoneMatrix, TimeType a_Time );
	void BindToSkeletalMesh( const aiNode* a_Node, const aiAnimation* a_Anim, MeshHandle a_Mesh );


	AssimpNodeData m_RootNode;
	std::map< string, BoneInfo > m_BoneInfoMap;
};

using SkeletalAnimHandle = std::shared_ptr < RSkeletalAnim >;

