#if !defined(AFX_IEHPLISTDLG_H__D36FE2A7_A431_4378_8610_606D7666FE13__INCLUDED_)
#define AFX_IEHPLISTDLG_H__D36FE2A7_A431_4378_8610_606D7666FE13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IehpListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CIehpListDlg dialog
/*
struct T_IEHP_EXPORT
{
	CString strName;
	int    nHingeType;
	int    nComponent;
	int    nHingeLoc;
	int    nHysModel;
	int    nSymmetryType;
	int    nInitStiffType;
	double dInitStiffness;
	int    nPAlphaDelta;
	int    nForceUnit;
	int    nLengthUnit;
	double P1[2];
	double P2[2];
	double P3[2];
	double P4[2];
	double D1[2];
	double D2[2];
	double D3[2];
	double D4[2];
	double dUnloadStiffCalcExpo;
	double dUnloadStiffReduFac;
	void Initialize()
	{
		strName = "";
		nHingeType = 0;
		nComponent = 1;
		nHingeLoc = 0;
		nHysModel = 0;
		nSymmetryType = 0;
		nInitStiffType = 0;
		dInitStiffness = 0.;
		nPAlphaDelta = 0;
		nForceUnit = 0;
		nLengthUnit = 0;
		for(int i = 0; i < 2; i++)
		{
			P1[i] = 0.;
			P2[i] = 0.;
			P3[i] = 0.;
			P4[i] = 0.;
			D1[i] = 0.;
			D2[i] = 0.;
			D3[i] = 0.;
			D4[i] = 0.;    
		}
		dUnloadStiffCalcExpo = 0.;
		dUnloadStiffReduFac = 0.;
	}
};
*/

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CIehpListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CIehpListDlg(CWnd* pParent = NULL);   // standard constructor
	void SetInitSelect(CString strInit);

//----------------------------------------------------------------------
// Overrides
//----------------------------------------------------------------------
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer(); 

//----------------------------------------------------------------------
// Implementation
protected:
	void SetHeaderTitle();
	virtual void MakeItemEx();

	BOOL InsertItem(T_IEHP_K Key, T_IEHP_D &Data);
	BOOL DeleteItem(T_IEHP_K Key, T_IEHP_D &Data);
	BOOL ModifyItem(T_IEHP_K KeyOld, T_IEHP_K Key, T_IEHP_D &Data);
	CString DataToStr(int i, T_IEHP_K Key, T_IEHP_D &Data);
	void SelectItem(CString str);
	
	static void GetCSVDatabyLine(CString strLine, CArray<CString, CString&>& aCSVLineData);

	void OnIehpImportFile(); 
	void OnIehpExportFile();
	BOOL MakeIehpCSVFile(LPCTSTR lpFileName);
	int GetHysModel(CString& strHysModel);
	CString GetHysModel(int nHysModel);
	BOOL WriteIehpCSVData();
	BOOL ReadIehpCSVData(FILE *file);  
	BOOL CheckDataValidityforExport(T_IEHP_D& IehpData);
	void GetHysModelData4Export(T_IEHP_D& IehpData, int nDof, CString* strHysModelData);

	BOOL CheckAndGetIehpData(CArray<CString, CString&>& apLineData, int nLineCount);
	BOOL SaveIehpCSVData();    
	BOOL ErrorMessage(LPCTSTR strName, LPCTSTR strDir, LPCTSTR strErrorItem);
	BOOL CheckErrorNameDofVar(LPCTSTR lpszName, LPCTSTR lpszDof, LPCTSTR lpszVar);
	BOOL CheckErrorVar(int nLineCount, LPCTSTR lpszVar);
	BOOL SetIehpHysModelData(T_IEHP_D& rData, int nDof, CArray<CString, CString&>& aLineData);
	BOOL CompareOldandNewData(T_IEHP_D& rPrevData, T_IEHP_D& rNewData, int nDof);
	void ConvertDeformIndex(T_IEHP_D& CSVData, T_IEHP_D& OldData);
protected:  
	CDBDoc* m_pDoc;
	CStdioFile m_cfile;
// Dialog Data
	//{{AFX_DATA(CIehpListDlg)
	enum { IDD = IDD_ETC_IEHP_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strInitSelect;
	BOOL m_bInitSelect;  
	CArray<T_IEHP_D, T_IEHP_D&> m_aIehpData;  
	// Generated message map functions
	//{{AFX_MSG(CIehpListDlg)
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnImport();
	afx_msg void OnCmdBtnExport();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnCopy();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEHPLISTDLG_H__D36FE2A7_A431_4378_8610_606D7666FE13__INCLUDED_)
