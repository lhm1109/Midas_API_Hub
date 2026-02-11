#if !defined(AFX_CMSECTITEMBASE_H__673754EA_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECTITEMBASE_H__673754EA_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemBase.h : header file
//

#include "..\wg_base\wg_base_DlgChild.h"

#include "CMSecViewWnd.h"
#include "..\MIT_frx\MillustViewer.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemBase dialog
using namespace mit::frx;
class CCMSectItemBase : public CDlgChild
{
// Construction
public:
	CCMSectItemBase(CWnd* pParent = NULL);   // standard constructor
	CCMSectItemBase(UINT nResID, CWnd* pParent);
	~CCMSectItemBase();

//---------------------------------------------------
// Interface
//---------------------------------------------------
public:
	void      SetMode(BOOL bDesign) { m_bDesign = bDesign; }
	void      SetVirual(BOOL bVirtual) { m_bVirtual = bVirtual; }
	void      SetModifyData(T_SECT_K Key, T_SECT_D &Data);
	T_SECT_K  GetNewID() { return m_Key; }
	void      SaveDlgData() { Dlg2Data(); }
	void			SetSectionID(UINT key);

	virtual void SetNewID(T_SECT_K nNewID) = 0;
	virtual BOOL OnOKPublic()=0;
	virtual BOOL OnApplyPublic()=0;
	virtual void ShowCalcResult()=0;
	virtual BOOL Dlg2Data()=0;
	virtual BOOL GetSectData(T_SECT_D& data){return FALSE;}
	virtual void DisplayOffsetPoint()=0;
	virtual BOOL DestroyWindow();

	virtual bool IsEnableSectShape7thDOF_Temp(const CString& strShape, const bool& bHSQType, const bool& bRoundType);

//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:
	void InitMember();

protected:
	CDBDoc*   m_pDoc;

	CImageList* m_pImageList;   // Image for ComboBox
	CBitmap* m_pBitmap;         // Dimension Guide Bitmap
	CCMSecViewWnd m_wndSecView; // Section Preview
	MillustViewer m_ImgSectItem;

	BOOL      m_bInitDone;
	BOOL      m_bModify;
	BOOL      m_bModifyGrpd;
	BOOL      m_bModifyStfn;
	T_SECT_K  m_OrgKey;
	T_SECT_K  m_Key;
	T_SECT_D  m_Data;
	T_GRPD_SSM_D m_GrpdSSMD;
	T_STFN_SSM_D m_StfnSSMD;

	BOOL      m_bDesign;
	BOOL      m_bVirtual;
	BOOL			m_bWorkTab;
	int				m_FixID;


protected:
// Dialog Data
	//{{AFX_DATA(CCMSectItemBase)
	enum { IDD = 0 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemBase)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemBase)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMBASE_H__673754EA_FBDD_11D3_92DE_0000C0B0E6B3__INCLUDED_)
