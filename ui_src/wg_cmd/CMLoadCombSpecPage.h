#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "LoadCombDefineData.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_ChildDialog.h"   // for CChildDialog
#include "..\wg_base\MyCheckComboBox.h"

struct T_ADVE_CASE	//Adverse live load case
{
	int nIndex;
	CString strLiveLoad;
	std::vector<T_STLD_K> vAdverKey;

	void Initialize()
	{	
		strLiveLoad = _T("");
		vAdverKey.clear();
	}
};

struct T_ADVE_COMB	//Adverse live load comb
{
	CArrayEx<UINT, UINT> aStldComb;

	void Initialize()
	{
		aStldComb.RemoveAll();
	}
};

struct T_MUTX_CASE	
{
	int nIndex;
	CString strLiveCase;
	CString strMutexLoad;
	CArrayEx<UINT, UINT> aStldKey;

	void Initialize()
	{
		strLiveCase = _T("");
		strMutexLoad = _T("");
		aStldKey.RemoveAll();
	}
};

class CCMLoadCombDefDlg;

class CCMLoadCombSpecPage : public CChildDialog
{
public:
	CCMLoadCombSpecPage(CLoadCombDefineData *pData, CWnd* pParent = NULL);   // standard constructor

public:
	BOOL m_bAdverse;
	BOOL m_bMutex;
	CMyCheckComboBox m_cmbLCase;
	mit::frx::MComboBox m_cmbLoadCase;
	CMyCheckComboBox m_cmbMutexCase;
	CListCtrl	m_ListLCase;
	CListCtrl	m_ListMutexCase;

public:
	CDBDoc* m_pDoc;
	CCMLoadCombDefDlg* m_pParent;
	CLoadCombDefineData *m_pData;
	enum { IDD = IDD_CMD_LOADCOMB_DEF_SPEC_PAGE };

protected:
	int m_nSpecitlArrange;//0 neither, 1 Adverse,  2 Mutex
	CArray<UINT, UINT> m_aCtrlAdverse;
	CArray<UINT, UINT> m_aCtrlMutex;
	CArray<T_STLD_K, T_STLD_K> m_arKeyList;
	BOOL m_bSeismicEvaluation;
public:
	BOOL Data2Dlg();
	BOOL Dlg2Data();
	virtual BOOL OnInitDialog();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void Initial_LCaseComboBox();
	void Initial_CaseComboBox();
	void Initial_MutexCaseComboBox();
	void SetListCtrlHeader();
	BOOL GetSelectedItem1(int &nIndex);
	BOOL GetSelectedItem2(int &nIndex);
	void MakeItemEx();
	void SetItem(int nIndex, T_ADVE_CASE &data);
	BOOL Dlg2Item(CArrayEx<T_ADVE_CASE, T_ADVE_CASE&> &arAdveCase);
	BOOL IsCombinationEqual(const LOAD_CASE_K_ARR &arKey1, const LOAD_CASE_K_ARR &raKey2);
	int combination(const T_KEY_LIST &lstKey, CArrayEx<T_ADVE_COMB, T_ADVE_COMB&> &arLstComb);
	void combination_op(const T_KEY_LIST &lstKey, const T_ADVE_COMB &combOut, CArrayEx<T_ADVE_COMB, T_ADVE_COMB&> &arLstComb);
	BOOL Item2Dlg(int nIndex, T_ADVE_CASE &data);
	void SetItem(int nIndex, T_MUTX_CASE &data);
	BOOL Dlg2Item(T_MUTX_CASE &data);
	BOOL Item2Dlg(int nIndex, T_MUTX_CASE &data);
	LRESULT OpenNewTab(WPARAM wParam, LPARAM lParam);
	LRESULT CloseCurTab(WPARAM wParam, LPARAM lParam);
	
protected:	
	afx_msg void OnChkAdverseCase();
	afx_msg void OnAdverseAddBtn();
	afx_msg void OnAdverseDelBtn();
	afx_msg void OnMutexAddBtn();
	afx_msg void OnMutexEditBtn();
	afx_msg void OnMutexDelBtn();
	afx_msg void OnChkMutexCase();
	afx_msg void OnChangeSelMutexCase();
	afx_msg void OnChangedCurrentItem1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangedCurrentItem2(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

