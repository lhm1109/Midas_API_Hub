// MapMesh_4Point.cpp: implementation of the CMapMesh_4Point class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "MapMesh_4Point.h"

//#include "..\Base\GPSElemData.h"
#include "..\wg_base\wg_base_GTF.h"
#include "..\wg_base\TypeEx.h"

#include <vector>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////
bool estimate_ratioArray_Ratio(double d_ratio, int n_division, double* ratioArray)
{
	using namespace std;
	vector<double> v_xi;
	
	if (d_ratio < MESHSIZETOL) return false;
	
	if (n_division == 1)
	{
		v_xi.push_back(0.0);
		v_xi.push_back(1.0);
		return false;
	}
	/////////////////////////////////////////////
	v_xi.push_back(0.0);
	double d_delta_ratio       = pow(d_ratio, 1.0/(n_division - 1.0));
	double d_segment_ratio     = 2.0 / (n_division * (d_ratio + 1.0));
	double d_accumulated_ratio = d_segment_ratio;
	v_xi.push_back(d_accumulated_ratio);
	/////////////////////////////////////////////
	for (int i=0; i<n_division-1; ++i)
	{
		d_segment_ratio     *= d_delta_ratio;
		d_accumulated_ratio += d_segment_ratio;
		v_xi.push_back(d_accumulated_ratio);
	}
	double d_error_ratio = 1.0 / v_xi.back();
	/////////////////////////////////////////////
	for(int i=0; i<(int)v_xi.size(); ++i) v_xi[i] *= d_error_ratio;
	v_xi.back() = 1.0;
	
	for(int i=0; i<(int)v_xi.size(); ++i) ratioArray[i] = v_xi[i];
	return true;
}
//////////////////////////////////////////////////////////////////////



CMapMesh_4Point::CMapMesh_4Point()
{}

CMapMesh_4Point::CMapMesh_4Point(const MESH4N_ARGUMENT& Arg)
{
	Init(Arg);
}

CMapMesh_4Point::~CMapMesh_4Point()
{}

//============================================================================
// Init
//============================================================================
void CMapMesh_4Point::Init(const MESH4N_ARGUMENT& Arg)
{
	m_Pnt[0] = Arg.Pnt[0];
	m_Pnt[1] = Arg.Pnt[1];
	m_Pnt[2] = Arg.Pnt[2];
	m_Pnt[3] = Arg.Pnt[3];
	m_d_u_division = Arg.d_u_division;
	m_d_v_division = Arg.d_v_division;
	m_d_u_bias = Arg.d_u_bias;
	m_d_v_bias = Arg.d_v_bias;

	n_MeshType = Arg.n_MeshType;
	m_bHighOrder = Arg.bHighOrder;
	m_nMatlK   = Arg.nMatlK;
	m_nPropK   = Arg.nPropK;
}

