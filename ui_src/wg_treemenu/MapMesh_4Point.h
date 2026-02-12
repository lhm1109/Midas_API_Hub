// MapMesh_4Point.h: interface for the CMapMesh_4Point class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__MAPMESH_4POINT_H__)
#define __MAPMESH_4POINT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\wg_db\MeshUtil.h"

enum MESH_TYPE
{
	TYPE_TRIANGLE,
	TYPE_QUAD
};

struct XYZ
{
	double xyz[3];
};
//////////////////////////////////////////////////////////////////////
struct ELEM
{
	int Connect[4];
};

//////////////////////////////////////////////////////////////////////
struct MESH4N_ARGUMENT
{
	XYZ    Pnt[4];
	int    d_u_division;
	int    d_v_division;
	double d_u_bias;
	double d_v_bias;
	BOOL   bHighOrder;

	int    n_MeshType;
	int    nMatlK;
	int    nPropK;
};

//////////////////////////////////////////////////////////////////////
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CMapMesh_4Point  
{
public:
	CMapMesh_4Point();
	CMapMesh_4Point(const MESH4N_ARGUMENT& Arg);
	virtual ~CMapMesh_4Point();

	void Init(const MESH4N_ARGUMENT& Arg);
	BOOL Perform();

	T_MESH_NODE_D_LIST& GetNodeData() { return m_aNodeD; }
	T_MESH_ELEM_D_LIST& GetElemData() { return m_aElemD; }
	int GetNodeData(T_MESH_NODE_D_LIST& arMeshNodeD);
	int GetElemData(T_MESH_ELEM_D_LIST& arMeshElemD);
	BOOL ChangeToHighOrderElem();
	
protected:
	XYZ    m_Pnt[4];
	int    m_d_u_division;
	int    m_d_v_division;
	double m_d_u_bias;
	double m_d_v_bias;
	int    n_MeshType;
	BOOL   m_bHighOrder;

	T_MESH_NODE_D_LIST m_aNodeD;
	T_MESH_ELEM_D_LIST m_aElemD;
	int    m_nMatlK;
	int    m_nPropK;
//	XYZ* m_Node;
//	ELEM* m_Connect;
};
#include "HeaderPost.h"


#endif // !defined(__MAPMESH_4POINT_H__)
