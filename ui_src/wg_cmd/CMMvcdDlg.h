#if !defined(__CMMVCDDLG_H__)
#define __CMMVCDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvcdDlg.h : header file
//
#include "wg_cmd.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvcdDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvcdDlg : public CDialogMove
{
// Construction
public:
	CCMMvcdDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMMvcdDlg();

// Dialog Data
	//{{AFX_DATA(CCMMvcdDlg)
	enum { IDD = IDD_CMD_ML_MVCD };
	MComboBox	m_cobxCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvcdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitCodCombo();
	void MakeCodeData();
	int  GetCodeData(CString csCodeName);
	CString GetCodeName(int nCode);
	CString GetMoveCodeName(int nCodeID);

protected:
	CStringArray      m_aCodeName;
	CArray<int, int>  m_aCodeData;
	CDBDoc* m_pDoc;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvcdDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMvctOk();
	afx_msg void OnCmdMvctCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void  GetCodeNameList(CStringArray &aCodeName);
	void  GetCodeDataList(CArray<int, int> &aCodeData);
	int   GetInitCodeType();
	CString GetInitCodeName();
	BOOL  AddMvcd(CString strCodeName);
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCDDLG_H__)
