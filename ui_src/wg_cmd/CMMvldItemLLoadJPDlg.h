#if !defined(AFX_CMMVLDITEMLLOADJPDLG_H__40204725_AF40_475D_BE26_3539C04DCA1B__INCLUDED_)
#define AFX_CMMVLDITEMLLOADJPDLG_H__40204725_AF40_475D_BE26_3539C04DCA1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemLLoadJPDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemLLoadJPDlg dialog

class CCMMvldItemLLoadJPDlg : public CChildDialog
{
// Construction
public:
	CCMMvldItemLLoadJPDlg(CWnd* pParent = NULL);   // standard constructor

public:
	BOOL GetCurDlgData(T_MVLDjp_D* pData);
	void SetCurDlgData(T_MVLDjp_D* pData);

protected:
	void InitControlUnit();
	void InitLengthCombo();
	void InitMethodCombo();
	void SetData2Dlg();

protected:
	CDBDoc* m_pDoc;
	T_MVLDjp_D  m_Data;
	CStringArray arLengthCombo;
	CStringArray arMethodCombo;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemLLoadJPDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_LLOAD_JAPAN };
	CTextUnit	m_unitP2L1;
	CTextUnit	m_unitUnitLoad;
	CEditUnit	m_editUnitLoad;
	CTextUnit	m_unitShear;
	CEditUnit	m_editShear;
	CTextUnit	m_unitP2Small;
	CEditUnit	m_editP2Small;
	CTextUnit	m_unitP2Medium;
	CEditUnit	m_editP2Medium2;
	CEditUnit	m_editP2Medium1;
	CTextUnit	m_unitP2Large;
	CTextUnit	m_unitGrSmall;
	CEditUnit	m_editGrSmall;
	CTextUnit	m_unitGrMedium;
	CEditUnit	m_editGrMedium2;
	CEditUnit	m_editGrMedium1;
	CTextUnit	m_unitGrLarge;
	CEditUnit	m_editGrLarge;
	CEditUnit	m_editP2Large;
	CEditUnit	m_editP2L2;
	CEditUnit	m_editP2L1;
	CTextUnit	m_unitP2L;
	CTextUnit	m_unitMoment;
	CEditUnit	m_editMoment;
	CTextUnit	m_unitLength;
	MComboBox	m_cboMethod;
	MComboBox	m_cboLength;
	double	m_dRatio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemLLoadJPDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemLLoadJPDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDITEMLLOADJPDLG_H__40204725_AF40_475D_BE26_3539C04DCA1B__INCLUDED_)
