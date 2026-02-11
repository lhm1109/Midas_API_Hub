#if !defined(AFX_MESHAUTODLG_H__C4F5F3A2_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_MESHAUTODLG_H__C4F5F3A2_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeshAutoDlg.h : header file
//
#include "..\wg_base\NotifyCtrl.h"

#include "..\wg_db\EditMatl.h"
#include "..\wg_db\EditThik.h"
#include "..\wg_db\CobxMatl.h"
#include "..\wg_db\CobxThik.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\UserDefType.h"

#include "..\wg_db\AutoMesh_PlanarArea.h"
#include "..\MIT_frx\MComboBox.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CMeshAutoDlg dialog

#define D_CPF_FIND_TYPE_INNER     0     // find inner cycle. 안쪽 폐구간 
#define D_CPF_FIND_TYPE_OUTER     1     // find outer cycle. 바깥쪽 폐구간 

class CMeshAutoDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
protected:
	static int m_nCurETypeIndex;

// Construction
public:
	CMeshAutoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMeshAutoDlg();
	virtual void Execute();

	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEscape(WPARAM wParam, LPARAM lParam);
	
public:
	virtual BOOL ExternalInit(UINT key);
	void SetTempElemDataByCurSetting();
	int  GetCurElemTypeID();// 현재 설정된 Element의 Type ID를 얻어낸다.  
	void InitElemTypeCombo();
	BOOL CheckArg(const BOOL bSimul = FALSE); 
	BOOL GetPropIDS(int nElemType,int& nMatID,int& nThkID);

	BOOL GetPlanePoint(std::vector<T_LINE_G>& aEdge, double dPlanePoint[3][3]);
	BOOL GetSamePlaneElementEdge(std::vector<T_LINE_G>& aPlaneEdge, T_ELEM_D& ElemD, std::vector<T_LINE_G>& arIntersectEdge);
	BOOL ConfirmInPlane(std::vector<T_LINE_G>& aEdge, CArray<T_POINT3D,T_POINT3D&>& aNode, double PlaneLocalVector[3][3], double dPlanePoint[3][3]);
	BOOL GetDomainType(int& nType, double dPlanePoint[3][3]);
