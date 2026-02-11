#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_base\wg_base_ChildDialog.h"   // for CChildDialog
#include "LoadCombDefineData.h"
struct T_COMB_CASE	
{
	int		nLoadCaseType;
	CString  strName;

	void Initialize()
	{
		nLoadCaseType = 0;
		strName = _T("");
	}
};

struct T_COMB_WE
{
	UINT keyW;
	UINT keyE;
	int nLoadCaseType;
	void Initialize()
	{
		keyW = 0;
		keyE = 0;
		nLoadCaseType = 0;
	}
};

class CCMLoadCombDefDlg;

class CCMLoadCombParaPage : public CChildDialog
{
public:
	CCMLoadCombParaPage(CLoadCombDefineData *pData, CWnd* pParent = NULL);   // standard constructor

	int m_nConstLoad;
	mit::frx::MButton		m_chkOrthRlt;
	mit::frx::MComboBox		m_cmbWCase;
	mit::frx::MComboBox		m_cmbECase;
	CListCtrl	m_ListCtrl;
	double		m_dPhiL;
	double		m_dPhiT;
	double		m_dPhiW;
	double		m_dGravL;

public:
	CDBDoc* m_pDoc;
	CCMLoadCombDefDlg* m_pParent;
	CLoadCombDefineData *m_pData;
	enum { IDD = IDD_CMD_LOADCOMB_DEF_PARA_PAGE };

protected:
	CArray<T_COMB_CASE, T_COMB_CASE&> m_arLCtype;
	CArray<T_STLD_K, T_STLD_K> m_arKeyList;
	CString m_strCaseW;
	CString m_strCaseE;
	CArray<UINT, UINT> m_aCtrlConstLoad;
	BOOL    m_bExistCS;
	BOOL    m_bExistEV;
public:
	BOOL Data2Dlg();
	BOOL Dlg2Data();
	virtual BOOL OnInitDialog();
protected:
	void Initial_WCaseComboBox();
	void Initial_ECaseComboBox();
	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	int GetLoadCaseType(CString strName);
	void MakeItemEx();
	CString DataToStr(int i, T_COMB_WE &data);
	void SetItem(int nIndex, T_COMB_WE &data);
	BOOL ValidItem(T_COMB_WE &data);
	BOOL Dlg2Item(LOAD_CASE_K &keyW, LOAD_CASE_K &keyE);
	BOOL Item2Dlg(int nIndex, T_COMB_WE &data);
	BOOL CheckLoadCaseName(CString str, int& Index);
	void ChangeDataStore(LOAD_CASE_K keyW, LOAD_CASE_K keyE, T_COMB_WE &data);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	LRESULT OpenNewTab(WPARAM wParam, LPARAM lParam);
	LRESULT CloseCurTab(WPARAM wParam, LPARAM lParam);
protected:
	afx_msg void OnChkSetOtholc();
	afx_msg void OnCmdSetOtholcBtn();
	afx_msg void OnWindAndEarthCaseCombAddBtn();
	afx_msg void OnWindAndEarthCaseCombEditBtn();
	afx_msg void OnWindAndEarthCaseCombDelBtn();
	afx_msg void OnConSpecCasePartCoefBtn();
	afx_msg void OnCombCoefDetailSetBtn();
	afx_msg void OnGravCoefDetailSetBtn();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void SetSeismicEvaluation(BOOL bSet) { m_bSeismicEvaluation=bSet; }
protected:
	BOOL m_bSeismicEvaluation;
};

