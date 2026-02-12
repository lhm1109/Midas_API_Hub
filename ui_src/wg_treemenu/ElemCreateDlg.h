#if !defined(__ELEMCREATEDLG_H__)
#define      __ELEMCREATEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemCreateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CElemCreateDlg dialog
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\EditMatl.h"
#include "..\wg_db\EditSect.h"
#include "..\wg_db\EditThik.h"
#include "..\wg_db\CobxMatl.h"
#include "..\wg_db\CobxSect.h"
#include "..\wg_db\CobxThik.h"
#include "..\wg_db\EditWid.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"

class _ElemCreate
{
public:
	int m_nElemType;
	
	int m_nSecID;
	int m_nMatID;
	int m_nThkID;

	int m_nBAngleOrRefPoint;
	int m_nRefAngle; // Plane Strain, Axisymmetric, Solid
	double m_fBAngle;
	double m_RPWx,m_RPWy,m_RPWz;
	T_NUCS_K m_NucsK;

	CArray<unsigned int,unsigned int> m_NodeKeyList;
	int m_nMaxNodeNum;

	BOOL m_bIntersectNode;
	BOOL m_bIntersectElem;
	BOOL m_bCreateInnerPoint;

	double m_dNonlLength, m_dTensionForce/*AlComp, AlTens*/, m_dLnLRatio;
	double m_dLimit; // Tens Limit, Comp Limit
	BOOL   m_bUseLimit;
	int    m_nTensCableKind;
	int    m_nSubType;
	int    m_nWallID;
	int    m_nNodeConnectivity;
	int    m_nWallType;


	_ElemCreate()
	{
		m_nElemType = -1;
		m_nSecID    = 0;
		m_nMatID    = 0;
		m_nThkID    = 0;

		m_nBAngleOrRefPoint = -1;
		m_nRefAngle = -1;
		m_fBAngle = 0.0;
		m_RPWx = m_RPWy = m_RPWz = 0.0;
		m_NucsK = 0;

		m_nMaxNodeNum = -1;
		
		m_bIntersectNode = FALSE;
		m_bIntersectElem = FALSE;
		m_bCreateInnerPoint = FALSE;

		m_dNonlLength = m_dTensionForce = m_dLimit = 0.0;
		m_bUseLimit = FALSE;
		m_dLnLRatio = 1.0;
		m_nTensCableKind = 0;
		m_nSubType = 0;
		m_nWallID = 0;
		m_nNodeConnectivity = 0;
		m_nWallType = 0;
	}
};

class CElemCreateDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
protected:
	static int m_nCurETypeIndex;

// Construction
public:
	CElemCreateDlg(CWnd* pParent = NULL);   // standard constructor
	~CElemCreateDlg();

protected:
	CArray<UINT, UINT> m_SectCtrls;
	CArray<UINT, UINT> m_ThickCtrls;
	CArray<UINT, UINT> m_KNodeCtrls;

	CArray<UINT, UINT> m_BAngleCtrls;
	CArray<UINT, UINT> m_RefPointCtrls;
	CArray<UINT, UINT> m_DirVectorCtrls;
	CArray<UINT, UINT> m_PlateNodesCtrls;
	CArray<UINT, UINT> m_SolidNodesCtrls;
	CArray<UINT, UINT> m_IntersectNodeCtrls;
	CArray<UINT, UINT> m_IntersectElemCtrls;

	CArray<UINT, UINT> m_NormalElemCtrls;
	CArray<UINT, UINT> m_TensTrussCtrls;
	CArray<UINT, UINT> m_TensTruss0Ctrls;
	CArray<UINT, UINT> m_TensTruss1Ctrls;
	CArray<UINT, UINT> m_TensTruss2Ctrls;
	CArray<UINT, UINT> m_TensTruss3Ctrls;
	CArray<UINT, UINT> m_TensTruss4Ctrls;
	CArray<UINT, UINT> m_CompTrussCtrls;
	CArray<UINT, UINT> m_CompTruss0Ctrls;
	CArray<UINT, UINT> m_CompTruss1Ctrls;
	CArray<UINT, UINT> m_WallCtrls;
	CArray<UINT, UINT> m_Wall1Ctrls;
	CArray<UINT, UINT> m_WallCRB;
	CArray<UINT, UINT> m_WallNodeConnectRdo;
	CArray<UINT, UINT> m_WallTypeRdo;
	CArray<UINT, UINT> m_HeightCtrls;

	CArray<UINT,UINT> m_arPlateTypeCtrl; // Plate Type Control
	CArray<UINT, UINT> m_aOrientation; // Plane Strain, Axisymmetric, Solid
	CArray<UINT, UINT> m_aRefAngle;
	CArray<UINT, UINT> m_aRefAxis;

	CArray<UINT, UINT> m_aNodalConnec;

	CArray<int,int> m_ElemTypeIDs;
	BOOL  m_bIsParamError;

