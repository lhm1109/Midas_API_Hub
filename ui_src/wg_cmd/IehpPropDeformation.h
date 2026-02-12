#if !defined(__IEHPPROPDEFORMATIONDLG_H__)
#define __IEHPPROPDEFORMATIONDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\wg_db_EditUnit.h"
#include "wg_cmdRes2.h"

class CIehpPropDeformation : public CDialogMove
{
	// Construction
public:
	CIehpPropDeformation(int nSymmetry, int nModelType, int nDeformDefineType, int nDof, T_IEHP_PROP* PROP, CWnd* pParent = NULL);   // standard constructor

	//-----------------------------------------------------------------------
	// Interfaces
	//-----------------------------------------------------------------------
public:
	enum { IDD = IDD_ETC_IEHP_PROP_DEFORMATION};
public:
	void Data2Dlg();
	BOOL Dlg2Data();
	int  GetDeformDefineType() { return m_nDeformDefineType; }
protected:
	int m_nDeformDefineType;
	int m_nSymmetry;
	int m_nModelType;
	int m_nDof;
	BOOL m_bCompOnly;
	CEditUnit m_edtDeformCapacity[2][5];
	T_IEHP_PROP* m_pPROP;
	// Generated message map functions
	//{{AFX_MSG(CIehpItemDlg)
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnChangeDefomCapacity();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__IEHPITEMDLG_H__)
