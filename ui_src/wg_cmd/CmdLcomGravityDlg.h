#if !defined(__CmdLcomGravityDlg_H__)
#define __CmdLcomGravityDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdLcomGravityDlg.h : header file
//

#include "..\mit_frx\MComboBox.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "LoadCombCtrl.h"
using namespace mit::frx;


/////////////////////////////////////////////////////////////////////////////
// CCmdLcomGravityDlg dialog

class CCmdLcomGravityDlg : public CDialogMove
{
// Construction
public:
	CCmdLcomGravityDlg(CWnd* pParent = NULL);   // standard constructor
	~CCmdLcomGravityDlg();

// Dialog Data
	//{{AFX_DATA(CCmdLcomGravityDlg)
	enum { IDD = IDD_CMD_LCOM_GRAVITY_DLG };
	//}}AFX_DATA

	// Attributes
public:
	void SetData(CArray<T_STLD_K, T_STLD_K>& aSelGravityLoad, CArray<T_STLD_K, T_STLD_K>& aUnselGravityLoad, CArray<_LCOM_GRAV_EFF, _LCOM_GRAV_EFF&>& aEffectiveLoad);
	void GetData(CArray<T_STLD_K, T_STLD_K>& aSelGravityLoad, CArray<T_STLD_K, T_STLD_K>& aUnselGravityLoad, CArray<_LCOM_GRAV_EFF, _LCOM_GRAV_EFF&>& aEffectiveLoad);
	CDBDoc* m_pDoc;

protected:
	CListCtrl	m_lstGrvSel       ;
	CListCtrl	m_lstGrvUnsel     ;
	CListCtrl	m_lstEffLoad      ;
	MComboBox m_cmbEffLoadCase  ;
	double    m_dEffLoadFactor  ;

protected:
// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnGrvLeftBtn   ();
	afx_msg void OnGrvRightBtn  ();
	afx_msg void OnEffLoadAddBtn();
	afx_msg void OnEffLoadModBtn();
	afx_msg void OnEffLoadDelBtn();

	void InitCtrl();
	void SetListCtrlHeader(CListCtrl& list, const int nColNum, const CString* aTitle, const int* nColWidth);
	void Data2Dlg();
	BOOL Dlg2Data();
	

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CArray<T_STLD_K, T_STLD_K> m_aSelGravityLoad;
	CArray<T_STLD_K, T_STLD_K> m_aUnselGravityLoad;
	CArray<_LCOM_GRAV_EFF, _LCOM_GRAV_EFF&> m_aEffectiveLoad;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CmdLcomGravityDlg_H__9E765C21_0CC3_11D4_888F_0000C0F30D4D__INCLUDED_)