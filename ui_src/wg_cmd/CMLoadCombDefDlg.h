#if !defined(__CMLOADCOMBDEFDLG_H__)
#define __CMLOADCOMBDEFDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadCombDlg.h : header file
//

#include "CMDlgBase.h"
#include "..\wg_db\TextFile.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\MyCheckComboBox.h"
#include "LoadCombDefineData.h"
/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombDefDlg dialog

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////
class CDBDoc;
class CCMLoadCombDefPage;
class CCMLoadCombParaPage;
class CCMLoadCombSpecPage;

class __MY_EXT_CLASS__ CCMLoadCombDefDlg : public CCMDlgBase
{
public:
	CCMLoadCombDefDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMLoadCombDefDlg();

public:
	CLoadCombDefineData *m_pData;
	CDBDoc* m_pDoc;
	CCMLoadCombDefPage*  m_pDefine;
	CCMLoadCombParaPage* m_pPara;
	CCMLoadCombSpecPage* m_pSpec;

	int m_nActivePage;

	enum { IDD = IDD_CMD_LOADCOMB_DEF_DLG };
	CDlgTabCtrl m_wndTab;
	CMyCheckComboBox	m_cmbLcomType;
	int m_nOption;	//0:add 1:replace
public:
	void SetActivePage(int SelectPage);
	BOOL Data2Dlg();
	BOOL Dlg2Data();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnReplace();	
	afx_msg void OnCmdBtnImport();
	afx_msg void OnCmdBtnExport();
	afx_msg void OnDlgClose();
	DECLARE_MESSAGE_MAP()

protected:
	CTextFile m_TextFile;
	CString m_strLine;
	BOOL m_bEndData;

protected:
	void InitComboType();
	void OnCmdCombDlg();
	BOOL WriteData(CString strFileName);
	BOOL ReadData(FILE *fp);
	int lex(FILE *fp, char *yytext);
};
/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////


#endif 
