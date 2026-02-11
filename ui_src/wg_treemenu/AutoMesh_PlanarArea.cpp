#include "stdafx.h"
#ifdef strcasecmp
#undef strcasecmp
#endif

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\BaseMeshTool.h"
#include "..\wg_db\Mesh_GeomUtil.h"
#include "..\wg_db\MeshUtil.h"
#include "..\wg_db\mathlib.h"
#include "..\wg_db\qsort.h"

#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_base\wg_base_Compfunc.h"
#include "..\wg_base\Profiler.h"
#include "..\wg_base\TextProgressCtrl.h"
#include "..\wg_base\MainFrmBase.h"

#include "AutoMesh_PlanarArea.h"
//#include "MainFrmBase.h"
//#include "Mesh_core.h"
//#include "Mesh_coreExport.h"
//#include "Mesh_GeomUtil.h"
//#include "Mesh_Tools.h"
//#include "OCAF_IAutoMesh.hxx"
//#include "..\Midas_Lib\GTF.h"

//#include <GCPnts_AbscissaPoint.hxx>
//#include <BRepBuilderAPI_MakeEdge.hxx>
//#include <Geom2d_Curve.hxx>
//#include <Geom2d_TrimmedCurve.hxx>
//#include <Geom2dAdaptor_Curve.hxx>

#define ESC_INTERRUPTION_RETURN(reTurn)																									\
	if (CMeshUtil::checkESC_interruption()){																							\
		if(AfxMessageBox(_LS(IDS_TM_MESH_QST_STOP_MESH), MB_YESNO|MB_ICONQUESTION) == IDYES) return reTurn;\
	}

CAutoMesh_PlanarArea::~CAutoMesh_PlanarArea()
{M_PROFILE('CAutoMesh_PlanarArea::~CAutoMesh_PlanarArea()');
	_AllElems.RemoveAll();
	_AllNodes.RemoveAll();
	DeleteEdgeMap();
	//_dmMeshedEdgeNodeKArray.Clear();
}

int CAutoMesh_PlanarArea::AutoMesh_PlanarArea(CArray<T_LINE_G,T_LINE_G&>& aEdge, 
																							 CArray<T_POINT3D,T_POINT3D&>& aSelectNode, 
																               const AUTOMESH_ARGUMENTS& AutoMeshArg,
															                 const int NbIndividual,
															                 const int nIth,
																							 CArray<int,int>& aDomainElemIndex, 
																							 double &dMinMeshSize,
																							 CArray<double,double&>& aSeedSize, 
																							 CArray<T_POLYLINE3D*,T_POLYLINE3D*>& rBoundaryList)
{M_PROFILE('BOOL CAutoMesh_PlanarArea::AutoMesh_PlanarArea()');
	//if(!checkShape(aShape)) return FALSE;	

	_Mesher       = AutoMeshArg.Mesher;
	_MeshType     = AutoMeshArg.MesherType;
	_bInnerDomain = AutoMeshArg.bInnerDomain;
	_lastInfo     = AutoMeshArg.theSizeInfo;
	//_nInteriorNode = AutoMeshArg.nInteriorNode;
	//_nInteriorEdge = AutoMeshArg.nInteriorEdge;

	_bRelax            = AutoMeshArg.bRelax;
	//if(!AutoMeshArg.bIndividualReg) _bInnerDomain = AutoMeshArg.bInnerDomain;
	//else                            _bInnerDomain = 0;
	_bIncInterEdge     = AutoMeshArg.bIncInterEdge;
	_dRefinementFactor = AutoMeshArg.dRefineFactor;
	_nPropertyID       = AutoMeshArg.nPropID;
	_nSinglyDomain     = NbIndividual;
	_nIth              = nIth+1;
	//_shapeSizeInfo.Set(CDBDoc::GetDocPoint());
	//CMECDocBase::GetCurDoc()->GetMeshEdgeManager()->Get_MeshedEdgeData(_MadeEdgePntArray);
	
	int nChangeMethod=-1;
	ESC_INTERRUPTION_RETURN(nChangeMethod);	
	rBoundaryList.RemoveAll();  
	if(!AutoMeshGeneration(aEdge, aSelectNode, aDomainElemIndex, nChangeMethod, aSeedSize, rBoundaryList))
	{
		int nDomainCount = rBoundaryList.GetSize();
		for(int i=nDomainCount-1; i>=0; i--)
		{
			if(rBoundaryList[i] != NULL)
			{
				delete rBoundaryList[i]; 
				rBoundaryList[i] = NULL;
			}
		}
		rBoundaryList.RemoveAll();
		return -1;
	}

	/*
	// element가 내부적으로 merge되어야할 때  4각형->3각형이거나 remove.
	CArray<int,int> seqRemovedElem;
	CMeshUtil::AdjustElemIncludingDupNodes(_AllElems, seqRemovedElem);

	if(!SetEdgeNodeIndexSeq()) return FALSE;
	
	// 고차요소 
	if(AutoMeshArg.nElemOrder == 2 && !ChangeToHighOrderElem()) return FALSE;
	*/

	if(nChangeMethod==1)  
		dMinMeshSize = GetMinMeshSize();
 
	return nChangeMethod;
}

void CAutoMesh_PlanarArea::DeleteEdgeMap()
{
	int nTemp;
	CArray<double, double>* pArray = NULL;
	POSITION pos = EdgeRatioArray.GetStartPosition();
	while (pos)
	{
		EdgeRatioArray.GetNextAssoc(pos, nTemp, pArray);
		if(pArray != NULL)
		{
			pArray->RemoveAll();
			delete pArray;
			pArray = NULL;
		}
	}
	EdgeRatioArray.RemoveAll();
}

double CAutoMesh_PlanarArea::GetMinMeshSize()
{
	return _minEdgeSize;
}

