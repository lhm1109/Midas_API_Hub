// CRCDgnLcomDlg.h: interface for the CCRCDgnLcomDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRCDGNLCOMDLG_H__9963A28F_1A93_47BE_BC42_027D6A5DCE5E__INCLUDED_)
#define AFX_CRCDGNLCOMDLG_H__9963A28F_1A93_47BE_BC42_027D6A5DCE5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DialogMove.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCRCDgnLcomDlg : public CDialogMove
{
public:
	CCRCDgnLcomDlg(CWnd* pParent = NULL);   // standard constructor

 void Init_Data();

// Dialog Data
	//{{AFX_DATA(CDgnConMatDlg)
#if defined (_US) || defined (_RUS)
	enum { IDD = IDD_DGN_CRC_DESIGN_LCOM_US };
#else
	enum { IDD = IDD_DGN_CRC_DESIGN_LCOM };
#endif
	BOOL m_bDead;
	BOOL m_bDesign;
	BOOL m_bTemper;
	BOOL m_bEarth;
	BOOL m_bFinalA;
	BOOL m_bFinalB;
	BOOL m_bFinalC;
	//}}AFX_DATA

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif // !defined(AFX_CRCDGNLCOMDLG_H__9963A28F_1A93_47BE_BC42_027D6A5DCE5E__INCLUDED_)
