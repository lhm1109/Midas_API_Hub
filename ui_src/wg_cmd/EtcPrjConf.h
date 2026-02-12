#if !defined(__ETCPRJCONF_H__)
#define __ETCPRJCONF_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EtcPrjConf.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "../MIT_frx/MEdit.h"

using namespace mit;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CEtcPrjConf dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CEtcPrjConf : public CDialogMove
{
// Construction
public:
	CEtcPrjConf(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEtcPrjConf)
	enum { IDD = IDD_ETC_PRJ_CONF };
	CString	m_csPrjName;
	CString	m_csUsrName;
	CString	m_csRevision;
	CString	m_csEmail;
	CString	m_csAddress;
	CString	m_csTelephone;
	CString	m_csFax;
	CString	m_csClient;
	CString	m_csTitle;
	CString	m_csFileName;
	CString	m_csCreated;
	CString	m_csDirectory;
	CString	m_csModified;
	CString	m_csFileSize;
	CString	m_csEngName;
	CString	m_csEngDate;
	CString	m_csChk1Name;
	CString	m_csChk1Date;
	CString	m_csChk2Name;
	CString	m_csChk2Date;
	CString	m_csChk3Name;
	CString	m_csChk3Date;
	CString	m_csAprName;
	CString	m_csAprDate;
	CString	m_csComment;
	//}}AFX_DATA

public:
	void SetProjectName(const CString& csName);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEtcPrjConf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitDialogData();

protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CEtcPrjConf)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnEtcPcBtnDetail();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ETCPRJCONF_H__)
