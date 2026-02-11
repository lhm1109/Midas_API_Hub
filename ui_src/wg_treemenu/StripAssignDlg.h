#if !defined(__StripAssignDlg_H__)
#define      __StripAssignDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StripAssignDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStripAssignDlg dialog
#include "ElemCreateDlg.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CStripAssignDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CStripAssignDlg(CWnd* pParent = NULL);   // standard constructor
	~CStripAssignDlg();

public:
	virtual void Execute();
	  
	// Dialog Data
	//{{AFX_DATA(CStripAssignDlg)
	enum { IDD = IDD_TM_STRIP_ASSIGN };
	//}}AFX_DATA

	CMouseEdit m_wndNodeListEdit;
	MComboBox m_cmbPrcs;
	int m_nByNodeElem; // 0:By node, 1:By element
	int m_nAddDel; // 0:Add/Replace, 1:Delete
	BOOL	m_bIntersectNode;
	BOOL	m_bIntersectElem;
	BOOL	m_nCheckOrtho;

	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEscape(WPARAM wParam, LPARAM lParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStripAssignDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

// Implementation
protected:
	CArray<UINT,UINT> m_aCtrlByNode;
	CArray<UINT,UINT> m_aCtrlByElem;

	CDBDoc* m_pDoc;
	void InitPrcsCmb();
	void UpdatePrcsCmb();
	void EnableDisableCtrls();
	void OnClickByNodeRdo();
	void OnClickByElemRdo();
	void OnTmOrthoCheck();
	void InitNodeCtrl();

	BOOL GetNodeListAndNodeNum(int nElemType, CArray<unsigned int,unsigned int>& _NodeList, int& nMaxNodeNum);
	BOOL GetKNodeDatas(int nElemType,CArray<unsigned int,unsigned int>& NodeKeyList,int &nBAngleOrRefP, 
										 double &BAngle,double& RPWx,double &RPWy,double &RPWz);
	void SetTempElemDataByCurSetting();
	void _CreateElemStrip();
	BOOL GetParameter(_ElemCreate & EC);

	// Generated message map functions
	//{{AFX_MSG(CStripAssignDlg)
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnTmPrcsButton();
	afx_msg void OnSetfocusTmNodesEdit();
	afx_msg void OnTmIntstTol();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_StripAssignDlg_H__E3A50C03_6049_11D3_8899_0000C0A2329D__INCLUDED_)
