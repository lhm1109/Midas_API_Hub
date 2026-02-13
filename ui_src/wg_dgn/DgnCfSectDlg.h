#if !defined(__DGNCFSECTDLG_H__)
#define __DGNCFSECTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnCfSectDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_cmd\CMSecViewWnd.h"

#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnCfSectDlg dialog

class __MY_EXT_CLASS__ CDgnCfSectDlg : public CDialogMove
{
// Construction
public:
	CDgnCfSectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnCfSectDlg)
	enum { IDD = IDD_DGN_CF_SECT_DLG };

	CListCtrl	m_List;
	CEditUnit m_edtCoatingThik;
	CTextUnit m_wndCoatingThik;
	CCMSecViewWnd m_wndSecView;
	//}}AFX_DATA
	CBCGPStatic   m_wndFirstTitle[10];
	CEditUnit	m_wndFirstSize[10];
	CTextUnit m_wndFirstUnit[10];

public:
	void EnableEditBox(BOOL bCheck);
	void Initial_Data();
	void Initial_Unit();
	void Initial_ListCtrl();
	
	CString GetDesignCodeName();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnCfSectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	T_SECT_K m_Key;
	T_SECT_D m_Data;

	CString DataToStr(int i, T_SECT_K Key, T_SECT_D &Data);
	void SetFirstSectData();
	void Data2Dlg_Size(CStringArray& aSizeTitle, double* pSize, BOOL bEnable);

	// Generated message map functions
	//{{AFX_MSG(CDgnCfSectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnModify();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif 
