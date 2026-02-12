#if !defined(__WG_CMD_CMTHRDDISPSTORY_H__)
#define __WG_CMD_CMTHRDDISPSTORY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\mit_frx\MComboBox.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "CMChildBarBase.h"
using namespace mit::frx;

class CCMThrdDispStory : public CCMChildBarBase
{
// Construction
public:
	CCMThrdDispStory(CWnd* pParent = NULL);   // standard constructor

	T_THRD_D	m_pDlgData;
	BOOL      m_bModify;
	
	virtual void	Execute(){ OnCmdApply(); }
	virtual BOOL ExternalInit(UINT key);

	void  UpdateRefNodeCtrl(void);

	void  SetDlgData(T_THRD_D&	DlgData) { m_pDlgData = DlgData; }
			
// Dialog Data
	//{{AFX_DATA(CCMThrdDispStory)
	enum { IDD = IDD_DBAR_THRD_DISP_STORY };
	MComboBox	m_RefTypeCb;
	CSelectLC	m_cbxLComb;
	MComboBox	m_CompCb;
	MComboBox m_cobxStor;
	CString	m_Name;
	int		m_Type;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThrdDispStory)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString strOldName;
	BOOL IsMutiSupportThis(UINT nLKey);
	void InitCobxStor();
	void DataInit();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL OnExecute();
	void MakeRefTypeCmb(BOOL bAddRelative);

	// Generated message map functions
	//{{AFX_MSG(CCMThrdDispStory)
	virtual BOOL OnInitDialog();
	afx_msg void OnGpsBarThmodnum();
	afx_msg void OnSelchangeGpsBarThloadcase();
	afx_msg void OnGpsBarThtypeofResult();
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WG_CMD_CMTHRDDISPSTORY_H__)
