#if !defined(__WG_CMD_CMTHRDSOLID_H__)
#define __WG_CMD_CMTHRDSOLID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdSolid.h : header file
//
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "CMChildBarBase.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMThrdSolid dialog

class CCMThrdSolid : public CCMChildBarBase
{
// Construction
public:
	CCMThrdSolid(CWnd* pParent = NULL);   // standard constructor

	T_THRD_D		m_pDlgData;
	BOOL m_bModify;

	virtual void	Execute(){ OnCmdApply(); }
	virtual BOOL ExternalInit(UINT key);
	int	m_nDlgMode; // = m_nContainerMode 0: TimeHistory, 1: Nonlinear  2: Pushover
	void  SetPosCompList(int nType,BOOL bDefault=FALSE);
// Dialog Data
	//{{AFX_DATA(CCMThrdSolid)
	enum { IDD = IDD_DBAR_THRD_SOLID };
	CSelectLC	m_cbxLComb;
	MComboBox	m_PointCb;
	MComboBox	m_ModNumCb;
	CMouseEdit	m_ElemPickEd;
	MComboBox	m_CompCb;
	CString	m_Name;
	CString	m_KeyNdEl;
	int		m_ModNum;
	int		m_Type;
	int   m_nGlobal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThrdSolid)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString strOldName;
	void DataInit();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL OnExecute();

	// Generated message map functions
	//{{AFX_MSG(CCMThrdSolid)
	virtual BOOL OnInitDialog();
	afx_msg void OnGpsBarThType();
	afx_msg void OnGpsBarThmodnum();
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WG_CMD_CMTHRDSOLID_H__)
