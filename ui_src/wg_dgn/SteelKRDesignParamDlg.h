#if !defined(__STEELKRDESIGNPARAMDLG_H__)
#define __STEELKRDESIGNPARAMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSteelKRDesignParamDlg.h : header file
//

#include "DgnDlgBase.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MCheckListBox.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\DB_ST_DN.h"

/////////////////////////////////////////////////////////////////////////////
// CSteelKRDesignParamDlg dialog
using namespace mit::frx;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CSteelKRDesignParamDlg : public CDgnDlgBase, public CDBUpdateConnector //CDialogMove
{
// Construction
public:
	CSteelKRDesignParamDlg(CWnd* pParent = NULL);   // standard constructor
	~CSteelKRDesignParamDlg();
	
// interface
public:
// Dialog Data
	//{{AFX_DATA(CSteelKRDesignParamDlg)
	enum { IDD = IDD_DGN_STL_KR_DESIGN_PARAM };	 
	CBCGPStatic	m_DesignCodeStatic;
	MCheckListBox	m_ltbxLcomStre;
	CString	m_SelectElemString;
	CSelectEdit m_editSelectElem;
	MEdit m_editEtc;
	MButton	m_radioCheckMethod1;
	MButton	m_radioCheckMethod2;
	MButton	m_radioCheckMethod3;
	MButton	m_chkBiaxial;
	MButton	m_radioY;
	MButton	m_radioZ;
	MButton	m_radioYZ;
	MButton	m_chkIend;
	MButton	m_chkJend;
	CListCtrl	m_List;
	//}}AFX_DATA
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSteelKRDesignParamDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CArray<T_ELEM_K,T_ELEM_K> m_arSelElemK;
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

protected:
	CDBDoc* m_pDoc;
	
	CArray<T_LCOM_K, T_LCOM_K> m_arLcomKey;
	CMap<T_LCOM_K,T_LCOM_K,T_LCOM_K,T_LCOM_K> m_arStreLcomKey;
	void InitialLcomDataForDgn();
	void Dlg2Data_LoadCase();

	void SetSelectElemString();
	void Data2Dlg_StdpCotrol(const T_STDP_D& data);
	T_STDP_D Dlg2Data_StdpCotrol();
	BOOL DataCheck(const T_STDP_D& data);

	void Initial_ListData();
	void SetListCtrlHeader();
	BOOL InsertListItem(T_STDP_K Key);
	BOOL DeleteListItem(T_STDP_K Key);
	BOOL ModifyListItem(T_STDP_K Key);
	int  FindInsertionPos(T_STDP_K nID);
	CString  DataToStr(int i, T_STDP_K Key);
	void ChangeItem(int nIndex);
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);
	void ChangeLoadCombination();
	void EnableCalcBtn(bool Enable);

	

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSteelKRDesignParamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddBtnClick();
	afx_msg void OnModifyBtnClick();
	afx_msg void OnDeleteBtnClick();
	afx_msg void OnCheckMethod();
	afx_msg void OnUnbracedLengthClick();
	afx_msg void OnEffectLengthClick();
	afx_msg void OnEffectWidthClick();
	afx_msg void OnApplyClick();
	afx_msg void OnCloseClick();
	afx_msg void OnOnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void RunApply() { OnApplyClick(); }
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__STEELKRDESIGNPARAMDLG_H__)