public:
	void SetTempElemDataByCurSetting();
	void _CreateElement_MEC(_ElemCreate& EC, T_ELEM_D& EData);
	void _CreateElement();
	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	/*********************************************************************
	 *
	 *
 *****   ELEMENT CREATION PARAMETER 설정 
	***
	 */
	BOOL GetNodeListAndNodeNum(int nElemType,
														 CArray<unsigned int,unsigned int>& _NodeList,
														 int& nMaxNodeNum);
	BOOL MakeVerticalNode(CArray<unsigned int, unsigned int>& _NodeList);
	BOOL GetKNodeDatas(int nElemType, CArray<unsigned int, unsigned int>& NodeKeyList, int& nBAngleOrRefP,
	                   int& nRefAngle, double& BAngle, double& RPWx, double& RPWy, double& RPWz, unsigned int& NucsK);
	int  GetCurElemTypeID();// 현재 설정된 Element의 Type ID를 얻어낸다.  
	BOOL GetPropIDS(int nElemType,int& nMatID,int& nSecID,int& nThkID);
	
	BOOL GetParameter(_ElemCreate & EC);
	
	BOOL GetElemTypeAndMaxNodeNum(int& nElemType,int& nMaxNodeNum);

	
	void UpdateNodeNumCtrls();
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEscape(WPARAM wParam, LPARAM lParam);
	void AngleSpinHandler(CWnd* pWnd, CString& str, NMHDR* pNMHDR, LRESULT* pResult);
	void KNodeEnableDisable();
	void InitPropertyCtrls();
	void ShowHideElemTypeCtrl(int nElemType);
	void EnableDisableIntersectCtrl(int nElemType);
	void ChildLayOut(int ElemType);
	void InitElemTypeCombo();
	void InitHeightCombo();
	void SetCtrlGroups();
	virtual void Execute();
	BOOL GetDistance(CMouseEdit* pDistEdit, double& dblDist);

	void TrussCtrlMan();
	void AdjustComboListBox(mit::frx::MComboBox& Cbx);
	// 새로 추가 
	void InitTensCableCombo();
	void ChangeTensCableCombo();
	int  GetTensCableKind();
	  
	// Dialog Data
	//{{AFX_DATA(CElemCreateDlg)
	enum { IDD = IDD_TM_ELEM_CREATE };
	CTextUnit	m_wndTTLengthUnit;
	CEditUnit	m_wndTTLengthEdit;
	mit::frx::MComboBox	m_cobxTensCable;
	CEditWid	m_editWall;
	mit::frx::MComboBox	m_cboWall;
	mit::frx::MButton	m_chkOrtho;
	CParseEdit	m_wndNodeUserEdit;
	mit::frx::MComboBox	m_cobxNodeEdit;
	mit::frx::MButton	m_chkCreateInnerPoint;
	CMouseEdit  m_wndCTDistEdit;
	CEditUnit	m_wndTTForceEdit;
	CMouseEdit	m_wndTTDistEdit;
	CEditUnit	m_wndTTAlCompEdit;
	CEditUnit	m_wndCTAlTensEdit;
	CEditUnit m_edtCompLimit; // MNET:2774 20070507 mylee
	CEditUnit m_edtTensLimit;
	CTextUnit m_untCompLimit;
	CTextUnit m_untTensLimit; // END
	CTextUnit	m_wndCTDistUnit;
	CTextUnit	m_wndTTForceUnit;
	CTextUnit	m_wndTTDistUnit;
	CTextUnit	m_wndCTAlTensUnit;
	CTextUnit	m_wndTTAlCompUnit;
	CBCGPStatic	m_wndOriTitle;
	CBCGPStatic	m_wndNodesTitle;
	CBCGPStatic	m_wndThickTitle;
	CBCGPStatic	m_wndSecTitle;
	CBCGPStatic	m_wndMatTitle;
	CMouseEdit	m_wndRefPointEdit;
	CMouseEdit	m_wndDirVectorEdit;
	mit::frx::MComboBox	  m_wndBAngleEdit;
	CTextUnit	m_wndAngleUnit;
	CTextUnit	m_wndDistUnit;
	CTextUnit m_wndVectorUnit;
	CMouseEdit m_wndNodeListEdit;
	mit::frx::MComboBox	m_ctrlCobxEType;
	CCobxThik	m_ctrlCobxThik;
	CCobxSect	m_ctrlCobxSect;
	CCobxMatl	m_ctrlCobxMatl;
	CEditThik	m_ctrlEditThik;
	CEditSect	m_ctrlEditSect;
	CEditMatl	m_ctrlEditMatl;
	mit::frx::MillustViewer m_wndPicture;
	int		m_nBetaAngleOrRefPoint;
	CString	m_strBetaAngle;
	int		m_nNodeNumIndex;
	BOOL	m_bIntersectNode;
	BOOL	m_bIntersectElem;
	BOOL	m_nCheckOrtho;
	int		m_nPreParseType;

	BOOL	m_bVerticalType;
	mit::frx::MComboBox	m_cbxHeight;
	CEditUnit m_edtHeight;
	CTextUnit m_untHeigth;

	int m_nRefAngle;
	mit::frx::MComboBox  m_edtRefAngle;
	CTextUnit  m_untRefAngle;
	mit::frx::MComboBox  m_cmbRefAxis;

	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemCreateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void AlignControl_MEC();
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

