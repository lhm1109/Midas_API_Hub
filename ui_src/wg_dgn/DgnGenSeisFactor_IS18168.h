#if !defined(__DGNGENSEISFACTOR_IS18168_H__)
#define __DGNGENSEISFACTOR_IS18168_H__

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

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSeisFactor_IS18168 dialog

struct _DGN_SEIS_LCTP_IS18168	
{
	int		index;
	int		iLoadCaseType;			// 0: Static,		1: Response Spectrum  2:LL

	void Initialize()
	{
		iLoadCaseType = 0;
	}
};


class __MY_EXT_CLASS__ CDgnGenSeisFactor_IS18168 : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenSeisFactor_IS18168(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnGenSeisFactor_IS18168)
	enum { IDD = IDD_DGN_GEN_SEIS_FAC_IS18168_DLG };
	
	MComboBox	m_LoadCase;
	MComboBox	m_LoadCase2;
    MComboBox	m_VLFactor;
	CEdit			m_SDS;
	CEdit	 	  m_OSFactor;
	CEdit			m_VFFactor;
	CListCtrl	m_ListCtrl;
	CListCtrl	m_ListCtrl2;
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
	void SetConcCode(CString& strCode) { m_strConCode = strCode; }
	//CString GetLoadCaseName(int iIndex);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenSeisFactor_IS18168)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<_DGN_SEIS_LCTP_IS18168, _DGN_SEIS_LCTP_IS18168> m_arLCtype;

	// Generated message map functions
	//{{AFX_MSG(CDgnGenSeisFactor_IS18168)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//afx_msg void OnClickDgnGenSeisList(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnItemchangedDgnGenSeisList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickLoadCaseBtn();
	//afx_msg void OnSelchangeDgnGenSeis();
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
	CString m_strConCode;
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNGENSEISFACTOR_H__)
