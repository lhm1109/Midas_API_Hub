#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LoadCombDefineData.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;
#include "HeaderPre.h"      /////////
struct T_GRAV_COEF	
{
	int		index;
	CString strLoadCase;
	double	dCoef;

	void Initialize()
	{
		index = -1;
		strLoadCase = _T("");
		dCoef = 1.0;
	}
};

class CDBDoc;

class __MY_EXT_CLASS__ CCMLoadCombDefGCoefDlg : public CDialogMove, public CDBUpdateConnector
{
public:
	CCMLoadCombDefGCoefDlg(CWnd* pParent = NULL);   // standard constructor

public:	
	enum { IDD = IDD_CMD_LOADCOMB_DEF_G_COEF_DLG };
	MComboBox	m_LoadCase;
	CListCtrl	m_ListCtrl;
	double		m_dCoef;

public:
	std::map<LOAD_CASE_K, double>  m_mapGravityFactor;
public:
	void SetDataList(const std::map<LOAD_CASE_K, double>  &mapGravityFactor);
	void GetDataList(std::map<LOAD_CASE_K, double>  &mapGravityFactor);
	void Initial_LCaseComboBox();
	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	void SetItem(int nIndex, T_GRAV_COEF &data);
	BOOL Dlg2Item(T_GRAV_COEF &data);
	BOOL Item2Dlg(int nIndex, T_GRAV_COEF &data);
	BOOL CheckLoadCaseName(CString str, int& Index);
	void SetLoadCaseCombo(T_GRAV_COEF &data);

protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenSeisFactor)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

protected:
	CDBDoc* m_pDoc;
	CArray<T_GRAV_COEF, T_GRAV_COEF&> m_arLCtype;
	CArray<T_STLD_K, T_STLD_K> m_arKeyList;
	CString m_strLCName;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();	
	afx_msg void OnCmdBtnDel();
	afx_msg void OnDlgOK();
	afx_msg void OnDlgCancel();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

public:
	void SetSeismicEvaluation(BOOL bSet) { m_bSeismicEvaluation=bSet; }
protected:
	BOOL m_bSeismicEvaluation;
};
/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

