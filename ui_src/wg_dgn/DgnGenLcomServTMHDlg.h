#if !defined(AFX_DGNGENLCOMSERVTMHDLG_H__)
#define AFX_DGNGENLCOMSERVTMHDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DialogMove.h"
#include "HeaderPre.h"


/////////////////////////////////////////////////////////////////////////////
//CDgnGenLcomServTMHDlg dialog
struct _DGN_LCOM_D;
class __MY_EXT_CLASS__ CDgnGenLcomServTMHDlg : public CDialogMove
{
	// Construction
public:
	CDgnGenLcomServTMHDlg(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CDgnGenLcomServTMHDlg)
	enum { IDD = IDD_DGN_GEN_SER_TYPE_TMH_DLG };

	CListCtrl m_aUnSelList;
	CListCtrl m_aComb1List;
	CListCtrl m_aComb2List;

	//}}AFX_DATA

protected:
	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_mUnSel;

	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_mComb1;
	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_mComb2;
	
	void InitCtrl();
	void InitLcomData();
	void Write_ListCtrl(CListCtrl& LoadList, CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&>& LoadMap);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenLcomServTMHDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnGenLcomServTMHDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDgnAddComb1Btn();
	afx_msg void OnDgnDelComb1Btn();
	afx_msg void OnDgnAddComb2Btn();
	afx_msg void OnDgnDelComb2Btn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_DGNGENLCOMSERVBSDLG_H__)
