#if !defined(__CMGROUPDAMPINGOPTIONDLG_H__)
#define __CMGROUPDAMPINGOPTIONDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGroupDampingOptionDlg.h : header file

class CDBDoc;
struct T_GRDP_D;
/////////////////////////////////////////////////////////////////////////////
// CCMGroupDampingOptionDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMGroupDampingOptionDlg : public CDialogMove
{
// Construction
public:
	CCMGroupDampingOptionDlg(CWnd* pParent = NULL);                      // standard constructor
	CCMGroupDampingOptionDlg(int nType, T_GRDP_D* pGrdpD, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CCMGroupDampingOptionDlg)
	enum { IDD = IDD_CMD_PROP_GRDP_OPT_DLG };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGroupDampingOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	T_GRDP_D* m_pData;

	int m_nType;      // 0 : Element Mass & Stiffness Proportiona, 1 : Strain Energy Damping

	CArray<UINT,UINT> m_aGroupRdo;
	CArray<UINT,UINT> m_aValueRdo;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMGroupDampingOptionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGROUPDAMPINGOPTIONDLG_H__)
