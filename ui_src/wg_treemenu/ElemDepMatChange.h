#if !defined(AFX_ELEMDEPMATCHANGE_H__0BF4F024_2D6F_484B_850E_1AA008D6BC80__INCLUDED_)
#define AFX_ELEMDEPMATCHANGE_H__0BF4F024_2D6F_484B_850E_1AA008D6BC80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemDepMatChange.h : header file
//
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\MIT_frx\MComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CElemDepMatChange dialog

class CElemDepMatChange : public CMenuBarChildDlg
{
// Construction
public:
	CElemDepMatChange(CWnd* pParent = NULL);   // standard constructor
	virtual ~CElemDepMatChange();
	
	virtual void Execute();
	
	void OptionCtrlMan();
	void AddDelCtrlMan();

// Dialog Data
	//{{AFX_DATA(CElemDepMatChange)
	enum { IDD = IDD_TM_ELEM_CHANGE_DEP_MAT };
	mit::frx::MComboBox	m_novCode;
	CTextUnit	  m_LengthUnit;
	CEditUnit	m_wndHEdit;
	CEditUnit m_wndAEdit;
	mit::frx::MComboBox	m_cboCode;
	int		m_nOption;
	int		m_nAddOrDel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemDepMatChange)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitCodeCombo();
	void InitNovolCombo();
	void AlignControls();
	void AdjustComboListBox();
	void ChangeFomulaByCode();
	void ChangeChildByCode();
	void AdjustNovolListBox();
	int  GetTypeComboData();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CElemDepMatChange)
	afx_msg void OnOptionRadio();
	virtual BOOL OnInitDialog();
	afx_msg void OnTmAddDelRadio();
	afx_msg void OnSelchangeTmCodeCombo();
	afx_msg void OnSelchangeTmNovolCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMDEPMATCHANGE_H__0BF4F024_2D6F_484B_850E_1AA008D6BC80__INCLUDED_)
