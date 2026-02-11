#if !defined(__CMDAUTOLOADSEISFACTOR_H__)
#define __CMDAUTOLOADSEISFACTOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
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

struct _CMD_SEIS_LCTP
{
	int		index;
	int		iLoadCaseType;			// 0: Static,		1: Response Spectrum

	void Initialize()
	{
		iLoadCaseType = 0;
	}
};


class __MY_EXT_CLASS__ CCmdAutoLoadSeisFactor : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CCmdAutoLoadSeisFactor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadSeisFactor)
	enum { IDD = IDD_CMD_LOAD_SEIS_FAC_DLG };

	MComboBox	m_LoadCase;
	CEdit			m_VLFactor;
	CEdit			m_SDS;
	CEdit	 	  m_OSFactor;
	CListCtrl	m_ListCtrl;
		//}}AFX_DATA

public:
	T_SIFT_D m_Data;

public:

	void Initial_LCaseComboBox();

	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_SIFT_BASE &data);
	void SetItem(int nIndex, T_SIFT_BASE &data);
	BOOL ValidItem(T_SIFT_BASE& data);
	BOOL Dlg2Item(T_SIFT_BASE &data);
	BOOL Item2Dlg(int nIndex, T_SIFT_BASE &data);
	BOOL CheckLoadCaseName(CString str, int& Index);
	int  GetLoadCaseType(int iIndex);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadSeisFactor)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<_CMD_SEIS_LCTP, _CMD_SEIS_LCTP> m_arLCtype;

	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadSeisFactor)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClickCmdGenSeisList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCmdGenSeisList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnSelchangeCmdGenSeis();
	afx_msg void OnCmdGenSeisAdd();
	afx_msg void OnCmdGenSeisModify();
	afx_msg void OnCmdGenSeisDelete();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADSEISFACTOR_H__)