// Dialog Data
	//{{AFX_DATA(CMeshAutoDlg)
	enum { IDD = IDD_TM_MESH_AUTO };
	int m_nElemType;
	
	int m_nMatID;
	int m_nThkID;

	mit::frx::MComboBox m_cmbMethod;
	mit::frx::MComboBox m_cmbType;

	CMouseEdit m_wndSelectNodeListEdit;  
	CMouseEdit m_wndNodeListEdit;  
	CMouseEdit m_wndElemListEdit;  
	CSelectEdit m_wndSelectElemListEdit;

	BOOL m_bInnerDomain;
	BOOL m_bInteriorNode;
	BOOL m_bInteriorLine;
	BOOL m_bBndrConnect;

	BOOL m_bDeleteSrcLine;  
	BOOL m_bSubdivideSrcLine;

	int m_nOption_Node;
	int m_nOption_Line;
	int m_nOption_Size;
	
	mit::frx::MComboBox	m_ctrlCobxEType;
	CCobxThik	m_ctrlCobxThik;
	CCobxMatl	m_ctrlCobxMatl;
	CEditThik	m_ctrlEditThik;
	CEditMatl	m_ctrlEditMatl;

	CEditUnit m_edtMeshSize;
	CTextUnit m_untMeshSize;
	//}}AFX_DATA	
	int m_nThickThin;// 0:Thick, 1:Thin
	BOOL m_bDOF;
	int m_nMembPlate;//0:Membrane, 1:Plate

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeshAutoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	BOOL    m_bStartNodeInput;
	double  m_dTol;
	double  m_dPlaneTol;
	double  m_dLengthTol;
	AUTOMESH_ARGUMENTS AutoMeshArg;
	BOOL    m_bChangeMeshSize;
	int     m_nChangeMethod;
	double  m_dMinMeshSize;
	double  m_dMaxBoundarySize; 

	std::vector<T_LINE_G> m_aEdge;
	CArray<T_POINT3D,T_POINT3D&> m_aSelectNode;

	CDBDoc*   m_pDoc;
	BOOL      m_bPropInit;
	CArray<int,int> m_ElemTypeIDs;
	std::vector<UINT> m_aKeyList;
	CMapEx<int,int,int,int> m_mapNodeKeyList;
	CMapEx<int,int,int,int> m_mapElemKeyList;
	CArray<UINT,UINT> m_aInteriorNode;
	CArray<UINT,UINT> m_aInteriorElem;

	CMap<UINT,UINT,UINT,UINT> m_mapSeedElemK;
	void MakeSeedElemKList();

	CArray<UINT, UINT> m_DpaaK;
	CArray<UINT, UINT> m_CcpaK;
	CArray<UINT, UINT> m_MasterNodeK;
	CMouseEdit m_wndDirPt1;
	BOOL m_bShowMsg;
	void InitPropertyCtrls();

	void ChangeMethod();

	void EnableInterierNode(BOOL bEnable);
	void EnableInterierLine(BOOL bEnable);

	BOOL CheckInteriorNode();
	BOOL CheckInteriorEdge();

	void ChangeElemTypeComboWidth();
	void DivideEdgeByIncludeNode(std::vector<T_LINE_G>& aEdge, CArray<T_POINT3D,T_POINT3D&>& aNode);
	void AddEdge(std::vector<T_LINE_G>& aEdge, T_LINE_G& LineG);
	BOOL IsInsideEdge(double dBoundLine1[2], double dBoundLine2[2], double dTargetLine1[2], double dTargetLine2[2]);
	void MeshByPlanarElement();
	void MeshByNode();
	void MeshByLine();
	BOOL AddNodeElemMadoSbdoList(CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD, CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD, std::vector<int>& aDomainElemCount, double dPlanePoint[3][3], std::vector<T_NODE_D>& aHoopNodeData);
	int  CreateMeshByEdge(CArray<T_MESH_ELEM_D,T_MESH_ELEM_D&>& aMeshElemD, CArray<T_MESH_NODE_D,T_MESH_NODE_D&>& aMeshNodeD, std::vector<int>& aDomainElemCount, double dPlanePoint[3][3], std::vector<T_NODE_D>& aHoopNodeData);
	void MakeHoopNode(CArray<T_POLYLINE3D*,T_POLYLINE3D*>& aPolyLine, std::vector<T_NODE_D>& raHoopNodeData);
	void MakeMainDomainData(T_MADO_D& MadoD);
	void MakeNodeElemList(CArray<T_MESH_ELEM_D,T_MESH_ELEM_D&>& aMeshElemD, CArray<T_MESH_NODE_D,T_MESH_NODE_D&>& aMeshNodeD, CArray<T_ELEM_K, T_ELEM_K>& arElemK, CArray<T_ELEM_D, T_ELEM_D&>& arElemD,
						  CArray<T_NODE_K, T_NODE_K>& arNodeK, CArray<T_NODE_D, T_NODE_D&>& arNodeD, std::vector<int>& aDomainElemCount);
	void MakeNodeElemList_SubDomain(CArray<T_MESH_ELEM_D, T_MESH_ELEM_D&>& aMeshElemD, CArray<T_MESH_NODE_D, T_MESH_NODE_D&>& aMeshNodeD, std::vector<T_ELEM_K>& arElemK, std::vector<T_ELEM_D>& arElemD,
									std::vector<T_NODE_K>& arNodeK, std::vector<T_NODE_D>& arNodeD, std::vector<int>& aDomainElemCount, int nNodeStartIndex=0, int nElemStartIndex=0);
	void MakeSubDomainData(std::vector<int>& aDomainElemCount, double dPlanePoint[3][3], std::vector<T_ELEM_K>& arElemK, CArray<T_SBDO_ELEMLIST_D, T_SBDO_ELEMLIST_D&>& arSbdoElemList);
	int  FindIndexDomainElemCount(int nIndex, std::vector<int>& aDomainElemCount);
	void MakeSeedSizeInfo(std::vector<T_LINE_G>& aEdge, CArray<double,double&>& aSeedSize, const double& dRotAngle, const double dAxisVec[3]);
	void DivideEdge(std::vector<T_LINE_G>& aEdge);
	void EndMesh();

	void SetDomainName();

	BOOL DetermineConnectionLine(const std::vector<T_LINE_G>& aEdge_copy, const T_LINE_G& rLineG, CMapEx<int, int, BOOL, BOOL>& mEdgeConnectBndr, CMapEx<int, int, int, int>& mUseEdge);
	void MakeEdgeConnectInfo(const std::vector<T_LINE_G>& aEdge_copy, std::vector<T_LINE_G>& aBoundaryEdge, CMapEx<int, int, BOOL, BOOL>& mEdgeConnectBndr, CArray<T_LINE_G,T_LINE_G&>& aEdge_Drop, CArray<T_LINE_G,T_LINE_G&>& aEdge_Colm);
	BOOL AddEdgeByDropPanelNColmCapital(std::vector<T_LINE_G>& aEdge, const std::vector<T_LINE_G>& aEdge_org, std::vector<T_LINE_G>& aBoundaryEdge, CArray<T_LINE_G,T_LINE_G&>& aEdge_Drop, CArray<T_LINE_G,T_LINE_G&>& aEdge_Colm, CArray<T_POINT3D,T_POINT3D&>& aSelectNode, CMapEx<int, int, BOOL, BOOL>& mEdgeConnectBndr, BOOL bSecond=FALSE);
	BOOL MakeEdgeByDropPanelNColmCapital(CArray<T_LINE_G,T_LINE_G&>& aEdge_Drop, CArray<T_LINE_G,T_LINE_G&>& aEdge_Colm, double dPlanePoint[3][3], double dRotAngle, double dAxisVec[3]);
	BOOL IsConnectBounary(const T_LINE_G& rLine1, const std::vector<T_LINE_G>& aBoundaryEdge);
	BOOL IsInterSectTwoLine(const T_LINE_G& rLine1, const T_LINE_G& rLine2, double pInts[3]);
	void RemoveRedundentEdge(std::vector<T_LINE_G>& aEdge);
	BOOL Get3Point(CMouseEdit& pt, double &Px, double &Py, double &Pz);
	struct T_CPF_EDGE
	{
		UINT id;    // = MembK

		UINT node1;   // edge로 연결되는 node id
		UINT node2;   // edge로 연결되는 node id

		double cx;  // 방향 판단용. edge의 중간점.
		double cy;  // 방향 판단용. edge의 중간점.

		T_CPF_EDGE() { Initialize(); }
		void Initialize()
		{
			id = 0;   
			node1 = 0;
			node2 = 0;
			cx = 0.0; 
			cy = 0.0; 
		}
		UINT GetOppositeNode(UINT nodeId)
		{
			if (nodeId == node1)    return node2;
			if (nodeId == node2)    return node1;

			ASSERT(0); 
			return FALSE;
		}
		void ReverseNode()
		{
			UINT nodeTemp = node1;
			node1 = node2;
			node2 = nodeTemp;
		}
	};
	typedef CMapEx<UINT,UINT, T_CPF_EDGE,T_CPF_EDGE&> T_CPF_EDGE_MAP;

	/////////////////////////////////////////////////////////////////////////////
	// T_CPF_NODE
	struct T_CPF_NODE
	{
		UINT id;    // = MembK

		double x;
		double y;

		CArray<UINT,UINT> listEdge;   // node에 연결되어 있는 edge들의 id list

		void Initialize()
		{
			id = 0;  
			x = 0.0;
			y = 0.0;

			listEdge.RemoveAll();
		}
		T_CPF_NODE() {Initialize();}
		T_CPF_NODE(T_CPF_NODE& src) { *this = src; }
		T_CPF_NODE& operator=(const T_CPF_NODE& src)
		{
			id = src.id;
			x  = src.x;
			y  = src.y;

			listEdge.Copy(src.listEdge);

			return *this;
		}
		void RemoveEdge(UINT edgeId)
		{
			for (int i=listEdge.GetSize()-1; i>=0; --i)
			{
				if (listEdge[i] == edgeId)   listEdge.RemoveAt(i);
			}
		}
		void AddEdge(UINT edgeId) {listEdge.Add(edgeId);}
		int GetConnectedEdgeCount() { return listEdge.GetSize(); };
	};
	typedef CMapEx<UINT,UINT, T_CPF_NODE,T_CPF_NODE&> T_CPF_NODE_MAP;


	struct T_CPF_CYCLE_PATH
	{
		UINT id;    // slab의 MembK를 위한 용도. CClosedPathFinder 내에서는 따로 세팅하지 않음.
		CArray<UINT,UINT> aEdgeId;
		CArray<UINT,UINT> aNodeId;

		BOOL bExistBotPoint;                              // 바닥점은 아래층의 점일 수도 있고 aPontGCS_bot에 좌표값만 갖는 점일 수도 있다.
		UINT StorK;
		CArray<T_POINT2D, T_POINT2D&> aPontGCS_bot;       // 바닥위치 절대좌표
		// aNodeId 에 -1값 존재시 차례대로 사용

		void Initialize() { id = StorK = NULL;  aEdgeId.RemoveAll();  aNodeId.RemoveAll();  bExistBotPoint = FALSE;   aPontGCS_bot.RemoveAll();   };

		T_CPF_CYCLE_PATH() { Initialize(); };
		T_CPF_CYCLE_PATH(T_CPF_CYCLE_PATH& src) { *this = src; }
		T_CPF_CYCLE_PATH& operator=(const T_CPF_CYCLE_PATH& src)
		{
			id = src.id;
			aEdgeId.Copy(src.aEdgeId);
			aNodeId.Copy(src.aNodeId);

			bExistBotPoint = src.bExistBotPoint;
			StorK = src.StorK;
			aPontGCS_bot.Copy(src.aPontGCS_bot);

			return *this;
		}
		void Set(UINT nID, CList<UINT,UINT>& listEdgeId, CList<UINT,UINT>& listNodeId)
		{
			ASSERT(listEdgeId.GetCount() == listNodeId.GetCount());

			id = nID;

			POSITION pos = NULL;

			int nCountEdge = listEdgeId.GetCount();
			aEdgeId.SetSize(nCountEdge);
			pos = listEdgeId.GetHeadPosition();
			int i=0;
			for(int i=0; i<nCountEdge; ++i)  aEdgeId[i] = listEdgeId.GetNext(pos);

			int nCountNode = listNodeId.GetCount();
			aNodeId.SetSize(nCountNode);      
			pos = listNodeId.GetHeadPosition();
			for(int i=0; i<nCountNode; ++i)  aNodeId[i] = listNodeId.GetNext(pos);
		}
	};
	
	void GetNodeListRemoveDup(CString& strNodeList, std::vector<UINT>& aNodeList);
	int  FindClosePath();
	BOOL MakeGraphFromMembK(T_KEY ElemK);
	BOOL MakeGraphFromPontK(T_KEY PontK);
	void InitializeGraph();
	BOOL IsAlreadyExistPath(const T_ELEM_D& ElemD);
	BOOL AddMember2Edge(T_ELEM_K ElemK, T_ELEM_D& ElemD);
	BOOL ExistSameEdge(const T_CPF_EDGE& edge);
	BOOL AddOrReplaceEdge(T_CPF_EDGE& edge);
	BOOL RegisterEdge2Node(UINT edgeId, UINT nodeId);
	BOOL GetOrMakeNode(UINT nodeId, T_CPF_NODE& node);
	BOOL AddOrReplaceNode(T_CPF_NODE& node);
	BOOL RemoveCaltileverFromGraph();
	BOOL RemoveCaltieverFromGraph_sub(UINT nodeId, CMapEx<UINT,UINT, int,int>& mNodeId_passed);
	BOOL RemoveCantileverNode(UINT nodeId, CMapEx<UINT,UINT, int,int>* pmNodeId_passed=NULL);
	BOOL RemoveNode(UINT nodeId);
	BOOL RemoveEdge(UINT edgeId);
	BOOL MakeOutCyclePath(CList<UINT,UINT>& rlistEdgeOfOuterCycle, UINT& rnNodeId_start,UINT& rnEdgeId_start);
	BOOL GetFirstNodeForOuterCycle(T_CPF_NODE& rNodeOut, CMapEx<UINT,UINT, T_CPF_NODE,T_CPF_NODE&>& mNode);   
	BOOL GetFirstEdgeForOuterCycle(const T_CPF_NODE& nodeStart, T_CPF_EDGE& rEdgeOuterCW);
	int GetCyclePathCCW(const T_CPF_EDGE& edge_start, UINT nNodeID_start, CList<UINT,UINT>& rlistEdgeOfCycle, CList<UINT,UINT>* plistNodeOfCycle, BOOL bSmallCycle, const T_CPF_NODE_MAP& m_mNode_working);
	BOOL GetNextEdgeCCW(const T_CPF_EDGE& edge_prev, UINT nodeID_joint, T_CPF_EDGE& rEdge_next, BOOL bSmallCycle, const T_CPF_NODE_MAP& m_mNode_working);
	double GetPathAngleCCW(double a0, double a1,  double b0, double b1,  double c0, double c1);
	int GF_GetHashSizeByCount(int nDataCount1);
	BOOL UpdateCycleEdgeDirection(CList<UINT,UINT>& listEdgeOfCycle, UINT nNodeId_start);
	BOOL MakeOutCycleNodeMap(const CList<UINT,UINT>& listEdgeOfOuterCycle);
	BOOL MakeUnitCyclePath(int nFindType/*=D_CPF_FIND_TYPE_INNER*/);
	BOOL RegisterEdge2Node_outerCycle(UINT nEdgeId);
	BOOL RegisterEdge2Node_outerCycle_sub(UINT nEdgeId, UINT nNodeId);
	BOOL CheckCyclePathCCW(const CList<UINT,UINT>& listNodeOfUnitCycle);
	BOOL DisjointUnionEdge_outerCycle(const CList<UINT,UINT>& listEdge);
	BOOL DisjointUnionEdge(UINT nodeId, UINT edgeId, T_CPF_NODE_MAP& mapNode, BOOL& rbDisconnect);
	BOOL RemoveCantileverNode_connectedOnly(const CList<UINT,UINT>& listNode);
	BOOL RemoveCantileverNode_connectedOnly(UINT nodeId);
	BOOL ReverseCycle(CList<UINT,UINT>& rlistEdgeOfCycle, CList<UINT,UINT>& rlistNodeOfCycle);
	void MakePolyLineMergingStraightLine(CArray<UINT,UINT>& aNodeK, std::vector<T_LINE_G>& aBoundaryEdge);
	CMapEx<T_KEY,T_KEY, int,int> m_mMembK_checked;
	T_CPF_EDGE_MAP  m_mEdge;
	T_CPF_NODE_MAP  m_mNode;
	T_CPF_NODE_MAP  m_mNode_outcycle;    // 외곽 cycle을 형성하는 edge의 연결 정보만 갖는 임시 node map
	CMapEx<UINT,UINT, int,int>   m_mEdge_erase;  // 폐구간 생성에 이미 사용된 Edge ID
	CArray<T_CPF_CYCLE_PATH,T_CPF_CYCLE_PATH&> m_aClosedPath;       // 찾아낸 폐구간들 
	// 외곽 cycle에서 한번 삭제 되었던 edge. (무한루프 방지용)

	double m_trang;
	double m_NormalZero;
	double mathDistanceToLine(double line_i[3], double line_j[3], double point[3]);// 점과 선과의 최단거리(선 범위내에서)
	double mathDistanceToPoint(double p1[3], double p2[3]);
	BOOL mathIntersectLine2(double pl1_i[3], double pl1_j[3], double pl2_i[3], double pl2_j[3], double Tolerance, double& dblDistance, double pInts[3]);	// 두개의 선분 내의 교점
	BOOL mathNormalize(double dx, double dy, double dz, double& dxn, double& dyn, double& dzn);	//단위벡터로 변환(3d)
	double mathLength(double dx, double dy, double dz = 0.);		//1점과 원점과의 거리(2,3차원)
	double mathCrossAngle(double Vector1[3], double Vector2[3]);	//두벡터 사이의 각도를 계산
	double mathDot(double xyz1[3], double xyz2[3]);
	double mathAcos(double x);
	void   mathCross_product_3d(const double xyz0[3], const double xyz1[3], const double xyz2[3], double result_vector[3]);
	BOOL mathIncludePointInPlane(double p1[3], double p2[3], double p3[3], double point[3], double Tol = 0.);
	BOOL mathPlaneEquation(double p1[3], double p2[3], double p3[3], double& a, double& b, double& c, double & h);
	double mathDistanceToPlane(double a, double b, double c, double h, double x, double y, double z); // 점과 면의 수직거리
	double mathSqrt(double x);
	BOOL mathIsPointOfLine2D(double bound1[2], double bound2[2], double targetPt[2], BOOL isOnLine = TRUE);
	double mathCross2D(double vector1[2], double vector2[2]);
	void mathCross(double xyz1[3], double xyz2[3], double xyzout[3]);
	double mathCrossAngleNormalize(double Vector1[3], double Vector2[3]);
	void mathRotate(double angle, double ux, double uy, double uz, double& rx, double& ry, double& rz);  //임의 축에 대한 회전, angle:회전각[deg], ux,uy,uz:회전축 벡터, rx,ry,rz:회전대상 좌표
	void mathRotate(double angle, double px, double py, double pz, double ux, double uy, double uz, double& rx, double& ry, double& rz);

	// Generated message map functions
	//{{AFX_MSG(CMeshAutoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmMatButton();
	afx_msg void OnTmThickButton();
	afx_msg void OnTmDomainButton();
	afx_msg void OnSelchangeTmElemTypeCombo();
	afx_msg void OnSelchangeMethodCombo();
	afx_msg void OnClickInteriorNodeRadio();
	afx_msg void OnClickInteriorLineRadio();
	afx_msg void OnClickMeshSizeRadio();
	afx_msg void OnClickInteriorNodeCheck();
	afx_msg void OnClickInteriorLineCheck();
	afx_msg void OnClickDeleteSrcCheck();
	afx_msg void OnSetfocusTmSelectNodeEdit();
	afx_msg void OnSetfocusTmNodeEdit();
	afx_msg void OnSetfocusTmElemEdit();
	afx_msg void OnElemTypeButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESHAUTODLG_H__C4F5F3A2_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_)
