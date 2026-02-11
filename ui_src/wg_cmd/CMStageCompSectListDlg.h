#if !defined(__CMSTAGECOMPSECTLISTDLG_H__)
#define __CMSTAGECOMPSECTLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageCompSectListDlg.h : header file
//
#include "CMSecViewWnd.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectListDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMStageCompSectListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMStageCompSectListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMStageCompSectListDlg)
	enum { IDD = IDD_CMD_SECT_FOR_CS_LIST };
	CListCtrl	m_ctrlList;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageCompSectListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

protected:
	CDBDoc* m_pDoc;
	T_SECT_D m_SectData;
	CCMSecViewWnd	m_ctrlViewFrm;
	int  m_nSectType;  // 0: section 1: Virtual Section
		
	void InitItemList();
	CString ConvStypeToString(int nStype);
	CString DataToStr(int nIndex, T_CSCS_K key, T_CSCS_D& data);
	void MakeEachItem(int nIndex, T_CSCS_K key, T_CSCS_D& data);
	void UpdateItemList();
	void SortCscsKey(CArray<T_CSCS_K, T_CSCS_K>& raCscsK);
	void InitSection();


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMStageCompSectListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCsBtnAdd();
	afx_msg void OnCmdCsBtnDel();
	afx_msg void OnCmdCsBtnMod();
	afx_msg void OnCmdCsBtnUpdate();
	afx_msg void OnCmdCsUpdateLongTermBtn();
	afx_msg void OnItemchangedCmdCsSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdCsBtnClose();
	afx_msg void OnDblclkCmdCsSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSectType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTAGECOMPSECTLISTDLG_H__A746CFA3_0E43_460C_BE93_0BAA63072093__INCLUDED_)
