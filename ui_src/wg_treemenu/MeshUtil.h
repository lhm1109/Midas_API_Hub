// MeshUtil.h: interface for the CMeshUtil class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MESHUTIL_H__
#define __MESHUTIL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

#include "HeaderPre.h"
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// VTK Lib가 없으므로 임시로 옮겨옴
// Linear cells
#define VTK_EMPTY_CELL     0
#define VTK_VERTEX         1
#define VTK_POLY_VERTEX    2
#define VTK_LINE           3
#define VTK_POLY_LINE      4
#define VTK_TRIANGLE       5
#define VTK_TRIANGLE_STRIP 6
#define VTK_POLYGON        7
#define VTK_PIXEL          8
#define VTK_QUAD           9
#define VTK_TETRA         10
#define VTK_VOXEL         11
#define VTK_HEXAHEDRON    12
#define VTK_WEDGE         13
#define VTK_PYRAMID       14
#define VTK_PENTAGONAL_PRISM 15
#define VTK_HEXAGONAL_PRISM  16

// Quadratic, isoparametric cells
#define VTK_QUADRATIC_EDGE       21
#define VTK_QUADRATIC_TRIANGLE   22
#define VTK_QUADRATIC_QUAD       23
#define VTK_QUADRATIC_TETRA      24
#define VTK_QUADRATIC_HEXAHEDRON 25
#define VTK_QUADRATIC_WEDGE      27  //KYE-HONG V440 -> V500변환시 번호를 바꿔놨다. 그래서 다시 돌려놨다.  
#define VTK_QUADRATIC_PYRAMID    26

// Special class of cells formed by convex group of points
#define VTK_CONVEX_POINT_SET 41

// Higher order cells in parametric form
#define VTK_PARAMETRIC_CURVE        51
#define VTK_PARAMETRIC_SURFACE      52
#define VTK_PARAMETRIC_TRI_SURFACE  53
#define VTK_PARAMETRIC_QUAD_SURFACE 54
#define VTK_PARAMETRIC_TETRA_REGION 55
#define VTK_PARAMETRIC_HEX_REGION   56

// Higher order cells
#define VTK_HIGHER_ORDER_EDGE        60
#define VTK_HIGHER_ORDER_TRIANGLE    61
#define VTK_HIGHER_ORDER_QUAD        62
#define VTK_HIGHER_ORDER_POLYGON     63
#define VTK_HIGHER_ORDER_TETRAHEDRON 64
#define VTK_HIGHER_ORDER_WEDGE       65
#define VTK_HIGHER_ORDER_PYRAMID     66 
#define VTK_HIGHER_ORDER_HEXAHEDRON  67
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

const double   dMeshTolFactor        = 0.1; // of the minMeshSize

//////////////////////////////////////////////////////////////////////
template<class Type>
struct T_CSET_K  
{
	operator unsigned long() 
	{
		long hk = 0;
		hk |= (i1 & 0x0000ffff) << 16;
		hk |= (i2 & 0x0000ffff); 
		return hk;
	}
	BOOL operator == (const T_CSET_K& u2) const { return (i1 == u2.i1 && i2 == u2.i2); }
	void Set(Type i1, Type i2) { if(i1<i2) {this->i1 = i1; this->i2 = i2;}else{this->i1 = i2; this->i2 = i1;} }
	void Get(Type& i1, Type& i2) const { i1 = this->i1; i2 = this->i2; }
private:
	Type i1;
	Type i2;
};


#ifndef T_MESH_NODE_K
#define T_MESH_NODE_K UINT
typedef CArray<T_MESH_NODE_K, T_MESH_NODE_K> T_MESH_NODE_K_LIST;
#endif
struct __MY_EXT_CLASS__ T_MESH_NODE_D
{
	double x, y, z;
	T_KEY CordK;   // Reference Coordinate : (1001에서 추가)
	UINT  nNo;     // 절점 번호 : (1002에서 추가)
	BOOL  bPost;         // v1003 추가, Reinforcement해석시 발생되는 절점과 요소를 구분하기 위한 Flag.
											 // 읽는 과정에서는 DB에 넣지 않는다.
	BOOL  bHard;   // v1004에서 추가.

