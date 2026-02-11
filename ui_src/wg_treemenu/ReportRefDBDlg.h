#if !defined(__REPORTREFDBDLG_H__)
#define __REPORTREFDBDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportRefDBDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\BnFile.h"  // BnFile 

/////////////////////////////////////////////////////////////////////////////
// CReportRefDBDlg dialog

class CReportRefDBDlg : public CDialogMove
{
// Construction
public:
	CReportRefDBDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReportRefDBDlg)
	enum { IDD = IDD_TM_REPORT_REFDB_DLG };
	BOOL  m_bUfig;
	BOOL  m_bUtbl;
	BOOL  m_bUcht;
	BOOL  m_bUtpl;
	BOOL  m_bUimg;
	int   m_nIDType;   // 0: Keep ID, 1: New ID
	int   m_nKeepDefaultUnit; // 0: Keep the Unit, 1: Default Unit
	//}}AFX_DATA
	
public:
	CString m_strAbsFileName; // File Name (절대 경로)

protected:
	CDBDoc* m_pDoc;
	CFile* m_pFile;
	CBnFile m_BnFile;
	CArray<T_UFIG_UDRD_D,T_UFIG_UDRD_D> m_arUfig;
	CArray<T_UTBL_UDRD_D,T_UTBL_UDRD_D> m_arUtbl;
	CArray<T_UCHT_UDRD_D,T_UCHT_UDRD_D> m_arUcht;
	CArray<T_UTPL_UDRD_D,T_UTPL_UDRD_D> m_arUtpl;
	CArray<T_UIMG_UDRD_D,T_UIMG_UDRD_D> m_arUimg;

public:
	void Data2Dlg();
	void Dlg2Data();
	void ReadModelFile();
	void SetModelPathName(CString strFileName);

protected:
	BOOL ReadData();
	BOOL RB_UFIG();
	BOOL RB_UTBL();
	BOOL RB_UCHT();
	BOOL RB_UTPL();
	BOOL RB_UIMG();
	BOOL ImportUfig(int nNumType=0);
	BOOL ImportUtbl(int nNumType=0);
	BOOL ImportUcht(int nNumType=0);
	BOOL ImportUtpl(int nNumType=0);
	BOOL ImportUimg(int nNumType=0);
	void SortData();
	BOOL InsertModelToDB();
	BOOL EndEdit(BOOL bEnd);
	void InitUfig(T_UFIG_D& Data);
	void InitUtbl(T_UTBL_D& Data);
	//void InitUcht(T_UCHT_D& Data);
	//void InitUtpl(T_UTPL_D& Data);
	//void InitUimg(T_UIMG_D& Data);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportRefDBDlg)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CReportRefDBDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRefDBSelectAllBtn();
	afx_msg void OnRefDBUnselectAllBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__REPORTREFDBDLG_H__)
