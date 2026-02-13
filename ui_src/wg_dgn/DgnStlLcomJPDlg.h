#if !defined(AFX_DgnStlLcomJPDlg_H__)
#define AFX_DgnStlLcomJPDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\EditUnit.h"
#include "DgnGenLcomServTypeDlg.h"

#include "HeaderPre.h"

enum
{
	UNSELECT = 0,
	SELECT = 1
};


/////////////////////////////////////////////////////////////////////////////
//CDgnStlLcomJPDlg dialog
struct _DGN_LCOM_D;
class __MY_EXT_CLASS__ CDgnStlLcomJPDlg : public CDialogMove
{
	// Construction
public:
	CDgnStlLcomJPDlg(CWnd* pParent = NULL);   // standard constructor
	
		
	// Dialog Data
	//{{AFX_DATA(CDgnStlLcomJPDlg)
	enum { IDD = IDD_DGN_STL_LCOM_JP_DLG};
	CListCtrl m_aServLcomList;
	CListCtrl m_aServLcomSelectList;

	CEditUnit m_edtFactor;
	//}}AFX_DATA
	
protected:
	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arServLcomMAP;        // Serviceability Type
	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arServLcomSelectMAP;  // Selected Type

	 void InitCtrl();
	 void InitLcomData();
	 void Write_ListCtrl(CListCtrl& LoadList, CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&>& LoadMap);
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlLcomJPDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDgnStlLcomJPDlg)
	afx_msg void OnDgnAddSelectBtn();
	afx_msg void OnDgnDelSelectBtn();
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_DgnStlLcomJPDlg_H__)