//============================================================================
// Perform
//============================================================================
BOOL CMapMesh_4Point::Perform()
{
	int nNode = (m_d_u_division+1)*(m_d_v_division+1);
	int nElem = m_d_u_division*m_d_v_division;
	if(n_MeshType==0) nElem *= 2;

	XYZ*  NODE     =  new XYZ[nNode];
	ELEM* Elem_Con =  new ELEM[nElem];
		
	// Calculate Bias 간격 (공비)
	double* u_BiasRatio = new double[m_d_u_division+1];
	double* v_BiasRatio = new double[m_d_v_division+1];

	if ( m_d_u_division > 1 && m_d_v_division > 1 )
	{
		estimate_ratioArray_Ratio(m_d_u_bias, m_d_u_division, u_BiasRatio);
	  estimate_ratioArray_Ratio(m_d_v_bias, m_d_v_division, v_BiasRatio);
	}
	if ( m_d_u_division == 1 && m_d_v_division > 1 )
	{
		u_BiasRatio[0] = 0.0;
		u_BiasRatio[1] = 1.0;
	  estimate_ratioArray_Ratio(m_d_v_bias, m_d_v_division, v_BiasRatio);
	}
	if ( m_d_u_division > 1 && m_d_v_division == 1 )
	{
		estimate_ratioArray_Ratio(m_d_u_bias, m_d_u_division, u_BiasRatio);
		v_BiasRatio[0] = 0.0;
		v_BiasRatio[1] = 1.0;
	}
	if ( m_d_u_division == 1 && m_d_v_division == 1 )
	{
		u_BiasRatio[0] = 0.0;
		u_BiasRatio[1] = 1.0;
		v_BiasRatio[0] = 0.0;
		v_BiasRatio[1] = 1.0;
	}
		

//아랫줄과 윗줄의 노드를 먼저 생성
	for (int i=0 ; i<m_d_u_division+1 ; i++)
	{
		for (int j=0 ; j<3 ; j++)
		{
			NODE[i].xyz[j]															    	= u_BiasRatio[i]*m_Pnt[1].xyz[j]+(1-u_BiasRatio[i])*m_Pnt[0].xyz[j];
			NODE[i+(m_d_u_division+1)*m_d_v_division].xyz[j]	= u_BiasRatio[i]*m_Pnt[2].xyz[j]+(1-u_BiasRatio[i])*m_Pnt[3].xyz[j];
		}
	}

// Mesh를 만들기 위해서 헷갈리니깐.. 왼쪽과 오른쪽 줄을 먼저 만든다.
	for (int i=1 ; i<m_d_v_division ; i++) 
	{
		for (int j=0 ; j<3 ; j++)
		{
			NODE[i*(m_d_u_division+1)].xyz[j] = v_BiasRatio[i]*m_Pnt[3].xyz[j] + (1-v_BiasRatio[i])*m_Pnt[0].xyz[j];
			NODE[(i+1)*(m_d_u_division+1)-1].xyz[j] = v_BiasRatio[i]*m_Pnt[2].xyz[j] + (1-v_BiasRatio[i])*m_Pnt[1].xyz[j];
		}
	}
		
// 중심에 있는 (u_division-1)*(v_division-1)개의 노드를 만든다.
	for (int i=1 ; i<m_d_u_division ; i++)
	{
		for (int j=0 ; j<m_d_v_division-1 ; j++) 
		{
			for (int k=0 ; k<3 ; k++)
			{
				NODE[(m_d_u_division+1)*(j+1)+i].xyz[k] = (1-v_BiasRatio[j+1])*NODE[i].xyz[k] + v_BiasRatio[j+1]*NODE[(m_d_u_division+1)*m_d_v_division+i].xyz[k];
			}
		}
	}

//노드는 다 만들었다고 가정하고....
//엘리먼트를 만든다.
//시계 반대 방향으로 돌린다. LowerLeft->LowerRight->UpperRight->UpperLeft방향으로
//	int Elem_Con[u_division*v_division+Tria_opt*u_division*v_division][4];
//	ELEM* Elem_Con =  new ELEM[u_division*v_division+Tria_opt*u_division*v_division];
	
	if (n_MeshType==0)
	{
		for (int i=1 ; i<m_d_u_division+1 ; i++)
		{
			for (int j=1 ; j<m_d_v_division+1 ; j++)
			{
				Elem_Con[2*((i-1)+m_d_u_division*(j-1))].Connect[0]= i+(j-1)*(m_d_u_division+1);
				Elem_Con[2*((i-1)+m_d_u_division*(j-1))].Connect[1]= i+(j-1)*(m_d_u_division+1)+1;
				Elem_Con[2*((i-1)+m_d_u_division*(j-1))].Connect[3] = 0;
				Elem_Con[2*((i-1)+m_d_u_division*(j-1))].Connect[2] = i+j*(m_d_u_division+1)+1;

				Elem_Con[2*((i-1)+m_d_u_division*(j-1))+1].Connect[0]= i+(j-1)*(m_d_u_division+1);
				Elem_Con[2*((i-1)+m_d_u_division*(j-1))+1].Connect[3]= 0;
				Elem_Con[2*((i-1)+m_d_u_division*(j-1))+1].Connect[2] = i+j*(m_d_u_division+1);
				Elem_Con[2*((i-1)+m_d_u_division*(j-1))+1].Connect[1] = i+j*(m_d_u_division+1)+1;
			}
		}
	}
	
	else
	{
		for (int i=1 ; i<m_d_u_division+1 ; i++)
		{
			for (int j=1 ; j<m_d_v_division+1 ; j++)
			{
				Elem_Con[(i-1)+m_d_u_division*(j-1)].Connect[0]= i+(j-1)*(m_d_u_division+1);
				Elem_Con[(i-1)+m_d_u_division*(j-1)].Connect[1]= i+(j-1)*(m_d_u_division+1)+1;
				Elem_Con[(i-1)+m_d_u_division*(j-1)].Connect[3] = i+j*(m_d_u_division+1);
				Elem_Con[(i-1)+m_d_u_division*(j-1)].Connect[2] = i+j*(m_d_u_division+1)+1;
			}
		}
	}

	m_aNodeD.RemoveAll();
	m_aElemD.RemoveAll();
	for(int i=0; i<nNode; i++)
	{
		T_MESH_NODE_D NodeD;
		NodeD.x = NODE[i].xyz[0];
		NodeD.y = NODE[i].xyz[1];
		NodeD.z = NODE[i].xyz[2];
		m_aNodeD.Add(NodeD);
	}

	T_MESH_ELEM_D ElemD;
	ElemD.Initialize();
	if(n_MeshType == 0) 
	{
		ElemD.vtktyp = VTK_TRIANGLE;
		ElemD.aNodeK.SetSize(3);
	}
	else 
	{
		ElemD.vtktyp = VTK_QUAD;
		ElemD.aNodeK.SetSize(4);
	}
	ElemD.elmat = m_nMatlK;
	ElemD.elpro = m_nPropK;
	for(int i=0; i<nElem; i++)
	{
		ElemD.aNodeK[0] = Elem_Con[i].Connect[0]-1;
		ElemD.aNodeK[1] = Elem_Con[i].Connect[1]-1;
		ElemD.aNodeK[2] = Elem_Con[i].Connect[2]-1;
		if(n_MeshType == 1)
			ElemD.aNodeK[3] = Elem_Con[i].Connect[3]-1;
		m_aElemD.Add(ElemD);
	}

	delete[] NODE;
	delete[] Elem_Con;
	delete[] u_BiasRatio;
	delete[] v_BiasRatio;

	if(m_bHighOrder && !ChangeToHighOrderElem()) return FALSE;

	return TRUE;
}