// Implementation
protected:
	void AlignControls();
	void ChangeBitmap();
	int GetWallMode();
	BOOL GetSelectedStor(T_STOR_K& rKey);
	BOOL GetHeightVal(double& rdHeight);
	bool IsUserDefinedStory();
	BOOL IsVerticalElemType();
	void InitComboNucs();
	void ChangeText();
	BOOL m_bPropInit;
	BOOL m_bPlanePos;   // 3,4 Node 컨트롤 위치가 너무 왼쪽이라서 위치조정하는 Flag

	// Generated message map functions
	//{{AFX_MSG(CElemCreateDlg)
	afx_msg void OnTmMatButton();
	afx_msg void OnTmSecButton();
	afx_msg void OnTmThickButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTmElemTypeCombo();
	afx_msg void OnTmBetaAngleOrRefPointRadio();
	void OrientationEnableDisable(int nElemType);
	afx_msg void OnSetfocusTmNodesEdit();
	afx_msg void OnTmIntsectElemCheck();
	afx_msg void OnTmIntsectNodeCheck();
	afx_msg void OnTmClickNodeRadio();
	afx_msg void OnClickTensTrussRadio();
	afx_msg void OnClickCompTrussRadio();
	afx_msg void OnTmIntstTol();
	afx_msg void OnTmOrthoCheck();
	afx_msg void OnSelchangeTmNodesCobx();
	afx_msg void OnTmNodesEnterBtn();
	afx_msg void OnSelchangeTmWIdCombo();
	afx_msg void OnSelchangeTensCableCombo();
	afx_msg void OnTmClickTensLimit(); // MNET:XXXX 20070531 mylee
	afx_msg void OnTmClickCompLimit();
	afx_msg void OnTmClickWallType();
	afx_msg void OnTmClickVertiType();
	afx_msg void OnSelHeightCmb();
	afx_msg void OnTmOrientationRdo();
	afx_msg void OnTmRefDirCmb();
	afx_msg void OnTmRefButton();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMCREATEDLG_H__E3A50C03_6049_11D3_8899_0000C0A2329D__INCLUDED_)
