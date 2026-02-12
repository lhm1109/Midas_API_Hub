#if !defined(__CMTENDONEXPORTDXFDLG_H__)
#define __CMTENDONEXPORTDXFDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTendonExportDXFDlg.h : header file
//

#include "..\wg_db\wg_db_TendonInfo.h"
#include "..\wg_db\DBProgressDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CCMTendonExportDXFDlg dialog
class CDBDoc;
class CCMTendonExportDXFDlg : public CDialogMove
{
// Construction
public:
	CCMTendonExportDXFDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMTendonExportDXFDlg();

// interface
	void SetSelectedProfiles(CArray<UINT, UINT> &aSelectedK);

// Dialog Data
	//{{AFX_DATA(CCMTendonExportDXFDlg)
	enum { IDD = IDD_CMD_DEF_TENDON_EXPORT_DXF_DLG };
	CListBox	m_listSelProfiles;
	CListBox	m_listAllProfiles;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTendonExportDXFDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitAllList();
	BOOL IsSelectedProfile(UINT TdnaK);
	void InitSelList();
	void DoSelectItem();
	void DoUnselectItem();
	void DoAllSelectItem();
	void DoNoneSelectItem();
	
	BOOL ExportToDXFFile(CString& strSuccessMsg);
	BOOL Write(LPCTSTR pFormat, ...);
	BOOL WriteTendonToDXF(CString strFileName);
	void WriteLayerInfo();
	void WriteEntities();
	void WriteTendonProfile();
	BOOL ProgressReturn(CDBProgressDlg *pPrgDlg, BOOL bReturn);

	void dxfSection(CString csSectionName);
	void dxfEndSection();
	void dxfVersion();
	void dxfTable();
	void dxfEndTable();
	void dxfTableName(CString csTableName);
	void dxfTableEntry(int nEntry);
	void dxfLayer(CString csLayerName, int nLayerFlag, int nLayerColor, CString csLineType);
	void dxfEntities(CString csEntityName);
	void dxfLayerName(CString csLayerName);
	void dxfCoord(int np, int nXYZ, double dCoord);
	void dxfText(CString csText);
	void dxfEOF();
	void ReplaceSpecialString(CString& str);

protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aSelectedProfiles;
	CStdioFile m_cfile;
	CTendonInfo* m_pTendonInfo;

// Implementation
protected:
	virtual void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(CCMTendonExportDXFDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdSelButton();
	afx_msg void OnCmdUnselButton();
	afx_msg void OnCmdAllSelButton();
	afx_msg void OnCmdNoneSelButton();
	afx_msg void OnDblclkCmdAllList();
	afx_msg void OnDblclkCmdSelectedList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMTENDONEXPORTDXFDLG_H__)
