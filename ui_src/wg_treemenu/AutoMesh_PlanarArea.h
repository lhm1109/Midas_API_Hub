#ifndef __MESH_AUTOMESH_PLANARAREA_H__
#define __MESH_AUTOMESH_PLANARAREA_H__

#include "..\wg_db\mesh_packet_2d.h"
#include "..\wg_db\MeshUtil.h"
#include "..\wg_db\UserDefType.h"
//#include "Mesh_ShapeSizeInfo.h"
//#include "BaseMeshMap.h"

enum 
{
	AUTOMESH_1D,
	AUTOMESH_2D,
	AUTOMESH_3D,
	AUTOMESH_PLANARAREA,
	AUTOMESH_EDGEAREA,
};

struct AUTOMESH_ARGUMENTS
{
	int bEditMode, bAllUpdateMode;
	int bMergeNode;
	int nMethod;
	int bOffset, bRelax, bInnerDomain, bIncInterEdge;
	int bIndividualReg;  
	int nPropID;
	int nElemOrder;  
	int nIndividual;
	int nthStatus;
	int bMatchFace;
	int bNoClamped;
	int bPostRemesh;
	int bAlignSeed;
	int bIgnoreMatchEdge;
	int bCleanup;  
	int Mesher,MesherType;

	double dRefineFactor;
	double dCollapseThreshold;  

	int nInteriorNode;
	int nInteriorEdge;
	
	MESHSIZEINFO theSizeInfo;
	
