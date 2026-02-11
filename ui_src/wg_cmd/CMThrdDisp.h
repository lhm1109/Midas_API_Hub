#if !defined(__WG_CMD_CMTHRDDISP_H__)
#define __WG_CMD_CMTHRDDISP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdDisp.h : header file
//
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "CMChildBarBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMThrdDisp dialog

class CCMThrdDisp : public CCMChildBarBase
{
// Construction
public:
	CCMThrdDisp(CWnd* pParent = NULL);   // standard constructor

	T_THRD_D	m_pDlgData;
	BOOL      m_bModify;
	
	virtual void	Execute() { OnCmdApply(); };
	virtual BOOL ExternalInit(UINT key);

	void  UpdateModeNumCtrl(void);
	void  UpdateAntNodeCtrl(int nType);
	void  UpdateRefNodeCtrl(void);
	int   GetThisLdcType(UINT ThisK); // 1:Mode Superposition  2:Direct Intergration
	void  EnableModeNumCtrl(BOOL bEnable);

	void  SetDlgData(T_THRD_D&	DlgData) { m_pDlgData = DlgData; }
			
// Dialog Data
	//{{AFX_DATA(CCMThrdDisp)
	enum { IDD = IDD_DBAR_THRD_DISP };
	CSelectLC	m_cbxLComb;
	MComboBox	m_ModNumCb;
	MComboBox	m_CompCb;
	CMouseEdit	m_NodePickEd;
	CString	m_KeyNdEl;
	CString	m_Name;
	int		m_Type;
	int		m_ModNum;
	int		m_nRefType;
	CString	m_sRefNode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThrdDisp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString strOldName;
	BOOL IsMutiSupportThis(UINT nLKey);
	void DataInit();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL OnExecute();

	// Generated message map functions
	//{{AFX_MSG(CCMThrdDisp)
	virtual BOOL OnInitDialog();
	afx_msg void OnGpsBarThmodnum();
	afx_msg void OnSelchangeGpsBarThloadcase();
	afx_msg void OnGpsBarThtypeofResult();
	afx_msg void OnGpsBarThtypeRef();
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WG_CMD_CMTHRDDISP_H__)
