#if !defined(__CMDAUTOLOADUNDERGROUND_H__)
#define			 __CMDAUTOLOADUNDERGROUND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\mit_frx\MCheckListBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadUnderGroundDlg dialog
#include "HeaderPre.h"
using namespace mit::frx;

class CLoadCaseUnderMainGrid;
class __MY_EXT_CLASS__ CCmdAutoLoadUnderGroundDlg : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CCmdAutoLoadUnderGroundDlg(CWnd* pParent = NULL);   // standard constructor
	~CCmdAutoLoadUnderGroundDlg();
	// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadUnderGroundDlg)
	enum { IDD = IDD_CMD_LOAD_UNDEGROUND_LOAD_DLG };

	MComboBox	m_ScaleFactorCmb;
	CListCtrl	m_ScaleFactorList;
	MButton		m_wndSeisFactorChk;

	mit::frx::MCheckListBox	m_SeisList;
	mit::frx::MCheckListBox	m_StaticList;
	
	//}}AFX_DATA

public:
	CString GetKey() { return m_strKey4CheckBoxList; }
	void InitCtrl();
	void Dlg2Data();
	void Data2Dlg();
	void SetUseSeisFactor(BOOL bUseSeisFactor) { m_bUseSeisFactor = bUseSeisFactor; }
	BOOL GetUseSeisFactor() { return m_bUseSeisFactor; }
	void SetAllCheck_CheckBoxList(int nCheck);
	void SetData_CheckBoxList(const CString& sKey, const T_STLD_K_LIST& aSeismicK, const T_STLD_K_LIST& aStaticK);
	void GetData_CheckBoxList(T_STLD_K_LIST& aSeismicK, T_STLD_K_LIST& aStaticK, BOOL bAll=FALSE);
	void GetData_CheckBoxList(mit::frx::MCheckListBox* CheckList, T_STLD_K_LIST& aStldK, BOOL bAll=FALSE);

protected:
	void Initial_ScaleFactorComboBox();
	void Initial_ScaleFactorListBox();
	void Initial_SeisListCtrl();
	BOOL ErrorCheck();
	BOOL CheckScaleFactorName(CString str, int& Index);
	void Write_ScaleFactorListBox(int Index, double dFactor, CString strLoadCase);
	void OverWrite_ScaleFactorListBox(int Index, double dFactor, CString strLoadCase);
	void SetListCtrl_Seis();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CCmdAutoLoadUnderGroundDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	// Implementation
protected:
	CDBDoc* m_pDoc;
	T_UGLC_D	m_Data;
	double		m_dScaleFactor;
	BOOL		m_bUseSeisFactor;
	CString		m_strKey4CheckBoxList;
	CLoadCaseUnderMainGrid* m_pLoadGroupGrid;
	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadUnderGroundDlg)

	virtual BOOL OnInitDialog();
	afx_msg void OnClickCmdScaleFactorList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCmdScaleFactorList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdScaleFactorAdd();
	afx_msg void OnCmdScaleFactorDel();
	afx_msg void OnCmdScaleFactorMod();
	afx_msg void OnCmdUnderLoadChk();
	afx_msg void OnCmdUnderLoadBtn();
	afx_msg void OnCmdCancelBtn();
	afx_msg void OnCmdOkBtn();
	afx_msg void OnClickCmdSeismicList();
	afx_msg void OnClickCmdStaticList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADUNDERGROUND_H__)