//============================================================================
// ChangeToHighOrderElem
//============================================================================
BOOL CMapMesh_4Point::ChangeToHighOrderElem()
{
	CMap<T_CSET_K<UINT>, T_CSET_K<UINT>, int, int> mNodePairKey;
	mNodePairKey.InitHashTable(GF_GetHashSizeByCount(m_aElemD.GetSize()*4));
	T_CSET_K<UINT> PairKey;
	int            nKey;
	T_NODE_K       NodeK1, NodeK2;

	// 내부의 Mid Node를 구한다.
	for(int i=0; i<m_aElemD.GetSize(); i++)
	{
		const T_MESH_ELEM_D& ElemD = m_aElemD[i];
		int nNode = ElemD.aNodeK.GetSize();
		for(int j=0; j<nNode; j++)
		{
			NodeK1 = ElemD.aNodeK[j];
			NodeK2 = ElemD.aNodeK[(j+1)%nNode];

			PairKey.Set(NodeK1, NodeK2);
			if(!mNodePairKey.Lookup(PairKey, nKey))
			{
				int nCurInd = m_aNodeD.GetSize();
				mNodePairKey.SetAt(PairKey, nCurInd);
				T_MESH_NODE_D MidNode;
				CMeshUtil::GetMidNode(m_aNodeD[NodeK1], m_aNodeD[NodeK2], MidNode);        
				m_aNodeD.Add(MidNode);
			}
		}
	}
	
	// Element를 재구성한다.
	for(int i=0; i<m_aElemD.GetSize(); i++)
	{
		T_MESH_ELEM_D& ElemD = m_aElemD[i];
		if(ElemD.vtktyp == VTK_TRIANGLE)  ElemD.vtktyp = VTK_QUADRATIC_TRIANGLE;
		else if(ElemD.vtktyp == VTK_QUAD) ElemD.vtktyp = VTK_QUADRATIC_QUAD;
		int nNode = ElemD.aNodeK.GetSize();
		for(int j=0; j<nNode; j++)
		{
			NodeK1 = ElemD.aNodeK[j];
			NodeK2 = ElemD.aNodeK[(j+1)%nNode];
			PairKey.Set(NodeK1, NodeK2);
			if(!mNodePairKey.Lookup(PairKey, nKey)) return FALSE;
			ElemD.aNodeK.Add(nKey);
		}
	}

	return TRUE;
}

//============================================================================
// Perform
//============================================================================
int CMapMesh_4Point::GetNodeData(T_MESH_NODE_D_LIST& arMeshNodeD)
{
	arMeshNodeD.RemoveAll();
	int nSize = m_aNodeD.GetSize();
	arMeshNodeD.SetSize(nSize);

	for(int i=0 ; i<nSize ; ++i)
	{
		arMeshNodeD[i] = m_aNodeD[i];
	}

	return arMeshNodeD.GetSize();
}
int CMapMesh_4Point::GetElemData(T_MESH_ELEM_D_LIST& arMeshElemD)
{
	arMeshElemD.RemoveAll();
	int nSize = m_aElemD.GetSize();
	arMeshElemD.SetSize(nSize);

	for(int i=0 ; i<nSize ; ++i)
	{
		arMeshElemD[i] = m_aElemD[i];
	}

	return arMeshElemD.GetSize();
}
