#if !defined(AFX_CMSECTITMEPSCVIEW_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_)
#define AFX_CMSECTITMEPSCVIEW_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItmePSCView.h : header file
//
#include "CMDlgBase.h"
#include "CMSecViewWnd.h"
#include "..\mit_frx\MillustViewer.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItmePSCView dialog

class CCMSectItmePSCView : public CCMDlgBase
{
// Construction
public:
	CCMSectItmePSCView(CWnd* pParent = NULL);   // standard constructor
	~CCMSectItmePSCView();

public:
	BOOL ChangeBitmap(UINT nIndex);
	BOOL ChangeBitmapOld(UINT nIndex);
	void SetDataSource(T_SECT_D* pDataSrc);
	void RedrawSection(BOOL bCentroid=FALSE); // 이 함수를 호출하기 전 SetDataSource()를 호출하여 
																						// Data가 Setting되어 있어야 한다.
	void ShowSectView(BOOL bShow);

// Dialog Data
	//{{AFX_DATA(CCMSectItmePSCView)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_VIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItmePSCView)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int     m_nCurIndex;
	BOOL    m_bSetData;

	CCMSecViewWnd m_wndSecView;
	CBitmap*      m_pBitmap;
	UINT          m_nBitmapID;
	mit::frx::MillustViewer m_wndPicture;

		// Generated message map functions
	//{{AFX_MSG(CCMSectItmePSCView)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	afx_msg void OnCmdSectCentroidBtn();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITMEPSCVIEW_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_)
