#include "stdafx.h"
#include "wg_db.h"
#include "MeshUtil.h"


void T_MESH_NODE_D::Initialize() 
{ 
	x = y = z = 0.0; 
	CordK = 1;
	nNo = 0;
	bPost = FALSE;
	bHard = FALSE;
}

void T_MESH_ELEM_D::Initialize()
{
	vtktyp = 0;
	eltyp = 0;
	nKind = 0; // v1006
	elmat = 0;
	elpro = 0;
	nNo = 0;
	aNodeK.RemoveAll();
	aInfoK.RemoveAll(); // v1003
	CordK = 1;
	bPost = FALSE;
}

CMeshUtil::CMeshUtil()
{
}

CMeshUtil::~CMeshUtil()
{
}

BOOL CMeshUtil::GetMidNode(const T_MESH_NODE_D& Node1, const T_MESH_NODE_D& Node2, T_MESH_NODE_D& MidNode)
{
	MidNode.x = ( Node1.x + Node2.x ) *0.5;
	MidNode.y = ( Node1.y + Node2.y ) *0.5;
	MidNode.z = ( Node1.z + Node2.z ) *0.5;
	return TRUE;
}