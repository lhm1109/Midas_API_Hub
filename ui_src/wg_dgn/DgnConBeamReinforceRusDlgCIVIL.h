#if !defined(__DgnConBeamReinforceRusDlgCIVIL_H__)
#define __DgnConBeamReinforceRusDlgCIVIL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBeamReinforceRusDlgCIVIL.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_cmd\wg_cmd_CMSectViewWnd.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnConBeamReinforceCommonDlgCIVIL.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamReinforceRusDlgCIVIL dialog

#include "DgnBeamGridWnd.h"
#include "DgnSectionDrawWnd.h"

#include "HeaderPre.h"
class CDgnBeamGridWndRusCIVIL;
class __MY_EXT_CLASS__ CDgnConBeamReinforceRusDlgCIVIL : public CDgnConBeamReinforceCommonDlgCIVIL
{
// Construction
public:
	CDgnConBeamReinforceRusDlgCIVIL(CWnd* pParent = NULL);   // standard constructor
		
// Dialog Data
	//{{AFX_DATA(CDgnConBeamReinforceRusDlgCIVIL)
	enum { IDD = IDD_DGN_CON_REIN_BEAM_RUS_DLG };
	
	//}}AFX_DATA
	virtual void Execute() {} 

protected:  
	CDgnBeamGridWndRusCIVIL*  m_TopGrid;  // Top   Grid
	CDgnBeamGridWndRusCIVIL*  m_BotGrid;  // Botom Grid

	//{{AFX_VIRTUAL(CDgnConBeamReinforceRusDlgCIVIL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	
	// 여기에 추가로 구연을 하려면 CDgnConBeamReinforceCommonDlgCIVIL에 있던 내용은 가상함수로 만들고, CDgnConBeamReinforceCommonDlgCIVIL을 상속받은 모든 Class에서 실제 내용을 추가하시오..
	virtual void SetLayerNumToGrid(int nTopBot, int nLayerNum);
	virtual BOOL CheckReDraw_RebarGrid(_DGNC_RBAR& RebarData, int& iChkResult);
	virtual void SetDataToGrid();
	virtual void InitGridData();	
	virtual void SetDataToGrid_TabCtrl();
	virtual void GetDataFromGrid_TabCtrl(int iIMJ);
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnConBeamReinforceRusDlgCIVIL_H__)
