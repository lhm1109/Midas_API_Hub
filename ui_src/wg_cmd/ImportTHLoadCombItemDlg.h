#if !defined(AFX_CImportTHLoadCombItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CImportTHLoadCombItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CImportTHLoadCombItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CImportTHLoadCombItemDlg dialog

class CImportTHLoadCombItemDlg : public CDialogMove
{
	// Construction
public:
	CImportTHLoadCombItemDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CDBDoc* m_pDoc;
	BOOL	m_bModify;

protected:
	T_THJI_K m_Key;
	T_THJI_D m_Data;

	BOOL Dlg2Data();
	BOOL Data2Dlg();

public:
	void	SetData(T_THJI_K Key, T_THJI_D &Data);

	// Dialog Data
		//{{AFX_DATA(CImportTHLoadCombItemDlg)
	enum { IDD = IDD_CMD_TH_LOAD_COMB_ITEM_IMPORT_DLG };
	MEdit     m_edtName;
	MEdit     m_edtDesc;
	CListCtrl	m_lstTHLoadCase;
	//CComboBox	m_cmbSelected; // GEN-7112 에서 삭제되었는데 다시 살아날수도 있어서 주석

	int m_nType;
	//int m_nOptType;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportTHLoadCombItemDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL AddToDB();

	// Generated message map functions
	//{{AFX_MSG(CImportTHLoadCombItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnButtonImport();
	afx_msg void OnButtonModify();
	afx_msg void OnButtonDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	enum ColName
	{
		Name,
		Status,
		Location
	};

private:
	//`void OnOptRadio();
	void SetListViewCtrl();
	void SetItem(CListCtrl& lc, int nIndex, std::string& strFilePath);

	std::string TrimJsonFileName(const std::string& FilePath);
	bool GetSelectedIndex(int &nIndex);

	CString DataToStr(const int i, std::string& strFilePath);
	bool CheckSamePathName(std::string& strFilePath);
	std::string ConvertRelativePath(const std::string& FilePath);
	bool ValidateThjiFile(const T_THJI_D& rData); // Json 경로 유효성 검사
	//void ResetSelectedCombo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CImportTHLoadCombItemDlg_H__5FD4EE01_64A1_11D5_880F_00010263A1F4__INCLUDED_)
