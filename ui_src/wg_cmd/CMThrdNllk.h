#if !defined(__WG_CMD_CMTHRDNLLK_H__)
#define __WG_CMD_CMTHRDNLLK_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdNllk.h : header file
//
#include "..\wg_db\wg_db_SelectLC.h"
#include "CMChildBarBase.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMThrdNllk dialog

class CCMThrdNllk : public CCMChildBarBase
{
// Construction
public:
	CCMThrdNllk(CWnd* pParent = NULL);   // standard constructor

	BOOL      m_bModify;
	T_THRD_D  m_pDlgData;
	CArray<T_NLNK_K,T_NLNK_K>m_arNllk;
	//CArray<int, int> m_arNllkSerial;
	//CArray<int, int> m_arSortedIdx;  
	
	void  MakeNllkKeyList(void);

public:
	virtual void Execute(){ OnCmdApply(); }
	virtual BOOL ExternalInit(UINT key);
	void  ChangeCompCbx(int nSubType);

	void  UpdateModeNumCtrl(void);
	int   GetThisLdcType(UINT ThisK); // 1:Mode Superposition  2:Direct Intergration
	void  EnableModeNumCtrl(BOOL bEnable);

// Dialog Data
	//{{AFX_DATA(CCMThrdNllk)
	enum { IDD = IDD_DBAR_THRD_NLLK };
	CSelectLC	m_cbxLComb;
	MComboBox	m_cbxNllk;
	MComboBox	m_cbxComp;
	CString	m_sName;
	int		m_nType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThrdNllk)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString strOldName;
	BOOL OnExecute();
	void DataInit();
	BOOL Dlg2Data();
	void Data2Dlg();
	int m_nComboType;

	// Generated message map functions
	//{{AFX_MSG(CCMThrdNllk)
	virtual BOOL OnInitDialog();
	afx_msg void OnGpsBarThtypeRadioPress();
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

#endif // !defined(__WG_CMD_CMTHRDNLLK_H__)
