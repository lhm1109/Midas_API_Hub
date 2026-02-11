#if _MSC_VER > 1000
#pragma once
#endif

#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

#include "CMDlgBase.h"

/////////////////////////////////////////////////////////////////////////////
// CCMDTendonTemplateViewDlg dialog

class CCMDTendonTemplateViewDlg : public CCMDlgBase
{
// Construction
public:
	CCMDTendonTemplateViewDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMDTendonTemplateViewDlg();

public:
	BOOL ChangeBitmap(UINT nType); //sub Type....

	enum { IDD = IDD_CMD_TENDON_TEMPLATE_VIEW };

public:
	virtual BOOL DestroyWindow();

protected:

// Implementation
protected:
	UINT m_nCurType;
	CString m_arResource[8];

	MillustViewer m_wndPicture1;
	MillustViewer m_wndPicture2;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};