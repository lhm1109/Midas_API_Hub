#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCRCDgnParameter_AASHTO12.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDgnCRCDgnParameter_AASHTO12::CDgnCRCDgnParameter_AASHTO12(T_PSCD_D* pParamData, CWnd* pParent /*=NULL*/)
: CMyChildDialog(CDgnCRCDgnParameter_AASHTO12::IDD, pParent)
{
	m_pParamData = pParamData;

	m_aTndType.Add(IDC_AASHTO_DESIGN_PARAMETER2);
	m_aTndType.Add(IDC_AASHTO_DESIGN_PARAMETER3);
	m_aTndType.Add(IDC_AASHTO_DESIGN_PARAMETER4);
	m_aExposureType.Add(IDC_AASHTO_DESIGN_PARAMETER6);
	m_aExposureType.Add(IDC_AASHTO_DESIGN_PARAMETER7);
	m_aExposureType.Add(IDC_AASHTO_DESIGN_PARAMETER8);
	m_aCorrosiveConditon.Add(IDC_AASHTO_DESIGN_PARAMETER11);
	m_aCorrosiveConditon.Add(IDC_AASHTO_DESIGN_PARAMETER12);
	m_aTndCalcMethod.Add(IDC_AASHTO_DESIGN_PARAMETER14);
	m_aTndCalcMethod.Add(IDC_AASHTO_DESIGN_PARAMETER15);
	m_aSegment.Add(IDC_AASHTO_DESIGN_PARAMETER17);
	m_aSegment.Add(IDC_AASHTO_DESIGN_PARAMETER18);
	m_aCastType.Add(IDC_JTG_DESIGN_PARAMETER15); //  Cast-in-place
	m_aCastType.Add(IDC_JTG_DESIGN_PARAMETER14); //  Precast

}

CDgnCRCDgnParameter_AASHTO12::~CDgnCRCDgnParameter_AASHTO12()
{
}


void CDgnCRCDgnParameter_AASHTO12::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCRCDgnParameter_AASHTO12)  
	DDX_Control(pDX, IDC_AASHTO_DESIGN_PARAMETER9,   m_edtExposureUser);  

	DDX_Control(pDX, IDC_AASHTO_PRINT_PARAMETER9,    m_ChkOutputOpt_ULS1);
	DDX_Control(pDX, IDC_AASHTO_PRINT_PARAMETER10,   m_ChkOutputOpt_ULS2);
	DDX_Control(pDX, IDC_AASHTO_PRINT_PARAMETER11,   m_ChkOutputOpt_ULS3);
	DDX_Control(pDX, IDC_AASHTO_PRINT_PARAMETER12,   m_ChkOutputOpt_ULS4);

	DDX_Control(pDX, IDC_AASHTO_PRINT_PARAMETER2,   m_ChkOutputOpt_SLS1);
	DDX_Control(pDX, IDC_AASHTO_PRINT_PARAMETER3,   m_ChkOutputOpt_SLS2);
	DDX_Control(pDX, IDC_AASHTO_PRINT_PARAMETER4,   m_ChkOutputOpt_SLS3);
	DDX_Control(pDX, IDC_AASHTO_PRINT_PARAMETER5,   m_ChkOutputOpt_SLS4);
	DDX_Control(pDX, IDC_AASHTO_PRINT_PARAMETER6,   m_ChkOutputOpt_SLS5);
	DDX_Control(pDX, IDC_AASHTO_PRINT_PARAMETER7,   m_ChkOutputOpt_SLS6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnCRCDgnParameter_AASHTO12, CMyChildDialog)
//{{AFX_MSG_MAP(CDgnCRCDgnParameter_AASHTO12)
ON_BN_CLICKED(IDC_AASHTO_DESIGN_PARAMETER6,   OnUserInput)
ON_BN_CLICKED(IDC_AASHTO_DESIGN_PARAMETER7,   OnUserInput)
ON_BN_CLICKED(IDC_AASHTO_DESIGN_PARAMETER8,   OnUserInput)
ON_BN_CLICKED(IDC_AASHTO_PRINT_PARAMETER13,   OnClickSelectAllOutputOpt)
ON_BN_CLICKED(IDC_AASHTO_PRINT_PARAMETER14,   OnClickUnselectAllOutputOpt)

