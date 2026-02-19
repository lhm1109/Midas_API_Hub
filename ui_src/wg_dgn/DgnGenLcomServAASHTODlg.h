#if !defined(AFX_DGNGENLCOMSERVAASHTODLG_H__)
#define AFX_DGNGENLCOMSERVAASHTODLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DialogMove.h"
#include "HeaderPre.h"


/////////////////////////////////////////////////////////////////////////////
//CDgnGenLcomServAASHTODlg dialog
struct _DGN_LCOM_D;
class __MY_EXT_CLASS__ CDgnGenLcomServAASHTODlg : public CDialogMove
{
	// Construction
public:
	CDgnGenLcomServAASHTODlg(CWnd* pParent = NULL);   // standard constructor
	
		
	// Dialog Data
	//{{AFX_DATA(CDgnGenLcomServAASHTODlg)
	enum { IDD = IDD_DGN_GEN_SER_AASHTO_DLG};
	CListCtrl	m_aServLcomList;
	CListCtrl m_aServLimit1List;
	CListCtrl m_aServLimit3List;

//   CListCtrl	m_aRareList;
//   CListCtrl	m_aQusaiList;
//   CListCtrl	m_aFrequentList;
	//}}AFX_DATA
	
protected:
	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arServLcomMAP;

	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arServLimit1MAP;  // m_arQuasiLcomMAP
	CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arServLimit3MAP;  // m_arRareLcomMAP

//   CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arQuasiLcomMAP;
//   CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arFrequentLcomMAP;
//   CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&> m_arRareLcomMAP;
	
	 void InitCtrl();
	 void InitLcomData();
	 void Write_ListCtrl(CListCtrl& LoadList, CMap<int, int, _DGN_LCOM_D, _DGN_LCOM_D&>& LoadMap);
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenLcomServAASHTODlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDgnGenLcomServAASHTODlg)
	afx_msg void OnDgnAddLimit1Btn();
	afx_msg void OnDgnDelLimit1Btn();
	afx_msg void OnDgnAddLimit3Btn();
	afx_msg void OnDgnDelLimit3Btn();
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_DGNGENLCOMSERVAASHTODLG_H__)