BOOL CAutoMesh_PlanarArea::AutoMeshGeneration(CArray<T_LINE_G,T_LINE_G&>& aEdge, CArray<T_POINT3D,T_POINT3D&>& aSelectNode, CArray<int,int>& aDomainElemCount, int& nChangeMethod, CArray<double,double&>& aSeedSize, CArray<T_POLYLINE3D*,T_POLYLINE3D*>& rBoundaryList)
{M_PROFILE('BOOL CAutoMesh_PlanarArea::AutoMeshGeneration()');
	int i,j,k;
	int NbNodes=0,Count=1;
	double dAng = 0.0;  
	CString MsgMesh;

	//CMesh_DataMapOfEdgeSequenceOfPnt  EdgePntArray;  
		
	//----------------------------------
	// edge마다 RatioSequence만들어 넣기.
	//----------------------------------
	//TopoDS_Iterator EdgeItr(_EdgeComp);
	//TColStd_SequenceOfReal aSeq;

	DeleteEdgeMap();
	
	_minEdgeSize = DBL_MAX;

	int nMeshType = _MeshType;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();  
	for(i=0; i<aEdge.GetSize(); i++)
	{	
		CArray<double,double>* tmpRatioArray = new CArray<double,double>;    
		if(aSeedSize.GetSize() > 0)
		{
			if(aSeedSize[i] == 1)
			{
				tmpRatioArray->SetSize(2);
				tmpRatioArray->SetAt(0, 0);	
				tmpRatioArray->SetAt(1, 1);
			}
			else if(!CMeshGeomUtil::make_RatioArray(*tmpRatioArray, aEdge[i], _lastInfo)) 
			{
				DeleteEdgeMap();
				return FALSE;			
			}
		}    
		else
		{
			if(!CMeshGeomUtil::make_RatioArray(*tmpRatioArray, aEdge[i], _lastInfo)) 
			{
				DeleteEdgeMap();
				return FALSE;			
			}
		}
		
		EdgeRatioArray.SetAt(i, tmpRatioArray);

		// 길이가 mesh size보다 작은 edge가 있을 경우 pure quad는 지원하지 않으므로, Quad+Tri로 변경해준다.
		if(nMeshType == D_MESHTYPE_QUAD && _lastInfo.nConstraintType == SEED_SIZE_METHOD)
		{
			double dTempLength = aEdge[i].GetLineLength();
			if(CCompFunc::CompRealTol(_lastInfo.dSize, dTempLength, MESHSIZETOL) == 1)
			{
				if(dTempLength < _minEdgeSize)
					_minEdgeSize = dTempLength;
				nMeshType = D_MESHTYPE_TRIAQUAD;
				nChangeMethod = 1;          
			}     
		}        
	}

	//----------------------------------------------
	// Confirmation of plane surface.
	//----------------------------------------------
	
// 	Handle(Geom_Plane) aPlane;
// 	if(!CMeshGeomUtil::ConfirmInPlane(aPlane, _EdgeComp, _VertexComp))
//   {
//     //MSGBOX_ERROR_RETURN(_LS(IDS_MESH___MSG369));
//     return FALSE;
//   }
	
//   T_POINT3D EdgesDir;
//   EdgesDir = (aPlane->Axis()).Direction();
//   _dirWP   = (CMECDocBase::GetCurDoc()->GetCurrentWorkPlane()->Axis()).Direction();
//   double dPlaneAng = math_lib::angle_3d(EdgesDir.x,EdgesDir.y,EdgesDir.z,_dirWP.x, dirWP.y, dirWP.z);
//   if(dPlaneAng < Precision::Approximation() || (dPlaneAng-M_PI) < Precision::Approximation()) 
//     _bMatchWorkPlaneNormal = TRUE; 
//   else _bMatchWorkPlaneNormal = FALSE; 
	
	//------------------------------------------------
	// 3D coord --> 2D coord     &    build EdgePntArray.
	//------------------------------------------------
 	CArray<T_POINT2D,T_POINT2D&> AllPnt2d, VtxPnts; 
 	CArray<double,double&>       seqVtxSize;
 	build_2DPnts(aEdge, aSelectNode, EdgeRatioArray, AllPnt2d, VtxPnts, seqVtxSize);//EdgePntArray);
	//////////////////////////////////

	ESC_INTERRUPTION_RETURN(nChangeMethod);
	//------------------------------------------------
	// Domain(Face) Construction
	//------------------------------------------------
	
	using namespace mesh_packet_2d;
	Mesh_Packet_2D mp;
	Construct_MeshPacket2D(mp, EdgeRatioArray, AllPnt2d, VtxPnts, seqVtxSize);

	
	//------------------------------------------------------------
	// set tol
	//------------------------------------------------------------	  
	_minMeshSize = DBL_MAX;
	for(i=0; i<AllPnt2d.GetSize()-1; i++)
	{
		double dist = math_lib::distance_2d(AllPnt2d[i].x,AllPnt2d[i].y,AllPnt2d[i+1].x,AllPnt2d[i+1].y);
		if(dist<MESHSIZETOL) continue;
		if(i != AllPnt2d.GetSize())
			_minMeshSize = min(_minMeshSize, dist);	
	}
	_mergeTol = _minMeshSize * dMeshTolFactor;

	//---------------------------------------------------
	// build EdgePntArray, EdgeRatioArray
	//---------------------------------------------------
	vector<int>   v_unused_node_chain_index;
	vector<int>   v_unused_node_index;
	if(!CBaseMeshTool::Seek_UnusedIndex(mp,v_unused_node_chain_index, v_unused_node_index,
																			_bInnerDomain, !_bIncInterEdge, CMeshUtil::checkESC_interruption))
	{
		DeleteEdgeMap();
		return FALSE;
	}

	//------------------------------------------------
	// get rid of unused edges & Re-naming 
	//------------------------------------------------
// 	TopTools_SequenceOfShape unUsedEdgeSeq;
// 	for(i=0; i<int(v_unused_node_chain_index.size()); i++)
// 	{
// 		const TopoDS_Edge& unusedEdge = EdgeRatioArray.FindEdge_Index(mp.v_node_chain[v_unused_node_chain_index[i]].n_tag);
// 		unUsedEdgeSeq.Append(unusedEdge);
// 	}
// 	// 위에서 index로 edge를 찾는과정이기때문에 찾는대로 Remove시키면 
// 	// 그 index뒤에있는 edge들의 index들이 달라지게된다.
// 	for(i=1;i<=unUsedEdgeSeq.Length();i++)
// 	{
// 		const TopoDS_Edge unusedEdge = TopoDS::Edge(unUsedEdgeSeq(i));
// 		_ignoredEdges.Add(unusedEdge);
// 		if(EdgeRatioArray.IsBound(unusedEdge)) EdgeRatioArray.Remove(unusedEdge);
// 		if(EdgePntArray.IsBound(unusedEdge))   EdgePntArray.Remove(unusedEdge);
// 	}
// 	_MadeEdgeRatioArray.Copy(EdgeRatioArray);
// 	_MadeEdgePntArray.Copy(EdgePntArray);
	  
	//---------------------------------------------------------------
	// Text Status Bar 
	
	CBCGPStatusBar*   p_status_bar = (CBCGPStatusBar*)((CMainFrameBase*)AfxGetMainWnd())->GetMessageBar();
	CTextProgressCtrl c_progress_bar;
	CRect             rect;
	p_status_bar->GetItemRect(0, rect);
	c_progress_bar.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH, rect, p_status_bar, 1);
	c_progress_bar.SetShowText(TRUE);
	
	c_progress_bar.SetRange(0, (int)mp.v_domain.size());
	if(_nSinglyDomain) {
		MsgMesh.Format(_LS(IDS_TM_PREPARE_MESH), _nIth, _nSinglyDomain);
		c_progress_bar.SetWindowText(MsgMesh);
	}	else c_progress_bar.SetWindowText(_LS(IDS_TM_FINALIZING_MESH));
	c_progress_bar.SetPos(0);
	//---------------------------------------------------------------!

	double dDepth = aEdge[0].p1.z;
	int nTempChangeMethod;
	//---------------------------------------------------------------!
	//------------------------------------------------
	// Meshing
	//------------------------------------------------
	for (i=0; i<(int)mp.v_domain.size(); ++i)
	{
		ESC_INTERRUPTION_RETURN(nChangeMethod);
		const Mesh_Packet_2D::I_Domain& i_domain = mp.v_domain[i];
		/////////////////////////////////////////
		vector<Mesh_Packet_2D::I_Node_Chain> v_node_chain;
		for (int j=0; j<(int)i_domain.v_node_chain_index.size();  ++j) 
			v_node_chain.push_back(mp.v_node_chain[i_domain.v_node_chain_index[j]]);
		/////////////////////////////////////////
		set<int>                       s_node_index;
		vector<Mesh_Packet_2D::I_Node> v_node;
		map<int, int>                  m_node_index;
		int                            n_index = 0;
		for (j=0; j<(int)v_node_chain.size(); ++j)
		{
			const Mesh_Packet_2D::I_Node_Chain& i_node_chain = v_node_chain[j];
			for (int k=0; k<(int)i_node_chain.v_node_index.size(); ++k) 
				s_node_index.insert(i_node_chain.v_node_index[k]);
		}
		for (j=0; j<(int)i_domain.v_island_node_index.size(); ++j) 
			s_node_index.insert(i_domain.v_island_node_index[j]);
		for (set<int>::const_iterator itr_ni=s_node_index.begin(); itr_ni!=s_node_index.end(); ++itr_ni)
		{
			const Mesh_Packet_2D::I_Node& i_node = mp.v_node[*itr_ni];
			v_node.push_back(i_node);
			m_node_index[*itr_ni] = n_index++;
		}

		///////////////////////////////////////////////////////////////////
		for (j=0; j<(int)v_node_chain.size(); ++j)
		{
			vector<int>& v_NodeIndex = v_node_chain[j].v_node_index;
			for(k=0;k<int(v_NodeIndex.size());k++)
			{
				v_NodeIndex[k]=m_node_index[v_NodeIndex[k]];
			}
		}    
				
		// Domain을 이루는 연속된 node 정보중에서 폐구간을 이루는 것만 Hoop Node로 저장한다. 
		T_POINT3D HoopNode;
		for (j=0; j<(int)v_node_chain.size(); ++j)
		{ 
			vector<int>& v_NodeIndex = v_node_chain[j].v_node_index;
			int nIndexSize = v_NodeIndex.size();
			if(nIndexSize < 4) continue;

			double dStartX = v_node[v_NodeIndex[0]].a_uv[0];
			double dStartY = v_node[v_NodeIndex[0]].a_uv[1];
			double dEndX   = v_node[v_NodeIndex[nIndexSize-1]].a_uv[0];
			double dEndY   = v_node[v_NodeIndex[nIndexSize-1]].a_uv[1];
			if(dStartX != dEndX || dStartY != dEndY) continue;

			T_POLYLINE3D* pPolyLine = new T_POLYLINE3D;              
			HoopNode.x = v_node[v_NodeIndex[0]].a_uv[0];
			HoopNode.y = v_node[v_NodeIndex[0]].a_uv[1];
			HoopNode.z = dDepth;                 
			pPolyLine->Add(HoopNode);
			double line1[2][2], line2[2][2], dTemp[2];      
			int nCross;      
			for (k=0; k<nIndexSize; k++)
			{
				if(k+2 == nIndexSize)
				{      
					line1[0][0] = v_node[v_NodeIndex[k]].a_uv[0];
					line1[0][1] = v_node[v_NodeIndex[k]].a_uv[1];  
					
					line1[1][0] = v_node[v_NodeIndex[k+1]].a_uv[0];
					line1[1][1] = v_node[v_NodeIndex[k+1]].a_uv[1];    
					
					line2[0][0] = v_node[v_NodeIndex[k+1]].a_uv[0];
					line2[0][1] = v_node[v_NodeIndex[k+1]].a_uv[1];   
					
					line2[1][0] = v_node[v_NodeIndex[0]].a_uv[0];
					line2[1][1] = v_node[v_NodeIndex[0]].a_uv[1];
					
					nCross = CMathFunc::mathLineLineCross2D(line1, line2, dTemp);
					if(nCross != 0)
					{
						HoopNode.x = v_node[v_NodeIndex[k+1]].a_uv[0];
						HoopNode.y = v_node[v_NodeIndex[k+1]].a_uv[1];
						pPolyLine->Add(HoopNode);
					}
					break;
				}
				
				line1[0][0] = v_node[v_NodeIndex[k]].a_uv[0];
				line1[0][1] = v_node[v_NodeIndex[k]].a_uv[1];  
				
				line1[1][0] = v_node[v_NodeIndex[k+1]].a_uv[0];
				line1[1][1] = v_node[v_NodeIndex[k+1]].a_uv[1];    
				
				line2[0][0] = v_node[v_NodeIndex[k+1]].a_uv[0];
				line2[0][1] = v_node[v_NodeIndex[k+1]].a_uv[1];   
				
				line2[1][0] = v_node[v_NodeIndex[k+2]].a_uv[0];
				line2[1][1] = v_node[v_NodeIndex[k+2]].a_uv[1];
				
				nCross = CMathFunc::mathLineLineCross2D(line1, line2, dTemp);
				if(nCross == 0) continue; 
				
				HoopNode.x = v_node[v_NodeIndex[k+1]].a_uv[0];
				HoopNode.y = v_node[v_NodeIndex[k+1]].a_uv[1];
				pPolyLine->Add(HoopNode);
			}      
			if(pPolyLine->GetSize() > 2)
				rBoundaryList.Add(pPolyLine);
			else
			{
				pPolyLine->RemoveAll();
				delete pPolyLine;
			}
		}
		///////////////////////////////////////////////////////////////////
		int nPrevNodeCount = _AllNodes.GetSize();
		// set BdyNodeIDs
		/*        		
		int countID=0;
		for(j=0; j<int(v_node_chain.size()); j++)
			for(int k=0; k<(int)v_node_chain[j].v_node_index.size()-1; k++, countID++)
				_seqBdyNodeID.Add(nPrevNodeCount + countID);
		*/
		
		CArray<T_MESH_ELEM_D,T_MESH_ELEM_D&> Elems;
		
		Mesh_Packet_2D mp_domain;
		mp_domain.b_offset_flag       = (nMeshType != 2 && _bOffset)? true:false; // tria case - false
		mp_domain.b_relaxation_flag   = (nMeshType != 2 && _bRelax) ? true:false;
		mp_domain.d_refinement_ratio  = _dRefinementFactor;
		mp_domain.v_node              = v_node;
		mp_domain.v_node_chain        = v_node_chain;
		 
		CArray<UINT, UINT> aFailedInd;
		// nTempChangeMethod == 0 이면 실패    
		// nTempChangeMethod == 1 이면 성공
		// nTempChangeMethod == 2 이면 Change Method 
		nTempChangeMethod = CBaseMeshTool::MeshGenerator2D(mp_domain, Elems, _Mesher, nMeshType, CMeshUtil::checkESC_interruption, aFailedInd); 
		if(nTempChangeMethod == 0)
			return FALSE;
				
		//-------------------------------------------------------------
		double dP = double(i+1) / (int)mp.v_domain.size() * 100.0;
		MsgMesh.Format(_LS(IDS_TM_PREPARE_MESH),\
			dP, i+1,(int)mp.v_domain.size());
		c_progress_bar.SetWindowText(MsgMesh);
		c_progress_bar.StepIt();
		//-------------------------------------------------------------!

		if(nTempChangeMethod == 2)
			nChangeMethod = 2;

		T_POINT3D pnt;
		for(j=0; j<int(mp_domain.v_node.size()); j++)
		{
			//const T_POINT3D& pnt = aPlane->Value(mp_domain.v_node[j].a_uv[0], mp_domain.v_node[j].a_uv[1]); 
			pnt.x = mp_domain.v_node[j].a_uv[0];
			pnt.y = mp_domain.v_node[j].a_uv[1];
			pnt.z = dDepth;
			CMeshGeomUtil::putNode(_AllNodes,pnt);
		}

//     // element의 normal방향을 workplane의 normal 방향과 맞춰준다.
//     if(_bMatchWorkPlaneNormal)
//     {
//       T_POINT3D CurElemDir;
//       for(j=0; j<Elems.GetSize(); j++)
// 		  {
//         T_POINT3D ElemPnt1, ElemPnt2, ElemPnt3;
//         ElemPnt1.x = _AllNodes[Elems[j].aNodeK[0]+nPrevNodeCount].x;
//         ElemPnt1.y = _AllNodes[Elems[j].aNodeK[0]+nPrevNodeCount].y;
//         ElemPnt1.z = _AllNodes[Elems[j].aNodeK[0]+nPrevNodeCount].z;
//         
//         ElemPnt2.x = _AllNodes[Elems[j].aNodeK[1]+nPrevNodeCount].x;
//         ElemPnt2.y = _AllNodes[Elems[j].aNodeK[1]+nPrevNodeCount].y;
//         ElemPnt2.z = _AllNodes[Elems[j].aNodeK[1]+nPrevNodeCount].z;
//         
//         ElemPnt3.x = _AllNodes[Elems[j].aNodeK[2]+nPrevNodeCount].x;
//         ElemPnt3.y = _AllNodes[Elems[j].aNodeK[2]+nPrevNodeCount].y;
//         ElemPnt3.z = _AllNodes[Elems[j].aNodeK[2]+nPrevNodeCount].z;
//         
//         double cross[2];
//         double line1[2][2];
// 	       double line2[2][2];
// 
//         T_LINE_G NodeVec1, NodeVec2;
//         if(ElemPnt1.IsSame(ElemPnt2, MESHTOL)) continue;
//         if(ElemPnt2.IsSame(ElemPnt3, MESHTOL)) continue;
//         NodeVec1.Set(ElemPnt1, ElemPnt2);
//         NodeVec2.Set(ElemPnt2, ElemPnt3);
//         if(CMathFunc::mathLineLineCross2D(line1, line2, cross)) continue;
//         CurElemDir = T_POINT3D(NodeVec1^NodeVec2);
//         break;
//       }
//       dAng = math_lib::angle_3d(CurElemDir.x,CurElemDir.y,CurElemDir.z,_dirWP.x, _dirWP.y, _dirWP.z);
//       if(dAng > M_PI_2)
//         CMeshUtil::ReverseElement(Elems);
//     }

		int nElemSize = Elems.GetSize();    
		for(j=0; j<nElemSize; j++)
		{
			T_MESH_ELEM_D elem = Elems[j];
			int nNodeCount = elem.aNodeK.GetSize();
			for(int k=0; k<nNodeCount; k++)
				elem.aNodeK[k] += nPrevNodeCount;      
			_AllElems.Add(elem);
		}
		aDomainElemCount.Add(nElemSize);
	}

	//------------------------------------
	if(_nSinglyDomain) {
		MsgMesh.Format(_LS(IDS_TM_FINALIZE_MESH), _nIth, _nSinglyDomain);
		c_progress_bar.SetWindowText(MsgMesh);
	}	else c_progress_bar.SetWindowText(_LS(IDS_TM_FINALIZING_MESH));
	
	c_progress_bar.RedrawWindow();
	//------------------------------------!


	//---------------------------------------
	// seek minimum meshsize length & merge
	//---------------------------------------
	// get Tolerance
