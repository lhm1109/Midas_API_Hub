#if !defined(__ELEMCREATECONVLINEDLG_H__)
#define      __ELEMCREATECONVLINEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemCreateConvLineDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CElemCreateConvLineDlg dialog
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
#include "..\wg_db\wg_db_SelectCtrl.h"

#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"
//#include "..\mit_frx\MSe"

class _CLElemCreate
{
public:
	int m_nElemType;
	
	int m_nSecID;
	int m_nMatID;
	int m_nThkID;

	int m_nBAngleOrRefPoint;
	double m_fBAngle;
	double m_RPWx,m_RPWy,m_RPWz;

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

	_CLElemCreate()
	{
		m_nElemType = -1;
		m_nSecID    = 0;
		m_nMatID    = 0;
		m_nThkID    = 0;

		m_nBAngleOrRefPoint = -1;
		m_fBAngle = 0.0;
		m_RPWx = m_RPWy = m_RPWz = 0.0;

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
	}
};

struct ST_ED_NODE_K
{
	UINT StNodeK;
	UINT EdNodeK;
	void Initialize()
	{
		StNodeK = 0;
		EdNodeK = 0;
	}
};

class CElemCreateConvLineDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
protected:
	static int m_nCurETypeIndex;

// Construction
public:
	CElemCreateConvLineDlg(CWnd* pParent = NULL);   // standard constructor
	~CElemCreateConvLineDlg();

protected:
	CArray<UINT, UINT> m_SectCtrls;
	CArray<UINT, UINT> m_ThickCtrls;
	
	CArray<UINT, UINT> m_BAngleCtrls;  

	CArray<int,int> m_ElemTypeIDs;
	
	BOOL  m_bIsParamError;
public:
	
	virtual void Execute();
	BOOL GetParameter(_CLElemCreate & EC);  

	void InitElemType();
	void InitPropertyCtrls();

	void _CreateElement();
	int  GetCurElemTypeID();// 현재 설정된 Element의 Type ID를 얻어낸다.  

	BOOL GetNodeListAndNodeNum(int nElemType,
														 CArray<unsigned int,unsigned int>& _NodeList,
														 int& nMaxNodeNum);
	BOOL GetKNodeDatas(int nElemType,CArray<unsigned int,unsigned int>& NodeKeyList,int &nBAngleOrRefP, 
										 double &BAngle,double& RPWx,double &RPWy,double &RPWz);
	BOOL GetPropIDS(int nElemType,int& nMatID,int& nSecID,int& nThkID);

	int GetStEdNodeKList(CArray<ST_ED_NODE_K, ST_ED_NODE_K&>& arStEdNodeK);
	BOOL SameConnectElement(T_ELEM_K ElemK, ST_ED_NODE_K StEdNodeK);
	BOOL OverLapStEdNodeK(CArray<ST_ED_NODE_K, ST_ED_NODE_K&>& arStEdNodeK, ST_ED_NODE_K NewStEdNodeK);
	  
	// Dialog Data
	//{{AFX_DATA(CElemCreateConvLineDlg)
	enum { IDD = IDD_TM_ELEM_CONV_LINE_CREATE };
	mit::frx::MButton	m_radioTruss;
	mit::frx::MButton	m_radioBeam;
	CEditSect	m_ctrlEditSect;
	CEditMatl	m_ctrlEditMatl;
	CCobxSect	m_ctrlCobxSect;
	CCobxMatl	m_ctrlCobxMatl;
	mit::frx::MComboBox	m_wndBAngleEdit;
	CString	  m_strBetaAngle;
	CString	m_SelectElemString;
	CSelectEdit m_editSelectElem;
// 	CSeparator	m_wndOriTitle;
// 	CSeparator	m_wndSecTitle;
// 	CSeparator	m_wndMatTitle;
// 	CSeparator	m_wndElemTitle;
	mit::frx::MButton	m_chkPeriphery;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemCreateConvLineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnTmMatButton();
	afx_msg void OnTmSecButton();
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

// Implementation
protected:

	BOOL m_bPropInit;
	
	// Generated message map functions
	//{{AFX_MSG(CElemCreateConvLineDlg)
	virtual BOOL OnInitDialog();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMCREATECONVLINEDLG_H__)
