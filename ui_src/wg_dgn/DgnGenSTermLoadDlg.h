#if !defined(AFX_DGNGENSTERMLOADDLG_H__49F823C1_EA92_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENSTERMLOADDLG_H__49F823C1_EA92_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "HeaderPre.h"

const int iDgn_SLTermID = 6;
const int iDgn_ShortTermID = 8;

/////////////////////////////////////////////////////////////////////////////
// CDgnGenSTermLoadDlg dialog
struct _DGN_STLD
{
	CString strName;
	UINT key;
	int iAnalType;
	int iSeqNum;
	void Initialize()
	{
		strName = _T("");
		key = 0;
		iAnalType=0;
		iSeqNum=0;
	}
};

class __MY_EXT_CLASS__ CDgnGenSTermLoadDlg : public CDialogMove
{
// Construction
public:
	CDgnGenSTermLoadDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CDgnGenSTermLoadDlg)
	enum { IDD = IDD_DGN_GEN_LOAD_DIR_DLG };
	CListCtrl	m_aXLoadList;
	CListCtrl	m_aYLoadList;
	CListCtrl	m_aLoadList;
	CListCtrl	m_aLongLoadList;
	//}}AFX_DATA

	int m_iSLTermID[iDgn_SLTermID];
	int m_iShortTermID[iDgn_ShortTermID];

public:
	CMap<int, int, _DGN_STLD, _DGN_STLD&> m_arLoadMap;
	CMap<int, int, _DGN_STLD, _DGN_STLD&> m_arLongLoadMap;
	CMap<int, int, _DGN_STLD, _DGN_STLD&> m_arXLoadMap;
	CMap<int, int, _DGN_STLD, _DGN_STLD&> m_arYLoadMap;

	void InitLCaseData();
	void InitCtrl();
	void InitDgnCode();
	void MoveCtrl();
	void Write_ListCtrl(CListCtrl& LoadList, CMap<int, int, _DGN_STLD, _DGN_STLD&>& LoadMap);
	
	CRect MoveCtrl(int iKind, int* pID, CRect Rect0);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBarSpaceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnConBarSpaceDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnAddLongLoadBtn();
	afx_msg void OnDgnDelLongLoadBtn();
	afx_msg void OnDgnAddXLoadBtn();
	afx_msg void OnDgnDelXLoadBtn();
	afx_msg void OnDgnAddYLoadBtn();
	afx_msg void OnDgnDelYLoadBtn();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENSTERMLOADDLG_H__49F823C1_EA92_11D3_888F_0000C0F30D4D__INCLUDED_)
