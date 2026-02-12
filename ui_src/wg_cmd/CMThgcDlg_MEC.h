#if !defined(__CMThgcDlg_MEC_H__)
#define __CMThgcDlg_MEC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMNtgcDlg.h : header file
//

#include "../wg_db/wg_db_DBDoc.h"
#include "../wg_db/wg_db_SelectLC.h"
#include "../wg_db/EditUnit.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMThgcDlg_MEC dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMThgcDlg_MEC : public CDialogMove
{
	// Construction
public:
	CCMThgcDlg_MEC(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CCMThgcDlg_MEC)
	enum { IDD = IDD_ETC_THGC_DLG_MEC };
	CSelectLC	m_cmbStld;
	CListCtrl	m_lstLCase;
	CEditUnit m_edtScale;
	MButton   m_chkConsiderIgnoreElem;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThgcDlg_MEC)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	T_THGC_D m_ThgcD;
	int	 m_nGeomNonlinearType;
	int	 m_nInitLoadMethod;
	void Data2Dlg();
	void Dlg2Data();
	void SetLoadCaseCmb();
	void SetListCtrlHeader();
	BOOL GetSelectedData(int& nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_TGLC_BASE& data);
	void SetItem(int nIndex, T_TGLC_BASE& data);
	BOOL Dlg2TglcBase(T_TGLC_BASE& data);
	BOOL TglcBase2Dlg(T_TGLC_BASE& data);
	void EnableConsiderIgnoreElem();
	void SetCheckGeomNonlinearType(int nGeomNonlinearType);
	void GetCheckGeomNonlinearType(int &nGeomNonlinearType);
	BOOL m_bCivil;

	CArray<UINT, UINT> m_aCtrl_InitLoadMethod;
	CArray<UINT, UINT> m_aCtrl_GeomNonlinearType;
	CArray<UINT, UINT> m_aCtrl_SeqLoadingType;

	// Generated message map functions
	//{{AFX_MSG(CCMThgcDlg_MEC)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnGeomNonlinearType();
	afx_msg void OnRdoInitLoadMethod();
	afx_msg void OnPushoverInitialLoadAdd();
	afx_msg void OnPushoverInitialLoadDelete();
	afx_msg void OnPushoverInitialLoadModify();
	afx_msg void OnBtnPointSpringSupport();
	afx_msg void OnChangeSelList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnIncrStep();
	afx_msg void OnBtnIterParam();
	afx_msg void OnThgcRemove();
	afx_msg void OnBtnWall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMThgcDlg_MEC_H__)
