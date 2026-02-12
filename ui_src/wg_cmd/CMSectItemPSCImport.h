#if !defined(AFX_CMSECTITEMPSCIMPORT_H__4FEEB64D_23FB_406B_AAC3_6154ED6DEB8A__INCLUDED_)
#define AFX_CMSECTITEMPSCIMPORT_H__4FEEB64D_23FB_406B_AAC3_6154ED6DEB8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCImport.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCImport dialog

class CCMSectItemPSCImport : public CDialogMove
{
// Construction
public:
	CCMSectItemPSCImport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCImport)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_IMPORT };
	CListBox	m_wndList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCImport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData(const CArray<UINT, UINT> &raID, const CArray<T_SECT_D, T_SECT_D&> &raData, int nCurID);
	BOOL GetData(UINT &rCurID, T_SECT_D& rData);
	void SetCompositeMode() {m_bCompositeMode=TRUE;}; // 20080223 mylee

protected:
	void SetList();
	BOOL SaveData();

protected:
	UINT                        m_nCurID;
	CArray<UINT, UINT>          m_aAllID;
	CArray<T_SECT_D, T_SECT_D&> m_aAllData;
	// 20080223 mylee
	int m_bCompositeMode; // FALSE: General, PSC Value / TRUE: Composite General

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCImport)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCIMPORT_H__4FEEB64D_23FB_406B_AAC3_6154ED6DEB8A__INCLUDED_)
