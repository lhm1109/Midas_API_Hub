#if !defined(__CMThgcDlg_H__)
#define __CMThgcDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThgcDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMThgcDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMThgcDlg : public CDialogMove
{
	// Construction
public:
	CCMThgcDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CCMThgcDlg)
	enum { IDD = IDD_ETC_THGC_DLG };
	BOOL m_bEnergyResult;
	BOOL m_bSdvi        ;
	BOOL m_bSdve        ;
	BOOL m_bSdst        ;
	BOOL m_bSdhy        ;
	BOOL m_bSdis        ;
	BOOL m_bMSSStatus   ;
	CSelectLC	m_cmbStld;
	CListCtrl	m_lstLCase;
	CEditUnit m_edtScale;
	MButton   m_chkConsiderIgnoreElem;
	int m_nHingeOutput;
	BOOL m_bFiberSameHinge;
	int m_nFiberOutput;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThgcDlg)
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
	BOOL GetSelectedData(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_TGLC_BASE &data);
	void SetItem(int nIndex, T_TGLC_BASE &data);
	BOOL Dlg2TglcBase(T_TGLC_BASE &data);
	BOOL TglcBase2Dlg(T_TGLC_BASE &data);
	void EnableConsiderIgnoreElem();
	void UpdateFiberOutputCtrl();

	CArray<UINT, UINT> m_aCtrl_InitLoadMethod;
	CArray<UINT, UINT> m_aCtrl_GeomNonlinearType;
	CArray<UINT, UINT> m_aCtrl_SeqLoadingType;
	BOOL m_bCivil;
	// Generated message map functions
	//{{AFX_MSG(CCMThgcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnGeomNonlinearType();
	afx_msg void OnRdoInitLoadMethod();
	afx_msg void OnPushoverInitialLoadAdd();
	afx_msg void OnPushoverInitialLoadDelete();
	afx_msg void OnPushoverInitialLoadModify();
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

#endif // !defined(__CMThgcDlg_H__)
