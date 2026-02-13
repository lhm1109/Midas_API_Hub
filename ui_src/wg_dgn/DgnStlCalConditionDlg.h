#if !defined(__DgnStlCalConditionDlg_H__)
#define __DgnStlCalConditionDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlCalConditionDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h" 
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CDgnStlCalConditionDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnStlCalConditionDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnStlCalConditionDlg(CWnd* pParent = NULL);   // standard constructor
	
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Dialog Data
	//{{AFX_DATA(CDgnMPhiCurveConditionDlg)
	enum { IDD = IDD_DGN_STL_STDC_DLG };

	MComboBox m_cmbSection;
	BOOL m_bLBuckl;
	BOOL m_bBendSt;
	int  m_nBendSt;
	BOOL m_bGusset;
	int  m_nGusset;
	CListCtrl m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlCalConditionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitCombo();

	void Data2Dlg(T_STDC_K StdcK, T_STDC_D &Data);
	BOOL Dlg2Data(T_STDC_K& StdcK, T_STDC_D &Data);

	void SetHeaderTitle();
	void MakeItemEx();

	int  FindInsertionPos(int nID);	
	BOOL InsertItem(T_STDC_K Key, T_STDC_D &Data);
	BOOL DeleteItem(T_STDC_K Key, T_STDC_D &Data);
	BOOL ModifyItem(T_STDC_K KeyOld, T_STDC_K Key, T_STDC_D &Data);
	CString DataToStr(int i, T_STDC_K Key, T_STDC_D &Data);
	
	CDBDoc* m_pDoc;
	
	// Generated message map functions
	//{{AFX_MSG(CDgnStlCalConditionDlg)
	virtual BOOL OnInitDialog();
 	afx_msg void OnCalcSectCmb();
 	afx_msg void OnBendingStressChk();
	afx_msg void OnGussetAxisChk();
 	afx_msg void OnCalcConditionAddBtn();
 	afx_msg void OnCalcConditionModBtn();
 	afx_msg void OnCalcConditionDelBtn();
 	afx_msg void OnCalcConditionCloseBtn();
 	afx_msg void OnCalcConditionDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnStlCalConditionDlg_H__)