// 	double dMinLen = RealLast();
// 	CArray<T_POINT3D,T_POINT3D&> SeqPnt;
// 	for(i=1;i<=EdgePntArray.Extent();i++)
// 	{
// 		SeqPnt=EdgePntArray.GetAt_Index(i);
// 		for(j=1;j<SeqPnt.Length();j++)
// 		{
// 			double dist = SeqPnt(j).Distance(SeqPnt(j+1));
// 			dMinLen = (dMinLen > dist)? dist:dMinLen;
// 		}
// 	}

// 	CMap<UINT, UINT, UINT, UINT> mMergedInd;
// 	CMap<UINT, UINT, UINT, UINT> mMovedInd;
// 	mMergedInd.InitHashTable(1000003);
// 	mMovedInd.InitHashTable(1000003);
// 	CBaseMeshTool::Merge2DMesh(_AllNodes,_AllElems, mMergedInd, mMovedInd, dMinLen*.1);
// 	SetBdyNode(mMergedInd, mMovedInd);

	if(nChangeMethod == -1)
		nChangeMethod = 0;
	return TRUE;
}

//===================================================
//	checkShape
//===================================================
/*
BOOL CAutoMesh_PlanarArea::checkShape(const TopoDS_Shape& aShape)
{M_PROFILE('BOOL CAutoMesh_PlanarArea::checkShape()');
	TopAbs_ShapeEnum shapeType=aShape.ShapeType();
	
	if(shapeType==TopAbs_COMPOUND)
	{
		TopoDS_Iterator itr(aShape);
		shapeType=itr.Value().ShapeType();
	}
	if(shapeType>TopAbs_EDGE)
	{
		MessageBox(NULL,_LS(IDS_MESH___MSG342),_LS(IDS_MESH___MSG484),MB_ICONWARNING);
		return FALSE;
	}
	return TRUE;
}
*/

