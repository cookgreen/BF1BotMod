#include "Frosbite.h"

bool fb::ClientSoldierEntity::GetBonePos(int BoneId, D3DXVECTOR3* vOut)
{
	BoneCollisionComponent* pBoneCollisionComponent = this->bonecollisioncomponent;
	if (!ValidPointer(pBoneCollisionComponent)) return false;

	if (!pBoneCollisionComponent->m_ragdollTransforms.m_ValidTransforms) return false;

	fb::UpdatePoseResultData::QuatTransform* pQuat = pBoneCollisionComponent->m_ragdollTransforms.m_ActiveWorldTransforms;
	if (!ValidPointer(pQuat)) return false;

	vOut->x = pQuat[BoneId].m_TransAndScale.x;
	vOut->y = pQuat[BoneId].m_TransAndScale.y;
	vOut->z = pQuat[BoneId].m_TransAndScale.z;
	//vOut->w = pQuat[BoneId].m_TransAndScale.w;

	return true;
}