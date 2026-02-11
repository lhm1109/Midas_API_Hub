#if !defined(AFX_STLDPTMPDLG_H__INCLUDED_)
#define AFX_STLDPTMPDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STLDPTMPDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStldPtmpDlg dialog
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditTemp.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

struct T_PTMP_D;  
struct T_PTMP_STMP;
class CStldPtmpDlg : public CMenuBarChildDlg , public CDBUpdateConnector
{
// Construction
	T_PTMP_D * m_pData;
public:
	CStldPtmpDlg(CWnd* pParent = NULL);   // standard constructor
	~CStldPtmpDlg();
	virtual void Execute() { OnTmExecute(); }
	void MakeListHeader();
	void MakeListData();
	
	int  FindEndSectTemp();
	BOOL GetParameter(T_PTMP_D * pPtmpD);
	BOOL GetSectTemp(T_PTMP_STMP * pSectTempD);
	BOOL SetSectTemp(T_PTMP_STMP * pSectTempD);
	BOOL ModifySecTemp();
	BOOL DeleteSecTemp();
	BOOL AddSecTemp();
	void DoUnitChange();

// Dialog Data
	//{{AFX_DATA(CStldPtmpDlg)
	enum { IDD = IDD_TM_STLD_PTMP };
	CSelectLC	m_SelectLC;
	CCobxLdgr	m_CobxLdgr;
	CTextUnit 	m_wndT0Unit;
	CEditTemp	m_wndT0Edit;
	CTextUnit	m_wndH1Unit;
	CEditUnit	m_wndH1Edit;
	CTextUnit	m_wndT1Unit;
	CEditUnit	m_wndT1Edit;
	CListCtrl	m_wndSTempList;
	//CListCtrl	m_wndSTempBlankList;  

	int  m_nOption;      // 0:Add, Replace, Delete
	int  m_nRefPosition; // 0:Top, Bottom
	int  m_nDirection;  //  1: local-z
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldPtmpDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Implementation
protected:
	CDBDoc* m_pDoc;
	MillustViewer  m_wndPicture;

	void ChangeBitmap(int nPos);

	// Generated message map functions
	//{{AFX_MSG(CStldPtmpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnTmBtnLc();
	afx_msg void OnTmOptChange();
	afx_msg void OnTmAddButton();
	afx_msg void OnTmDeleteButton();
	afx_msg void OnTmModifyButton();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnItemchangedTmStempList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmT0TempBtn();
	afx_msg void OnTmRefPostRadio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
 




};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDPTMPDLG_H__INCLUDED_)