//============================================================================
// SetBdyNode
//============================================================================
void CAutoMesh_PlanarArea::SetBdyNode(const CMap<UINT, UINT, UINT, UINT>& mMergedInd,
																			const CMap<UINT, UINT, UINT, UINT>& mMovedInd)
{M_PROFILE('void CAutoMesh_PlanarArea::SetBdyNode()');
	for(int i=_seqBdyNodeID.GetSize(); i>0; i--)
	{
		int nNodeID = _seqBdyNodeID[i];
		UINT nReplacedInd;
		if(mMergedInd.Lookup(nNodeID, nReplacedInd)) _seqBdyNodeID.RemoveAt(i);
		else if(mMovedInd.Lookup(nNodeID, nReplacedInd))
			_seqBdyNodeID[i] = nReplacedInd;
	}
}
//===================================================
//	OutputMsg
//===================================================
/*
void CAutoMesh_PlanarArea::OutputMsg()
{M_PROFILE('void CAutoMesh_PlanarArea::OutputMsg()');
	CString MsgMesh;
	MsgMesh.Format(_LS(IDS_MESH___MSG327),_AllNodes.GetSize(),_AllElems.GetSize());

	((CMainFrameBase*)AfxGetMainWnd())->OutputMessage(_LS(IDS_MESH___MSG328),0);
	((CMainFrameBase*)AfxGetMainWnd())->OutputMessage(MsgMesh,0);
	//((CMainFrameBase*)AfxGetMainWnd())->OutputMessage("======================================================",0);
}
*/