//}}AFX_MSG_MAP
END_MESSAGE_MAP()



//////////////////////////////////////////////////////////////////////////

BOOL CDgnCRCDgnParameter_AASHTO12::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	Initial_Data();
		
	return TRUE;  

}

void CDgnCRCDgnParameter_AASHTO12::OnClickSelectAllOutputOpt()
{
	UpdateData(TRUE);
	// SET TRUE
	m_ChkOutputOpt_ULS1.SetCheck(TRUE); // 휨
	m_ChkOutputOpt_ULS2.SetCheck(TRUE); // 전단
	m_ChkOutputOpt_ULS3.SetCheck(TRUE); // 비틀림
	m_ChkOutputOpt_ULS4.SetCheck(TRUE); // 크랙

	//
	m_ChkOutputOpt_SLS1.SetCheck(TRUE);
	m_ChkOutputOpt_SLS2.SetCheck(TRUE);
	m_ChkOutputOpt_SLS3.SetCheck(TRUE);
	m_ChkOutputOpt_SLS4.SetCheck(TRUE);
	m_ChkOutputOpt_SLS5.SetCheck(TRUE);
	m_ChkOutputOpt_SLS6.SetCheck(TRUE);
	
	UpdateData(FALSE);
}

// Unselect All Output Opt.
void CDgnCRCDgnParameter_AASHTO12::OnClickUnselectAllOutputOpt()
{
	UpdateData(TRUE);
	
	m_ChkOutputOpt_ULS1.SetCheck(FALSE);
	m_ChkOutputOpt_ULS2.SetCheck(FALSE);
	m_ChkOutputOpt_ULS3.SetCheck(FALSE);
	m_ChkOutputOpt_ULS4.SetCheck(FALSE);

	m_ChkOutputOpt_SLS1.SetCheck(FALSE);
	m_ChkOutputOpt_SLS2.SetCheck(FALSE);
	m_ChkOutputOpt_SLS3.SetCheck(FALSE);
	m_ChkOutputOpt_SLS4.SetCheck(FALSE);
	m_ChkOutputOpt_SLS5.SetCheck(FALSE);
	m_ChkOutputOpt_SLS6.SetCheck(FALSE);
 
	UpdateData(FALSE);
}

void CDgnCRCDgnParameter_AASHTO12::OnUserInput()
{
	UpdateData(TRUE);

	int iExposureType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aExposureType, iExposureType);    
	GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER9)->EnableWindow(iExposureType==2);
}



void CDgnCRCDgnParameter_AASHTO12::Initial_Data()
{

	// rdo
	CDlgUtil::CtrlRadioSetCheck(this, m_aTndType           , m_pParamData->iTndnKind); 
	CDlgUtil::CtrlRadioSetCheck(this, m_aExposureType      , m_pParamData->iExposureType);                                                         
	CDlgUtil::CtrlRadioSetCheck(this, m_aCorrosiveConditon , m_pParamData->iCorrosiveCondition);
	CDlgUtil::CtrlRadioSetCheck(this, m_aTndCalcMethod     , m_pParamData->ifpsMethod);
	CDlgUtil::CtrlRadioSetCheck(this, m_aSegment           , m_pParamData->iConstructionType);

	// edit
	m_edtExposureUser.SetEditUnit(m_pParamData->dExposureFactor);
	GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER9)->EnableWindow(m_pParamData->iExposureType==2);

	// check
	m_ChkOutputOpt_ULS1.SetCheck(m_pParamData->bStrePrint4AASHTO[0]); // 휨
	m_ChkOutputOpt_ULS2.SetCheck(m_pParamData->bStrePrint4AASHTO[1]); // 전단
	m_ChkOutputOpt_ULS3.SetCheck(m_pParamData->bStrePrint4AASHTO[2]); // 비틀림
	m_ChkOutputOpt_ULS4.SetCheck(m_pParamData->bStrePrint4AASHTO[3]); // 크랙

	m_ChkOutputOpt_SLS1.SetCheck(m_pParamData->bServPrint4AASHTO[0]); // 시공중 응력
	m_ChkOutputOpt_SLS2.SetCheck(m_pParamData->bServPrint4AASHTO[1]); // 시공후 응력
	m_ChkOutputOpt_SLS3.SetCheck(m_pParamData->bServPrint4AASHTO[2]); // 긴장재 응력
	m_ChkOutputOpt_SLS4.SetCheck(m_pParamData->bServPrint4AASHTO[3]); // 주응력 - 시공중
	m_ChkOutputOpt_SLS5.SetCheck(m_pParamData->bServPrint4AASHTO[4]); // 주응력 - 시공후-전단
	m_ChkOutputOpt_SLS6.SetCheck(m_pParamData->bServPrint4AASHTO[5]); // 주응력 - 시공후-비틀림*/

}

