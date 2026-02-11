#if !defined(__ElemEBeamCreateDlg_H__)
#define      __ElemEBeamCreateDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemEBeamCreateDlg.h : header file
//

// 유효보 생성 대화상자
// ElemCreateDlg와 기본동작은 같다.
// 4개의 폭 너비를 입력받아 Tapered section을 생성하여 사용

/////////////////////////////////////////////////////////////////////////////
// CElemEBeamCreateDlg dialog
#include "ElemCreateDlg.h"
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

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CDBDoc;
class CElemEBeamCreateDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CElemEBeamCreateDlg(CWnd* pParent = NULL);   // standard constructor
	~CElemEBeamCreateDlg();

public:
	void SetTempElemDataByCurSetting();
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
	BOOL GetKNodeDatas(int nElemType,CArray<unsigned int,unsigned int>& NodeKeyList,int &nBAngleOrRefP, 
										 double &BAngle,double& RPWx,double &RPWy,double &RPWz);
	BOOL GetMatlIDS(int nElemType,int& nMatID); 
	BOOL GetSectIDS(T_NODE_K NodeK, int& nSecID);
	BOOL GetParameter(_ElemCreate & EC);
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEscape(WPARAM wParam, LPARAM lParam);
	void InitCtrls();
	void InitPrebCmb();
	void UpdatePrebCmb();
	void InitNodeCtrl();
	void InitPropertyCtrls();
	virtual void Execute();
	BOOL GetDistance(CMouseEdit* pDistEdit, double &dblDist);
	  
	// Dialog Data
	//{{AFX_DATA(CElemEBeamCreateDlg)
	enum { IDD = IDD_TM_ELEM_EBEAM_CREATE };
	MButton	m_chkOrtho;
	CParseEdit	m_wndNodeUserEdit;
	MComboBox	m_cobxNodeEdit;
	MComboBox	m_cobxPreb;
	CBCGPStatic	m_wndNodesTitle;
	CBCGPStatic	m_wndMatTitle;
	CMouseEdit m_wndNodeListEdit;
	CCobxMatl	m_ctrlCobxMatl;
	CEditMatl	m_ctrlEditMatl;
	int		m_nNodeNumIndex;
	BOOL	m_bIntersectNode;
	BOOL	m_bIntersectElem;
	BOOL	m_nCheckOrtho;
	int		m_nPreParseType;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemEBeamCreateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

// Implementation
protected:
	void AlignControls();
	void InitUnit();

	CDBDoc* m_pDoc;
	CBitmap* m_pBitmap;
	BOOL m_bPropInit;

	// Generated message map functions
	//{{AFX_MSG(CElemEBeamCreateDlg)
	afx_msg void OnTmMatButton();
	afx_msg void OnTmPrebButton();
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnSetfocusTmNodesEdit();
	afx_msg void OnTmIntstTol();
	afx_msg void OnTmOrthoCheck();
	afx_msg void OnSelchangeTmNodesCobx();
	afx_msg void OnTmNodesEnterBtn();
	afx_msg void OnSelchangeTmSectCobx();
	afx_msg void OnSelchangePrebCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ElemEBeamCreateDlg_H__E3A50C03_6049_11D3_8899_0000C0A2329D__INCLUDED_)