//------------------------------------------------
// 3D coord --> 2D coord
// And build EdgePntArray.
//------------------------------------------------
void CAutoMesh_PlanarArea::build_2DPnts(CArray<T_LINE_G, T_LINE_G&>& aEdge,
																				CArray<T_POINT3D,T_POINT3D&>& aSelectNode,
																				CMap<int,int,CArray<double, double>*,CArray<double, double>*>& EdgeRatioArray,																			  
																			  CArray<T_POINT2D,T_POINT2D&>& AllPnts, 
																			  CArray<T_POINT2D,T_POINT2D&>& VtxPnts, 
																			  CArray<double,double&>&       seqVtxSize
																			  //CMesh_DataMapOfEdgeSequenceOfPnt&  EdgePntArray
																				)
{M_PROFILE('void CAutoMesh_PlanarArea::build_2DPnts()');
	int       i,j;
	T_POINT2D          uv;	
	T_POINT3D          pnt;
	CArray<double, double>* pRatioArray;
	double dSize = 0.0;
	////////////////////////////////////////////////////
	// build EdgePntArray
	for(i=0; i<aEdge.GetSize(); i++)
	{
		double dLength = aEdge[i].GetLineLength();
		EdgeRatioArray.Lookup(i, pRatioArray);
		int nSeed = pRatioArray->GetSize(); 
		for(j=0;j<nSeed;j++)
		{
			aEdge[i].GetPosByRatio(pnt, pRatioArray->GetAt(j));
			uv.x = pnt.x;
			uv.y = pnt.y;      
			AllPnts.Add(uv);			
		}
	}

	////////////////////////////////////////////////////
	
	if(aSelectNode.GetSize() != 0)
	{
		for(i=0; i<aSelectNode.GetSize(); i++)
		{
			uv.x = aSelectNode[i].x;
			uv.y = aSelectNode[i].y;  
			VtxPnts.Add(uv);				

			seqVtxSize.Add(dSize);
		}
	}  
}

