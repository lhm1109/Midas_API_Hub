#if !defined(AFX_INELASTICHINGEIMPORTDLG_H__7029B12F_6D15_48A4_9A91_D261E24129A0__INCLUDED_)
#define AFX_INELASTICHINGEIMPORTDLG_H__7029B12F_6D15_48A4_9A91_D261E24129A0__INCLUDED_

// Eqmf -> Inmf·Î º¯°æ 05.02.14 by jbseon

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InelasticHingeImportDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\BnFile.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
/////////////////////////////////////////////////////////////////////////////
using namespace mit::frx;
// CInelasticHingeImportDlg dialog
struct T_IEHP
{
	T_IEHP_K Key;
	T_IEHP_D Data;
};

struct T_IEHG
{
	T_IEHG_K Key;
	T_IEHG_D Data;
};

struct T_INMF
{
	T_INMF_K Key;
	T_INMF_D Data;
};

class CAttrCtrl;
class CEditData;
class CInelasticHingeImportDlg : public CDialogMove
{
// Construction
public:
	CInelasticHingeImportDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInelasticHingeImportDlg)
	enum { IDD = IDD_INELASTIC_HINGE_IMPORT };
	MButton	m_InitialForceChk;
	MButton	m_AssignIehpChk;
	MButton	m_IehpChk;
	MEdit	m_FilePathEdt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInelasticHingeImportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CAttrCtrl* m_pAttrCtrl;
	CEditData* m_pEditData;
	CFile* m_pFile;
	CBnFile m_BnFile;

	CArray<UINT,UINT> m_arRadio;

	CMap<UINT,UINT,T_INMF_K,T_INMF_K> m_mapElem;
	CMap<UINT,UINT,T_INMF_K,T_INMF_K> m_mapELink;
	CMap<UINT,UINT,T_INMF_K,T_INMF_K> m_mapGLink;

	CArray<T_IEHP, T_IEHP&> m_arIehp;
	CArray<T_IEHG, T_IEHG&> m_arIehg;
	CArray<T_INMF, T_INMF&> m_arInmf;

	BOOL Execute();
	BOOL RB_IEHP();
	BOOL RB_IEHG();
	BOOL RB_INMF();
	BOOL ImportData(int nImportType);

	void MakeInmfMap();
	CString MakeNewIehpName(CString strName);
	BOOL EndEdit(BOOL bEnd, BOOL bDirectDrawLabel=FALSE, unsigned int nCommand=0);

	// Generated message map functions
	//{{AFX_MSG(CInelasticHingeImportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBrowseBtn();
	virtual void OnOK();
	afx_msg void OnCmdIehpChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INELASTICHINGEIMPORTDLG_H__7029B12F_6D15_48A4_9A91_D261E24129A0__INCLUDED_)
