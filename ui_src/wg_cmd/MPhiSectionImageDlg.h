#if !defined(__MPhiSectionImageDlg_H__)
#define __MPhiSectionImageDlg_H__

#include "..\MIT_frx\MillustViewer.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SWGRILLayoutElaDlg.h : header file
//

class CMPhiSectionImageDlg : public CChildDialog
{
public:
	CMPhiSectionImageDlg(CWnd* pParent = NULL);   // standard constructor	
	enum { IDD = IDD_CMD_MPHI_SECTION_IMAGE_DLG };
	mit::frx::MillustViewer m_KeepStrDown;
private:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

#endif