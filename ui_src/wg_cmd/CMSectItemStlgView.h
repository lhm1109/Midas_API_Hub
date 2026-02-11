#if !defined(AFX_CMSectItemStlgView_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_)
#define AFX_CMSectItemStlgView_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemStlgView.h : header file
//
#include "CMDlgBase.h"
#include "CMSecViewWnd.h"

#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemStlgView dialog

class CCMSectItemStlgView : public CCMDlgBase
{
// Construction
public:
	CCMSectItemStlgView(CWnd* pParent = NULL);   // standard constructor
	~CCMSectItemStlgView();

public:
	BOOL ChangeBitmap();  
	void SetDataSource(T_SECT_D* pDataSrc);
	void RedrawSection(BOOL bCentroid=FALSE);
																					 
	void ShowSectView(BOOL bShow);

// Dialog Data
	//{{AFX_DATA(CCMSectItemStlgView)
	enum { IDD = IDD_CMD_SECT_ITEM_STLG_VIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemStlgView)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int GetResIDBySect(CString& strImgPath_Sect);
	int GetResIDBMCell(CString& strImgPath_Sect);
	CCMSecViewWnd m_wndSecView;
	MillustViewer m_ImgSectItem;

	CBitmap*      m_pBitmap;
	UINT          m_nBitmapID;

	T_SECT_D* m_pSect;

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemStlgView)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//	afx_msg void OnCmdSectCentroidBtn();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSectItemStlgView_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_)