	AUTOMESH_ARGUMENTS()
	{
		bEditMode          = 0;
		bMergeNode         = 0;
		bAllUpdateMode     = 0;
		nMethod            = AUTOMESH_PLANARAREA;		
		Mesher             = 1;				//delaunay
		MesherType         = 2;		    //triangle
		bRelax             = bInnerDomain = bIncInterEdge = 0;
		bOffset            = 1;
		nPropID            = 0;
		nElemOrder         = 1;
		nIndividual        = 0;
		nthStatus          = 0;
		bIndividualReg     = 0;
		bMatchFace         = 1;
		bNoClamped         = 0;
		dCollapseThreshold = 0.1;
		bPostRemesh        = 0;
		bAlignSeed         = 0;
		bIgnoreMatchEdge   = 0;
		bCleanup           = 0;
		nInteriorNode      = 0;
		nInteriorEdge      = 0; //0=None, 1=Auto, 2=User    
	}
};

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CAutoMesh_PlanarArea 
{
	private:
		BOOL                             _bMatchWorkPlaneNormal;
		T_POINT3D                        _dirWP;

	protected:    
		//CMeshMapOfShapeSize              _edgesSizeMap;
		CMap<int,int,CArray<double, double>*,CArray<double, double>*> EdgeRatioArray;

		int    _Mesher;
		int    _MeshType;
		int    _nPropertyID;
		BOOL   _bOffset, _bRelax, _bInnerDomain, _bIncInterEdge;
		double _dRefinementFactor;
		double _minMeshSize;
		double _minEdgeSize;
		double _mergeTol;

		T_MESH_ELEM_D_LIST _AllElems;
		T_MESH_NODE_D_LIST _AllNodes;
											 
		//TopoDS_Compound    _EdgeComp, _VertexComp;
		//TopoDS_Shape       _currentShape;

		//CMeshShapeSizeInfo _shapeSizeInfo;
		MESHSIZEINFO      _lastInfo;

		//int _nInteriorNode;
		//int _nInteriorEdge;


		//CMesh_DataMapOfEdgeSequenceOfPnt     _MadeEdgePntArray;
		//CMesh_DataMapOfEdgeSequenceOfReal    _MadeEdgeRatioArray;
		//CMesh_DataMapOfEdgeSequenceOfInteger _dmMeshedEdgeNodeKArray;
		//TopTools_MapOfShape               _ignoredEdges;
		    
		int _nSinglyDomain, _nIth;

		//CMesh_DataMapOfShapeArrayIntPair     _dmFaceElemInfo;
		//CMesh_DataMapOfShapeArrayIntPair     _dmEdgeElemInfo;
		CArray<int,int> _seqBdyNodeID;

		void DeleteEdgeMap();

	public:
		CAutoMesh_PlanarArea(){}

		int AutoMesh_PlanarArea(CArray<T_LINE_G,T_LINE_G&>& aEdge,  
														 CArray<T_POINT3D,T_POINT3D&>& aSelectNode,
			                       const AUTOMESH_ARGUMENTS& AutoMeshArg,
											       const int NbIndividual,
											       const int nIth, 
														 CArray<int,int>& aDomainElemCount,
														 double &dMinMeshSize,
														 CArray<double,double&>& aSeedSize,
														 CArray<T_POLYLINE3D*,T_POLYLINE3D*>& rBoundaryList);

		virtual ~CAutoMesh_PlanarArea();
		virtual CArray<T_MESH_ELEM_D,T_MESH_ELEM_D&>& AllElems() {return _AllElems;}
		virtual CArray<T_MESH_NODE_D,T_MESH_NODE_D&>& AllNodes() {return _AllNodes;}
		//virtual CMesh_DataMapOfEdgeSequenceOfPnt&  MadeEdgePntArray()   {return _MadeEdgePntArray;}
		//virtual CMesh_DataMapOfEdgeSequenceOfReal& MadeEdgeRatioArray() {return _MadeEdgeRatioArray;}

		//virtual TopTools_MapOfShape& IgnoredEdge() { return _ignoredEdges;}

		//virtual BOOL SetShapeElemSide(const CMap<UINT, UINT, T_NODE_K, T_NODE_K>& mNodeK);
		//virtual CMesh_DataMapOfShapeArrayIntPair& FaceElemInfo() {return _dmFaceElemInfo;}
		//virtual CMesh_DataMapOfShapeArrayIntPair& EdgeElemInfo() {return _dmEdgeElemInfo;}
		//virtual double GetMergeTol() const {return _mergeTol;}		
		//virtual void OutputMsg();    

		BOOL AutoMeshGeneration(CArray<T_LINE_G,T_LINE_G&>& aEdge, CArray<T_POINT3D,T_POINT3D&>& aSelectNode, CArray<int,int>& aDomainElemCount, int& nChangeMethod, CArray<double,double&>& aSeedSize, CArray<T_POLYLINE3D*,T_POLYLINE3D*>& rBoundaryList);
		double GetMinMeshSize();

		//BOOL checkShape(const TopoDS_Shape& aShape);
		void SetBdyNode(const CMap<UINT, UINT, UINT, UINT>& mMergedInd,
			              const CMap<UINT, UINT, UINT, UINT>& mMovedInd);

		//BOOL SetEdgeNodeIndexSeq();
		//BOOL ChangeToHighOrderElem();

		
		//--------------------------------------
		// static functions

		static void build_2DPnts(CArray<T_LINE_G, T_LINE_G&>& aEdge,
														 CArray<T_POINT3D,T_POINT3D&>& aSelectNode,														 
														 CMap<int,int,CArray<double, double>*,CArray<double, double>*>& EdgeRatioArray,
			                       CArray<T_POINT2D,T_POINT2D&>& AllPnts, 
														 CArray<T_POINT2D,T_POINT2D&>& VtxPnts,  
														 CArray<double, double&>&      seqVtxSize
														 //CMesh_DataMapOfEdgeSequenceOfPnt&        EdgePntArray
														 );

		static void Construct_MeshPacket2D(mesh_packet_2d::Mesh_Packet_2D&     mp,
			                                 CMap<int,int,CArray<double, double>*,CArray<double, double>*>& EdgeRatioArray,
																			 const CArray<T_POINT2D,T_POINT2D&>& AllPnt2d,
																			 const CArray<T_POINT2D,T_POINT2D&>& VtxPnts, 
																			 const CArray<double,double&>&       seqVtxSize
																			 );
};
#include "HeaderPost.h"

#endif
