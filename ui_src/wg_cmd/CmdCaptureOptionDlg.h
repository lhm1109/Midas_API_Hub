#if !defined(__CmdCaptureOptionDlg_H__)
#define __CmdCaptureOptionDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdCaptureOptionDlg.h : header file
//
using namespace mit::frx;
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

#include "HeaderPre.h"

struct __MY_EXT_CLASS__ T_CAPTURE_OPT
{
	int nSizeType; // 0 : 1920x1080 (FHD), 1 : 1366x768(HD), 2 : 1600x900(HD + ), 3 : 1280x800(WXGA), 4 : 1024x768(XGA), 5 : 800x600(SVGA), 6 : 640x480(VGA), 7 : Custom
	int nWidth;
	int nHeight;
	int nExtension; // 0:*.emf, 1:*.bmp, 2:*.jpg, 3:*.dxf, 4:dwg
	BOOL bSaveModelFolder;
	CString strDirPath; // 폴더 경로
	
	void Init()
	{
		nSizeType = 4;
		nWidth = 1024;
		nHeight = 768;
		nExtension = 2;
		bSaveModelFolder = TRUE;
		strDirPath = _T("");
	}

	void ReadFromReg();
	void GetCaptureOpt(int& nWidthParm, int& nHeightParm, CString& strPathParm);
	CString GetExtension(int nExtensionParam);
};


/////////////////////////////////////////////////////////////////////////////
// CCmdCaptureOptionDlg dialog
class __MY_EXT_CLASS__ CCmdCaptureOptionDlg : public CDialogMove
{
// Construction
public:
	CCmdCaptureOptionDlg(CWnd* pParent = NULL);   // standard constructor
	~CCmdCaptureOptionDlg();

// Dialog Data
	//{{AFX_DATA(CCmdCaptureOptionDlg)
	enum { IDD = IDD_CMD_CAPTURE_OPT_DLG };
	//}}AFX_DATA

	// Attributes
public:
	void SetData();		
	CDBDoc* m_pDoc;

protected:
	MComboBox	m_cbxSize;
	MEdit	m_edtWidth;
	MEdit	m_edtHeight;
	MComboBox	m_cbxExtension;
	MButton m_chkModelFolder;
	MEdit	m_edtFolder;
	

	T_CAPTURE_OPT m_Data;

protected:
// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombDlg)
	afx_msg void OnSaveFolderBtn();
	afx_msg void OnSizeCbxChange();
	afx_msg void OnSaveModelFolder();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void InitCtrl();
	
	void Data2Dlg();
	BOOL Dlg2Data();
	void EnableCtrl();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CmdCaptureOptionDlg_H__9E765C21_0CC3_11D4_888F_0000C0F30D4D__INCLUDED_)