void CDgnCRCDgnParameter_AASHTO12::SetAnnexCtrl()
{
	const auto bCA = (m_nAnnex == dgn::def::enAAshtoAmendments::Amend_California);
	GetDlgItem(IDC_JTG_DESIGN_PARAMETER13)->ShowWindow(bCA);
	GetDlgItem(IDC_JTG_DESIGN_PARAMETER14)->ShowWindow(bCA);
	GetDlgItem(IDC_JTG_DESIGN_PARAMETER15)->ShowWindow(bCA);

	int  iCastTypeCA = (m_pParamData->iCastType == 1) ? 0 : 1;
	if (m_bInitData) iCastTypeCA = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCastType, iCastTypeCA);

	return;
}


BOOL CDgnCRCDgnParameter_AASHTO12::Dlg2Data()
{
	// radio
	CDlgUtil::CtrlRadioGetCheck(this, m_aTndType           , m_pParamData->iTndnKind); 
	CDlgUtil::CtrlRadioGetCheck(this, m_aExposureType      , m_pParamData->iExposureType);    
	CDlgUtil::CtrlRadioGetCheck(this, m_aCorrosiveConditon , m_pParamData->iCorrosiveCondition);
	CDlgUtil::CtrlRadioGetCheck(this, m_aTndCalcMethod     , m_pParamData->ifpsMethod);
	CDlgUtil::CtrlRadioGetCheck(this, m_aSegment           , m_pParamData->iConstructionType);

	int  iCastTypeCA = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCastType, iCastTypeCA);
	m_pParamData->iCastType = (iCastTypeCA == 1) ? 0 : 1;
	
	// edit
	m_pParamData->dExposureFactor = m_edtExposureUser.GetEditValue();


	// check 
	m_pParamData->bStrePrint4AASHTO[0]  = m_ChkOutputOpt_ULS1.GetCheck();
	m_pParamData->bStrePrint4AASHTO[1]  = m_ChkOutputOpt_ULS2.GetCheck();
	m_pParamData->bStrePrint4AASHTO[2]  = m_ChkOutputOpt_ULS3.GetCheck(); 
	m_pParamData->bStrePrint4AASHTO[3]  = m_ChkOutputOpt_ULS4.GetCheck();  
	
	m_pParamData->bServPrint4AASHTO[0]  = m_ChkOutputOpt_SLS1.GetCheck();
	m_pParamData->bServPrint4AASHTO[1]  = m_ChkOutputOpt_SLS2.GetCheck();
	m_pParamData->bServPrint4AASHTO[2]  = m_ChkOutputOpt_SLS3.GetCheck();
	m_pParamData->bServPrint4AASHTO[3]  = m_ChkOutputOpt_SLS4.GetCheck();
	m_pParamData->bServPrint4AASHTO[4]  = m_ChkOutputOpt_SLS5.GetCheck();
	m_pParamData->bServPrint4AASHTO[5]  = m_ChkOutputOpt_SLS6.GetCheck();
	
	return TRUE;
}