//==================================================================================
// construct_MeshPacket2D()
//==================================================================================
void CAutoMesh_PlanarArea::Construct_MeshPacket2D(mesh_packet_2d::Mesh_Packet_2D& mp,
																					       CMap<int,int,CArray<double, double>*,CArray<double, double>*>& EdgeRatioArray,
																					       const CArray<T_POINT2D,T_POINT2D&>& AllPnt2d, 
																					       const CArray<T_POINT2D,T_POINT2D&>& VtxPnts, 
																					       const CArray<double,double&>& seqVtxSize)
{M_PROFILE('void CAutoMesh_PlanarArea::Construct_MeshPacket2D()');
	using namespace mesh_packet_2d;
	int i;

	//--------------------------------------
	// vertex set
	// isolated node를 뒤에 놓을 경우 전체의 mesh size가 node사이즈에 지배된다.
	// 맨앞에 놓아야 grading 되는 듯한 형상의 mesh들을 얻을 수 있다.
	
	for(i=0;i<VtxPnts.GetSize();i++)
	{
		//////////////////////////////////
		Mesh_Packet_2D::I_Node i_node;
		i_node.b_hard_flag   = 1;
		i_node.b_island_flag = 1;
		i_node.d_mesh_size   = seqVtxSize[i];
		i_node.a_uv[0]       = VtxPnts[i].x;
		i_node.a_uv[1]       = VtxPnts[i].y;
		mp.v_node.push_back(i_node);
		//////////////////////////////////
	}  

	for(i=0; i<AllPnt2d.GetSize(); ++i)
	{
		Mesh_Packet_2D::I_Node i_node;
		i_node.b_hard_flag   = 1;
		i_node.b_island_flag = 0;
		i_node.d_mesh_size   = 0.0;
		i_node.a_uv[0]       = AllPnt2d[i].x;
		i_node.a_uv[1]       = AllPnt2d[i].y;
		mp.v_node.push_back(i_node);
	}

	int countNode = VtxPnts.GetSize();
	CArray<double, double>* pRatioArray;
	for(i=0; i<EdgeRatioArray.GetCount(); ++i)
	{
		Mesh_Packet_2D::I_Node_Chain i_node_chain;
		i_node_chain.n_tag=i;
		EdgeRatioArray.Lookup(i, pRatioArray);
		int nSeed = pRatioArray->GetSize(); 

		for(int j=0; j<pRatioArray->GetSize(); ++j)
			i_node_chain.v_node_index.push_back(countNode++);
		
		/////////////////////////////////////////////
		mp.v_node_chain.push_back(i_node_chain);
	}
}


