// DgnPSCSelectPrintUnitDlg.h: interface for the CDgnPSCSelectPrintUnitDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DgnPSCSelectPrintUnitDlg_H__)
#define AFX_DgnPSCSelectPrintUnitDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnPSCSelectPrintUnitDlg : public CDialogMove
{
public:
	CDgnPSCSelectPrintUnitDlg(CWnd* pParent = NULL);   // standard constructor

	enum Member_Type
	{
		EN_MEMBER_PSC = 0,
		EN_MEMBER_CSG
	};

 void Init_Data();

// Dialog Data
	//{{AFX_DATA(CDgnConMatDlg)
	enum { IDD = IDD_DGN_PSC_PRINT_SELECT_UNIT };

	MComboBox m_CmbSelUnit;
	MComboBox m_CmbSelLang;
	//}}AFX_DATA

	void Initial_Data();
	int GetSelectUnit();

protected:
	int m_iSelUnit;
	int m_iSelLang;
	int m_nDesignType;  // 0:PSC, 1: CSD

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConMatDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnSelchangeUnit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetDesignType(int nDesignType);

};

#include "HeaderPost.h"

#endif // !defined(AFX_DgnPSCSelectPrintUnitDlg_H__)
