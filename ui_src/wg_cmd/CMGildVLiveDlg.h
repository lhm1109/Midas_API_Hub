#if !defined(__CMGILDVLIVEDLG_H__)
#define __CMGILDVLIVEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;
class CDBDoc;
class CCMGildBaseGrid;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMGildVLiveDlg : public CDialogMove
{
// Construction
public:
	CCMGildVLiveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMGildVLiveDlg();

	void SetLoadCase(T_GILD_K LoadCaseKey);

// Dialog Data
	//{{AFX_DATA(CCMGildVLiveDlg)
	enum { IDD = IDD_CMD_GRID_GILD_VLIVE};
	CEdit	m_editName;
	MComboBox m_cobxType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGildVLiveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDBDoc* m_pDoc;
	CCMGildBaseGrid* m_wndGrid;

	BOOL m_bModify;
	T_GILD_K m_Key;
	T_GILD_D m_Data;

	void InitTypeCombo();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMGildVLiveDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGILDVLIVEDLG_H__)
