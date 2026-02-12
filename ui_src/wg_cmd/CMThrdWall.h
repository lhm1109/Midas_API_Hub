#if !defined(__WG_CMD_CMTHRDWALL_H__)
#define __WG_CMD_CMTHRDWALL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdBeam.h : header file
//

#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "CMChildBarBase.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMThrdWall dialog

class CCMThrdWall : public CCMChildBarBase
{
// Construction
public:
	CCMThrdWall(CWnd* pParent = NULL);   // standard constructor

	T_THRD_D		m_pDlgData;
	BOOL        m_bModify;

	void  UpdateModeNumCtrl(void);
	int   GetThisLdcType(UINT ThisK); // 1:Mode Superposition  2:Direct Intergration
	void  EnableModeNumCtrl(BOOL bEnable);
	virtual void	Execute(){ OnCmdApply(); }
	virtual BOOL ExternalInit(UINT key);
// Dialog Data
	//{{AFX_DATA(CCMThrdWall)
	enum { IDD = IDD_DBAR_THRD_WALL };
	CSelectLC	m_cbxLComb;
	MComboBox	m_PointCb;
	MComboBox	m_ModNumCb;
	CMouseEdit	m_ElemPickEd;
	MComboBox	m_CompCb;
	CString	m_Name;
	CString	m_KeyNdEl;
	int		m_ModNum;
	int		m_Type;
	BOOL	m_bCominedAxial;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThrdWall)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString strOldName;

	BOOL OnExecute();
	BOOL Dlg2Data();
	void DataInit();
	void Data2Dlg();
	void MakeMenuCombo(int nType);

	// Generated message map functions
	//{{AFX_MSG(CCMThrdWall)
	virtual BOOL OnInitDialog();
	afx_msg void OnGpsBarThType();
	afx_msg void OnGpsBarThmodnum();
	afx_msg void OnSelchangeGpsBarThcomp();
	afx_msg void OnSelchangeGpsBarThloadcase();
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WG_CMD_CMTHRDBEAM_H__)