//============================================================================
// SetEdgeNodeIndexSeq
//============================================================================
/*
BOOL CAutoMesh_PlanarArea::SetEdgeNodeIndexSeq()
{M_PROFILE('BOOL CAutoMesh_PlanarArea::SetEdgeNodeIndexSeq()');
	Standard_Real   dTol = Max(_mergeTol, 1.e-6);
 	TopTools_IndexedMapOfShape imEdge;
	TColgp_SequenceOfPnt       seqBdyPnt;
	TColStd_SequenceOfInteger  seqBdyNodeK;

	for(int i=1; i<=_seqBdyNodeID.Length(); i++)
	{
		T_NODE_K NodeK = _seqBdyNodeID(i);
		// temporary...
		if(NodeK >= _AllNodes.GetSize()) {ASSERT(0); continue;}

		const T_POINT3D& pnt = CMeshTools::NodeToPnt(_AllNodes[NodeK]);
		seqBdyPnt.Append(pnt);
		seqBdyNodeK.Append(NodeK);
	}

	TopExp::MapShapes(_EdgeComp, TopAbs_EDGE, imEdge);

	for(i=1; i<=imEdge.Extent(); i++)
	{
		const TopoDS_Edge& edge = TopoDS::Edge(imEdge(i));
		if(_ignoredEdges.Contains(edge)) continue;
		TColgp_SequenceOfPnt&     seqMeshedPnt = _MadeEdgePntArray.GetAt(edge);
		TColStd_SequenceOfInteger seqNodeK;
		TColgp_SequenceOfPnt      seqEdgePnt;
		if(_MadeEdgePntArray.IsHighOrder(edge))
		{
			for(int j=1; j<=seqMeshedPnt.Length(); j+=2)
				seqEdgePnt.Append(seqMeshedPnt(j));
		}
		else seqEdgePnt = seqMeshedPnt;
		
		for(int j=1; j<=seqEdgePnt.Length(); j++)
		{
			const T_POINT3D& pnt = seqEdgePnt(j);
			//거리계산에서 tol를 만족못시키는 때에 이용
			BOOL   bExist = FALSE;
			int    nMinDistInd = 0;
			double dMinDist = RealLast();
			for(int k=1; k<=seqBdyPnt.Length(); k++)
			{
				const T_POINT3D& PntOnEdge = seqBdyPnt(k);
				if(Abs(PntOnEdge.X()-pnt.X())> dTol ||
					 Abs(PntOnEdge.Y()-pnt.Y())> dTol ||
					 Abs(PntOnEdge.Z()-pnt.Z())> dTol) 
				{
					Standard_Real dTmpDist = PntOnEdge.SquareDistance(pnt);
					if( dTmpDist < dMinDist)
					{
						nMinDistInd = k;
						dMinDist = dTmpDist;
					}
					continue;
				}

				if(pnt.IsEqual(PntOnEdge, dTol))
				{
					seqNodeK.Append(seqBdyNodeK(k));
					bExist = TRUE;
					break;
				}
			}
			if(!bExist) 
			{
				if(nMinDistInd) seqNodeK.Append(seqBdyNodeK(nMinDistInd));
				else
					MSGBOX_ERROR_RETURN( CANNOT_MAKE_RELATION_MESH_SHAPE );
			}
		}

		_dmMeshedEdgeNodeKArray.Bind(edge, seqNodeK);
	}

	return TRUE;
}
*/

//============================================================================
// ChangeToHighOrderElem
// 1. 먼저 Edge상의 Mid Node먼저 구하여 Map에 넣는다.
// 2. 내부의 Mid Node를 구한다.
// 3. Element를 재구성한다.
//============================================================================
/*
BOOL CAutoMesh_PlanarArea::ChangeToHighOrderElem()
{M_PROFILE('BOOL CAutoMesh_PlanarArea::ChangeToHighOrderElem()');
	CMap<T_CSET_K<UINT>, T_CSET_K<UINT>, int, int> mNodePairKey;
	mNodePairKey.InitHashTable(GF_GetHashSizeByCount(_AllElems.GetSize()*3));
	T_CSET_K<UINT> PairKey;
	int nKey;
	UINT NodeK1;
	UINT NodeK2;
	
	T_PREFERENCE pref;
	CMECDocBase::GetCurDoc()->GetInitCtrl()->GetPreference(pref);

	// 1. 먼저 Edge상의 Mid Node먼저 구하여 Map에 넣는다.
	int nEdge = _dmMeshedEdgeNodeKArray.Extent();
	for(int i=1; i<=nEdge; i++)
	{
		const TopoDS_Edge& CurEdge    = _dmMeshedEdgeNodeKArray.FindEdge_Index(i);
		//BOOL        bRevEdge          = (CurEdge.Orientation() == TopAbs_REVERSED);
		BOOL        bCheckedHighOrder = _MadeEdgeRatioArray.IsHighOrder(CurEdge);
		TColStd_SequenceOfReal&    seqRatio = _MadeEdgeRatioArray(CurEdge);
		TColgp_SequenceOfPnt&      seqPnt   = _MadeEdgePntArray(CurEdge);
		TColStd_SequenceOfInteger& seqIndex = _dmMeshedEdgeNodeKArray(CurEdge);

		ASSERT(seqRatio.Length() == seqPnt.Length());
		
		int nLinearNode = seqRatio.Length();
		if(nLinearNode == 0) continue;
		
		if(bCheckedHighOrder)
		{
			nLinearNode = nLinearNode/2 +1;
			for(int j=1; j<nLinearNode; j++)
			{
				NodeK1 = seqIndex(j);
				NodeK2 = seqIndex(j+1);
				PairKey.Set(NodeK1, NodeK2);
				if(!mNodePairKey.Lookup(PairKey, nKey))
				{
					int nCurInd = _AllNodes.GetSize();
					mNodePairKey.SetAt(PairKey, nCurInd);
					
					// Get Mid-node
					T_POINT3D  MidPnt;
					MidPnt = seqPnt(j*2);    
					_AllNodes.Add(CMeshUtil::PntToNode(MidPnt));
				}
			}
		}
		else
		{
			ASSERT(seqRatio.Length() == seqIndex.Length());

			_MadeEdgeRatioArray.SetHighOrder(CurEdge, TRUE);
			_MadeEdgePntArray.SetHighOrder(CurEdge, TRUE);

			Standard_Real FirstParam, LastParam;
			Handle(Geom_Curve) hCurve= BRep_Tool::Curve (CurEdge, FirstParam, LastParam);
			Handle(Geom_TrimmedCurve) tCurve = new Geom_TrimmedCurve(hCurve, FirstParam, LastParam);
			FirstParam = tCurve->FirstParameter();
			LastParam  = tCurve->LastParameter();
			if(tCurve.IsNull()) continue;
			GeomAdaptor_Curve anAdaptorCurve(tCurve);
			Standard_Real dCurvelength = GCPnts_AbscissaPoint::Length(anAdaptorCurve, FirstParam, LastParam);

			TColStd_SequenceOfReal seqLinearRatio;
			TColgp_SequenceOfPnt   seqLinearPnt;
			seqLinearRatio = seqRatio;
			seqLinearPnt   = seqPnt;
			for(int j=1; j<nLinearNode; j++)
			{
				NodeK1 = seqIndex(j);
				NodeK2 = seqIndex(j+1);
				PairKey.Set(NodeK1, NodeK2);
				if(!mNodePairKey.Lookup(PairKey, nKey))
				{
					int nCurInd = _AllNodes.GetSize();
					mNodePairKey.SetAt(PairKey, nCurInd);
					
					// Get Mid-node
					Standard_Real dMidRatio = (seqLinearRatio(j) + seqLinearRatio(j+1))*.5;
					T_POINT3D        MidPnt;
					
					if(pref.Mesh.bGeomMatchedMidNode)
					{
						GCPnts_AbscissaPoint GCPntTool(anAdaptorCurve, dMidRatio*dCurvelength, FirstParam);
						if(!GCPntTool.IsDone()) { ASSERT(0); return FALSE;}
				    hCurve->D0 ( GCPntTool.Parameter(), MidPnt );
					}
					else
					{
						MidPnt.SetX((seqLinearPnt(j).X()+seqLinearPnt(j+1).X())*.5);
						MidPnt.SetY((seqLinearPnt(j).Y()+seqLinearPnt(j+1).Y())*.5);
						MidPnt.SetZ((seqLinearPnt(j).Z()+seqLinearPnt(j+1).Z())*.5);
					}
					seqRatio.InsertAfter(j*2-1, dMidRatio);
					seqPnt.InsertAfter(j*2-1, MidPnt);
					
					_AllNodes.Add(CMeshUtil::PntToNode(MidPnt));
				}

			}
		}
	}

	// 2. 내부의 Mid Node를 구한다.
	for(i=0; i<_AllElems.GetSize(); i++)
	{
		const T_ELEM_D& ElemD = _AllElems[i];
		int nNode = ElemD.aNodeK.GetSize();
		for(int j=0; j<nNode; j++)
		{
			NodeK1 = ElemD.aNodeK[j];
			NodeK2 = ElemD.aNodeK[(j+1)%nNode];
					
			PairKey.Set(NodeK1, NodeK2);
			if(!mNodePairKey.Lookup(PairKey, nKey))
			{
				int nCurInd = _AllNodes.GetSize();
				mNodePairKey.SetAt(PairKey, nCurInd);
				T_NODE_D MidNode;
				CMeshUtil::GetMidNode(_AllNodes[NodeK1], _AllNodes[NodeK2], MidNode);        
				_AllNodes.Add(MidNode);
			}
		}
	}
	
	// 3. Element를 재구성한다.
	for(i=0; i<_AllElems.GetSize(); i++)
	{
		T_ELEM_D& ElemD = _AllElems[i];
		
		if(ElemD.vtktyp == VTK_TRIANGLE) ElemD.vtktyp = VTK_QUADRATIC_TRIANGLE;
		else ElemD.vtktyp = VTK_QUADRATIC_QUAD;
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
*/

