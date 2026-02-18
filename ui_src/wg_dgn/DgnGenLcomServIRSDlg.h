#if !defined(AFX_DGNGENLCOMSERVIRSDLG_H__)
#define AFX_DGNGENLCOMSERVIRSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DialogMove.h"
#include "HeaderPre.h"


/////////////////////////////////////////////////////////////////////////////
//CDgnGenLcomServIRSDlg dialog
struct _DGN_LCOM_D;
class __MY_EXT_CLASS__ CDgnGenLcomServIRSDlg : public CDialogMove
{
	// Construction
public:
	CDgnGenLcomServIRSDlg(CWnd* pParent = NULL);   // standard constructor


	// Dialog Data
	//{{AFX_DATA(CDgnGenLcomServIRSDlg)
	enum { IDD = IDD_DGN_GEN_SER_TYPE_AS5100_DLG}; // CDgnGenLcomServAS5100Dlg 이 클래스와 같은 리소스 사용 
	CListCtrl	m_aServLcomList;
	CListCtrl m_aServLcomSelectList;
	//}}AFX_DATA

protected:
	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arServLcomMAP;        // Serviceability Type
	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arServLcomSelectMAP;  // Selected Type

	void InitCtrl();
	void InitLcomData();
	void Write_ListCtrl(CListCtrl& LoadList, CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&>& LoadMap);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenLcomServIRSDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnGenLcomServIRSDlg)
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

#endif // !defined(AFX_DGNGENLCOMSERVIRSDLG_H__)
