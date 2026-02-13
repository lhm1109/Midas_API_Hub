#if !defined(__DGNGENSEISFACTOR_H__)
#define __DGNGENSEISFACTOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenSeisFac.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_CobxSpfc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\MIT_frx\MEdit.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSeisFactor dialog

struct _DGN_SEIS_LCTP	
{
	int		index;
	int		iLoadCaseType;			// 0: Static,		1: Response Spectrum

	void Initialize()
	{
		iLoadCaseType = 0;
	}
};

using namespace mit::frx;
class __MY_EXT_CLASS__ CDgnGenSeisFactor : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenSeisFactor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnGenSeisFactor)
	enum { IDD = IDD_DGN_GEN_SEIS_FAC_DLG };
	
	MComboBox	m_LoadCase;
	MEdit		m_VLFactor;
	MEdit		m_SDS;
	MEdit	 	m_OSFactor;
	MEdit		m_VFFactor;
	CListCtrl	m_ListCtrl;
//	double	m_OSFac;
//	double	m_VFFac;
	//}}AFX_DATA

public:
	T_SLCF_D m_Data;

public:

	void Initial_LCaseComboBox();

	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_SOSF_BASE &data);
	void SetItem(int nIndex, T_SOSF_BASE &data);
	BOOL ValidItem(T_SOSF_BASE& data);
	BOOL Dlg2Item(T_SOSF_BASE &data);
	BOOL Item2Dlg(int nIndex, T_SOSF_BASE &data);
	BOOL CheckLoadCaseName(CString str, int& Index);
	int  GetLoadCaseType(int iIndex);
	void SetHideSpecialGroup(BOOL bSet) { m_bHideSpecialGroup=bSet; }
	void SetDgnCode(const CString& strCode) { m_strCode = strCode; }
	//CString GetLoadCaseName(int iIndex);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenSeisFactor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<_DGN_SEIS_LCTP, _DGN_SEIS_LCTP> m_arLCtype;

	// Generated message map functions
	//{{AFX_MSG(CDgnGenSeisFactor)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClickDgnGenSeisList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedDgnGenSeisList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnSelchangeDgnGenSeis();
	afx_msg void OnDgnGenSeisAdd();
	afx_msg void OnDgnGenSeisModify();
	afx_msg void OnDgnGenSeisDelete();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void ShowHideCtrl();
	void EnableCtrl();
	BOOL m_bHideSpecialGroup;
	CString m_strCode;

private:
	void SetTextCtrl();
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNGENSEISFACTOR_H__)