	void Initialize();
	T_MESH_NODE_D() {Initialize();} // 필요할 때마다 Initialize를 시켜야 하는 것이 더 좋은 성능을 나타낸다.
														 // 하지만, CoordKey문제를 수정하기위하여 모든 instance가 생성되는 곳마다 작업하기에는 
														 // 양이 많고 안정성을 유지하기 위해 임시로 생성자에서 Initialize를 시킨다.
														 // 생성자에서 Initialize를 하지않고 직접 필요할때마다 쓰도록 
														 // 추후에 시간을 갖고 수정하도록 한다.(Y.Jee 20040930)  
};

typedef CArray<T_MESH_NODE_D, T_MESH_NODE_D&> T_MESH_NODE_D_LIST;


struct __MY_EXT_CLASS__ T_MESH_ELEM_D
{
	int vtktyp;						// 절점 개수와 요소 형상에 따른 타입. vtkCelltype.h참조.
	int eltyp;            // ????_EL (defined in DBStructure.h) // 현재 Post에서만 사용되고 있음.
	int nKind;            // v1006추가, 0:Normal, 1:Interface Elem, 2:Reinforcement Section
	int elmat;            // Material Number // 현재 사용안함.
	int elpro;            // Section or Thickness Number
	
	UINT nNo;             // v1001 추가, 요소 번호
	T_MESH_NODE_K_LIST aNodeK;
	T_MESH_NODE_K_LIST aInfoK; // v1003 추가, interface 요소를 위해 수정추가.(20041217 Y.Jee)
	//int nAngleType;       // v1002 추가, 0=beta angle, 1=K-node, 2=vector, 3=two angle, 4=3point // ->> v1006에서 삭제
	//T_KEY  NodeK;         // v1002 추가, nAngleType==1 일때 사용           // ->> v1006에서 삭제
	//double dVector[3];    // v1002 추가, nAngleType==2 일때 사용 (x, y, z) // ->> v1006에서 삭제
	T_KEY  CordK;         // v1004 추가, FX+ for DIANA를 위한 Coordinate Key변수.
	BOOL   bPost;         // v1005 추가, Reinforcement해석시 발생되는 절점과 요소를 구분하기 위한 Flag.
												// 읽는 과정에서는 DB에 넣지 않는다.

	void Initialize();	
	T_MESH_ELEM_D() {}
	T_MESH_ELEM_D(int nType) {vtktyp = nType;}
	T_MESH_ELEM_D(const T_MESH_ELEM_D& src) { *this = src; }
	T_MESH_ELEM_D& operator=(const T_MESH_ELEM_D& src)
	{
		vtktyp = src.vtktyp;
		eltyp = src.eltyp;
		nKind = src.nKind;
		elmat = src.elmat;
		elpro = src.elpro;
		nNo   = src.nNo;
		aNodeK.Copy(src.aNodeK);
		aInfoK.Copy(src.aInfoK);
		CordK = src.CordK;
		bPost = src.bPost;
		return *this;
	}

	BOOL IsEqualElem(const T_MESH_ELEM_D& Other) // Table에서 사용되기 위하여 추가된 Method...
	{
	  if(vtktyp != Other.vtktyp) return FALSE;					
		if(eltyp  != Other.eltyp)  return FALSE;					
		if(nKind  != Other.nKind)  return FALSE;					
		if(elmat  != Other.elmat)  return FALSE;					
		if(elpro  != Other.elpro)  return FALSE;					
		if(nNo    != Other.nNo)    return FALSE;
		if(aNodeK.GetSize() != Other.aNodeK.GetSize()) return FALSE;
		if(aInfoK.GetSize() != Other.aInfoK.GetSize()) return FALSE;
		for(int i=0; i<aNodeK.GetSize(); i++) { if(aNodeK[i] != Other.aNodeK[i]) return FALSE; }
		for(    i=0; i<aInfoK.GetSize(); i++) { if(aInfoK[i] != Other.aInfoK[i]) return FALSE; }
		if(CordK  != Other.CordK)  return FALSE;
		if(bPost  != Other.bPost)  return FALSE;

		return TRUE;
	}  
};
typedef CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&> T_MESH_ELEM_D_LIST;

enum SEEDING_METHOD
{
	SEED_SIZE_METHOD,
	SEED_DIVISION_METHOD,
	//SEED_LENGTH_GRADING_METHOD,
	//SEED_RATIO_GRADING_METHOD,
	//SEED_CUSTOM_METHOD,
	//SEED_HYPER_TANGENT_METHOD
};

struct MESHSIZEINFO
{	
	int    bEmpty;
	int    nConstraintType; // as SEEDING_METHOD
	int    nDivision;
	double dSize;
	int    bAdapSeed,bSymSeed;
	int    bManualSeed; // DB에 까지는 넣을 필요없다. 필요할 때 만들기 때문.
	
