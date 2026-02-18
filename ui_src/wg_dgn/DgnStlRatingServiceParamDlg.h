// DgnStlRatingServicePARAMDLG.h: interface for the CDgnStlRatingServiceParamDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DgnStlRatingServicePARAMDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_DgnStlRatingServicePARAMDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "DgnStlRatingServiceParamTabIDlg.h"
#include "DgnStlRatingServiceParamTabJDlg.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "DgnTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MButton.h"

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CDgnStlRatingServiceParamTabIDlg;
class CDgnStlRatingServiceParamTabJDlg;
class __MY_EXT_CLASS__ CDgnStlRatingServiceParamDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnStlRatingServiceParamDlg(CWnd* pParent = NULL);

	// Dialog Data
		//{{AFX_DATA(CDgnStlRatingServiceParamDlg)
	enum { IDD = IDD_DGN_STL_RATING_SERVICE_PARAM_DLG };
	
	int		m_nOption;
	int		m_nElemType;
	mit::frx::MButton m_Ctrl_BothIJ;
	CDlgTabCtrl	m_Tab;
	//}}AFX_DATA

	T_SRSP_TYPE m_I_Data;
	T_SRSP_TYPE m_J_Data;

	BOOL m_bBothIJ;
	int  m_nRatingCode;

private:
	CArray<UINT, UINT> m_aCtrl;           // Element Type 그룹박스 아래의 모든 컨트롤들
	CArray<UINT, UINT> m_aElemTypeCtrl;   // Element Type 그룹박스와 관련 컨트롤들 (숨김/표시용)
	CArray<UINT, UINT> m_aPositionCtrl;

public:
	void Initial_SelectItem();
	void SetDgnCode(int nDgnCode);
	int  GetDgnCode();
	void SetText();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDgnStlRatingServiceParamDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Implementation
protected:
	CDBDoc* m_pDoc;

	BOOL Dlg2Data(T_SRSP_TYPE* pTfType);
	void AlignControl();
	void ShowHideControls();

	T_SRSP_D m_Data;

	CDgnStlRatingServiceParamTabIDlg* m_pSubDlg_I;
	CDgnStlRatingServiceParamTabJDlg* m_pSubDlg_J;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlRatingServiceParamDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnClose();
// 	afx_msg void OnDgnExecute();
	afx_msg void OnDgnCPGLoadAppAddDel();
	afx_msg void OnBothIJChk();
	afx_msg void OnDgnCPGElemTypeRdo();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnStlRatingServicePARAMDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
