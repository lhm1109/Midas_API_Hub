#if !defined(__DGNCOMPOSITEMATDLG_H__)
#define __DGNCOMPOSITEMATDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConMatDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

#include "HeaderPre.h"

using namespace mit::frx;
class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg dialog

class __MY_EXT_CLASS__ CDgnCompositeMatDlg : public CDialogMove
{
	// Construction
public:
	CDgnCompositeMatDlg(BOOL bPscDgn=FALSE, CWnd* pParent = NULL);   // standard constructor
	// Dialog Data
	//{{AFX_DATA(CDgnConMatDlg)
	enum { IDD = IDD_DGN_CON_COMPOSITE_MAT_DLG };

	CListCtrl	m_ListCtrl;

	// 거더
	MComboBox	m_cmbConCodeGr;
	MComboBox	m_cmbConMatNameGr;

	CEditUnit m_edtFcGr;  
	CEditUnit m_edtFyGr ; 
	CEditUnit m_edtFysGr ; 
	CEditUnit m_edtLambdaGr;
	CEditUnit   m_edtTransferGr;
	CTextUnit	m_FcUnitGr;
	CTextUnit	m_FyUnitGr;
	CTextUnit	m_FysUnitGr;
	CTextUnit	m_TransferGrUnit;
	MButton     m_chkLambdaGr;
	MButton     m_chkTransferGr;
	MComboBox	m_cmbRebarCodeGr;
	MComboBox	m_cmbMainBarNamGr;
	MComboBox	m_cmbSubBarNameGr;
	MEdit m_edtMatNameGr;      
	MEdit m_edtSubBarNameGr;   
	MEdit m_edtMainBarNameGr;  

	// 슬래브
	MComboBox	m_cmbConCodeSl;
	MComboBox	m_cmbConMatNameSl;

	CEditUnit m_edtFcSl ; 
	CEditUnit m_edtFySl ; 
	CEditUnit m_edtFysSl ; 
	CEditUnit m_edtLambdaSl;
	CTextUnit	m_FcUnitSl;
	CTextUnit	m_FyUnitSl;
	CTextUnit	m_FysUnitSl;
	MButton   m_chkLambdaSl;
	MComboBox	m_cmbRebarCodeSl;
	MComboBox	m_cmbMainBarNamSl;
	MComboBox	m_cmbSubBarNameSl;

	MEdit m_edtMatNameSl;      
	MEdit m_edtSubBarNameSl;   
	MEdit m_edtMainBarNameSl;  

public:  
	void InitCtrls();
	BOOL Data2Dlg();
	void AlignControl();

	void Init_Items();
	void Init_ItemsSlab();
	void Init_ListCtrl();
	void Init_Unit();
	void Init_Data();
	void Write_ListBox(int iIndex, CString strID, CString strNa, CString strConNameGr, CString strMbarGr, CString strSbarGr, CString strConNameSl, CString strMbarSl, CString strSbarSl);
	BOOL GetSelectedItem(int &nIndex);
	void Init_CodeCombo();
	void Init_NameCombo();
	void Init_RebarNameCombo(CString strRebarCode, T_MATD_D mData);
	void Init_MatlNameCombo(CString strCode, CString strNa);
	void Init_CodeComboSlab();
	void Init_NameComboSlab();
	void Init_RebarNameComboSlab(CString strRebarCode, T_MATD_D mData);
	void Init_MatlNameComboSlab(CString strCode, CString strNa);

	void Set_DlgData(T_MATD_D& mData);

	void DgnConcmatModify(int nItem); 
	BOOL ErrorCheck();
	void ChangeDlgItemInfo();
	void EnableEditBox(int nCheck);
	void EnableRebarEditBox(int nCheck);

	void DgnConcmatModifySlab(int nItem);
	BOOL ErrorCheckSlab();
	void ChangeDlgItemInfoSlab();
	void EnableEditBoxSlab(int nCheck);
	void EnableRebarEditBoxSlab(int nCheck);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConMatDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc* m_pDoc;
	CString m_strCurrentItem;
	T_MATD_D m_Data;
	BOOL m_bTransfer;
	// Generated message map functions
	//{{AFX_MSG(CDgnConMatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickDgnConcmatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedDgnConcMatList(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnSelchangeDgnConcMatCode();
	afx_msg void OnSelchangeDgnConcMatName();
	afx_msg void OnSelchangeDgnConcRebarCode();
	afx_msg void OnSelchangeDgnConcRebarMainName();
	afx_msg void OnSelchangeDgnConcRebarSubName();
	afx_msg void OnClickLambdaChk();
	afx_msg void OnDgnConcElasticity();
	afx_msg void OnClickTransferChk();

	afx_msg void OnSelchangeDgnConcMatCodeSlab();
	afx_msg void OnSelchangeDgnConcMatNameSlab();
	afx_msg void OnSelchangeDgnConcRebarCodeSlab();
	afx_msg void OnSelchangeDgnConcRebarMainNameSlab();
	afx_msg void OnSelchangeDgnConcRebarSubNameSlab();
	afx_msg void OnClickLambdaChkSlab();
	afx_msg void OnDgnConcElasticitySlab();

	afx_msg void OnDgnConcmatModify();
	afx_msg void OnDgnConcmatClose();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCOMPOSITEMATDLG_H__INCLUDED_)