//============================================================================
// SetShapeElemSide
//============================================================================
/*
BOOL CAutoMesh_PlanarArea::SetShapeElemSide(const CMap<UINT, UINT, T_NODE_K, T_NODE_K>& mNodeK)
{M_PROFILE('BOOL CAutoMesh_PlanarArea::SetShapeElemSide()');
	CMeshMgr              MeshMgr;
	TColgp_SequenceOfPnt  seqBdyPnt;
	TColStd_SequenceOfInteger  seqBdyNodeK;
	
	for(int i=1; i<=_seqBdyNodeID.Length(); i++)
	{
		T_NODE_K NodeK;
		T_NODE_D NodeD;
		if(!mNodeK.Lookup(_seqBdyNodeID(i)+1, NodeK)) return FALSE;
		MeshMgr.GetNodeNeutralUnit(NodeK, NodeD);
		const T_POINT3D& pnt = CMeshTools::NodeToPnt(NodeD);
		seqBdyPnt.Append(pnt);
		seqBdyNodeK.Append(NodeK);
	}

	// set edge list
	int nEdge = _dmMeshedEdgeNodeKArray.Extent();
	for(i=1; i<=nEdge; i++)
	{
		TColStd_SequenceOfInteger& seqNodeK = _dmMeshedEdgeNodeKArray.GetAt_Index(i);
		TopoDS_Edge CurEdge = _dmMeshedEdgeNodeKArray.FindEdge_Index(i);
		CArray<UINT, UINT>   aNodeID;
		T_NODE_K             NodeK;

		for(int j=1; j<=seqNodeK.Length(); j++)
		{
			if(!mNodeK.Lookup(seqNodeK(j)+1, NodeK)) return FALSE;
			aNodeID.Add(NodeK);
		}
// nodeID마다의 attribute를 이용하여 owner element들을 확인.
		CArray<ELEMKEY_INDEX, ELEMKEY_INDEX&>* paKeyInd;
		paKeyInd = new CArray<ELEMKEY_INDEX, ELEMKEY_INDEX&>;

		for(j=0; j<aNodeID.GetSize()-1; j++)
		{
			UINT NodeK1 = aNodeID[j];
			UINT NodeK2 = aNodeID[j+1];
			if(NodeK1 == NodeK2) continue;
			T_ELEM_K_LIST aElemK;
			T_NODE_K_LIST aNodeK;
			aNodeK.Add(NodeK1);aNodeK.Add(NodeK2);
			MeshMgr.GetElemKeyListIncludingCommonNodes(aNodeK, aElemK);
			for(int k=0; k<aElemK.GetSize(); k++)
			{
				int nSideInd = CMeshUtil::GetEdgeSideIndex(aElemK[k], NodeK1, NodeK2);
				if(!nSideInd) MSGBOX_ERROR_RETURN( CANNOT_MAKE_RELATION_MESH_SHAPE );
				ELEMKEY_INDEX KeyInd;
				KeyInd.nKey     = aElemK[k];
				KeyInd.nSideInd = nSideInd;
				paKeyInd->Add(KeyInd);
			}
		}
		_dmEdgeElemInfo.Bind(CurEdge, paKeyInd);
	}


	return TRUE;
}
*/