	double GradingSLen;
	double GradingELen;
	int    GradingDiv;
	double GradingRatio;
	double GradingCParam;
	
	CArray<double, double> aPosRatio;
	
	MESHSIZEINFO()
	{
		bEmpty          = 1;
		nConstraintType = 0;
		dSize           = 0;
		nDivision       = 0;
		bAdapSeed       = 0;
		bSymSeed        = 0;
		GradingSLen     = 0.;
		GradingELen     = 0.;
		GradingDiv      = 0;
		GradingRatio    = 0.;
		GradingCParam   = 0.;
		bManualSeed     = 0;
	}
	MESHSIZEINFO(const MESHSIZEINFO& src) {*this = src;}
	MESHSIZEINFO& operator=(const MESHSIZEINFO& src)
	{
		bEmpty          = src.bEmpty;
		nConstraintType = src.nConstraintType;
		dSize           = src.dSize;
		nDivision       = src.nDivision;
		bAdapSeed       = src.bAdapSeed;
		bSymSeed        = src.bSymSeed;
		GradingSLen     = src.GradingSLen;
		GradingELen     = src.GradingELen;
		GradingDiv      = src.GradingDiv;
		GradingRatio    = src.GradingRatio;
		GradingCParam   = src.GradingCParam;
		bManualSeed     = src.bManualSeed;
		aPosRatio.Copy(src.aPosRatio);
		return *this;
	}
};

/////////////////////////////////////////////////////////////////////
namespace mesh_packet_2d
{
	using namespace std;
	/////////////////////////////////////////////////////////////////
	
	/////////////////////////////////////////////////////////////////
	struct Mesh_Packet_2D
	{
		enum MESH_TYPE { MT_TRIA = 1, MT_COMB, MT_QUAD };
		/////////////////////////////////////////
		struct I_Node
		{
			bool   b_hard_flag;
			bool   b_island_flag;
			double d_mesh_size;
			double a_uv[2];	
			/////////////////////////////////////
			I_Node()
			{
				b_hard_flag   = false;
				b_island_flag = false;
				d_mesh_size   = 0.0;
			}
		}; // end: struct Mesh_Packet_2D::I_Node
		/////////////////////////////////////////
		struct I_Element
		{
			vector<int> v_node_index;
		}; // end: struct Mesh_Packet_2D::I_Element 
		/////////////////////////////////////////
		struct I_Node_Chain
		{
			enum CHAIN_TYPE { CT_OUTER = 1, CT_INNER, CT_OPEN };
			/////////////////////////////////////
			int         n_tag;
			vector<int> v_tag;
			CHAIN_TYPE  t_type;
			vector<int> v_node_index;
		}; // end: struct Mesh_Packet_2D::I_Node_Chain
		/////////////////////////////////////////
		struct I_Domain
		{
			vector<int> v_node_chain_index;
			vector<int> v_island_node_index;
		}; // end: struct Mesh_Packet_2D::I_Domain
		/////////////////////////////////////////
		Mesh_Packet_2D()
		{
			d_mesh_size        = 0.0;
			t_mesh_type        = MT_QUAD;
			d_scale_factor     = 1.0;
			b_offset_flag      = true;
			b_relaxation_flag  = true;
			d_refinement_ratio = 1.0;
		}
		/////////////////////////////////////////
		double               d_mesh_size;
		MESH_TYPE            t_mesh_type;
		double               d_scale_factor;
		bool                 b_offset_flag;
		bool                 b_relaxation_flag;
		double               d_refinement_ratio;
		vector<I_Node>       v_node;
		vector<I_Element>    v_element;
		vector<I_Node_Chain> v_node_chain;
		vector<I_Domain>     v_domain;
		// data only for ortho_mesher
		vector<double>       v_uv_grid     [2];
		vector<double>       v_uv_mesh_size[2];
	}; // end: struct Mesh_Packet_2D
} // end: namespace mesh_packet_2d
/////////////////////////////////////////////////////////////////////

class __MY_EXT_CLASS__  CMeshUtil  
{
public:
	CMeshUtil();
	virtual ~CMeshUtil();

	static BOOL GetMidNode(const T_MESH_NODE_D& Node1, const T_MESH_NODE_D& Node2, T_MESH_NODE_D& MidNode);
};
#include "HeaderPost.h"

#endif // !defined(__MESHUTIL_H__)
