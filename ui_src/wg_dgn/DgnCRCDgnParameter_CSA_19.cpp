#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCRCDgnParameter_CSA_19.h"

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

CDgnCRCDgnParameter_CSA_19::CDgnCRCDgnParameter_CSA_19(T_PSCD_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CDgnCRCDgnParameter_CSA_19::IDD, pParent)
{
	m_pParamData = pParamData;

	m_aTndType.Add(IDC_CSA_PARAMETER2);
	m_aTndType.Add(IDC_CSA_PARAMETER3);
	m_aTndType.Add(IDC_CSA_PARAMETER4);
	m_aTndCalcMethod.Add(IDC_CSA_PARAMETER6);
	m_aTndCalcMethod.Add(IDC_CSA_PARAMETER7);
	m_aRebarEpoxy.Add(IDC_CSA_PARAMETER9);
	m_aRebarEpoxy.Add(IDC_CSA_PARAMETER10);
	m_aExposureType.Add(IDC_CSA_PARAMETER12);
	m_aExposureType.Add(IDC_CSA_PARAMETER13);
	m_aSegment.Add(IDC_CSA_PARAMETER19);
	m_aSegment.Add(IDC_CSA_PARAMETER20);
}

CDgnCRCDgnParameter_CSA_19::~CDgnCRCDgnParameter_CSA_19()
{
}


void CDgnCRCDgnParameter_CSA_19::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCRCDgnParameter_EC2PSC)  
	DDX_Control(pDX, IDC_CSA_PRINT_PARAMETER2,   m_ChkOutputOpt_ULS1);
	DDX_Control(pDX, IDC_CSA_PRINT_PARAMETER3,   m_ChkOutputOpt_ULS2);
	DDX_Control(pDX, IDC_CSA_PRINT_PARAMETER4,   m_ChkOutputOpt_ULS3);
	DDX_Control(pDX, IDC_CSA_PRINT_PARAMETER11,  m_ChkOutputOpt_ULS4);

	DDX_Control(pDX, IDC_CSA_PRINT_PARAMETER6,   m_ChkOutputOpt_SLS1);
	DDX_Control(pDX, IDC_CSA_PRINT_PARAMETER7,   m_ChkOutputOpt_SLS2);
	DDX_Control(pDX, IDC_CSA_PRINT_PARAMETER8,   m_ChkOutputOpt_SLS3);
	DDX_Control(pDX, IDC_CSA_PRINT_PARAMETER14,  m_ChkOutputOpt_SLS4);
	DDX_Control(pDX, IDC_CSA_PRINT_PARAMETER9,   m_ChkOutputOpt_SLS5);
	DDX_Control(pDX, IDC_CSA_PRINT_PARAMETER10,  m_ChkOutputOpt_SLS6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnCRCDgnParameter_CSA_19, CMyChildDialog)
	//{{AFX_MSG_MAP(CDgnCRCDgnParameter_CSA_19)
	ON_BN_CLICKED(IDC_CSA_PRINT_PARAMETER12,   OnClickSelectAllOutputOpt)
	ON_BN_CLICKED(IDC_CSA_PRINT_PARAMETER13,   OnClickUnselectAllOutputOpt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



//////////////////////////////////////////////////////////////////////////

BOOL CDgnCRCDgnParameter_CSA_19::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	Initial_Data();

	return TRUE;  

}

void CDgnCRCDgnParameter_CSA_19::OnClickSelectAllOutputOpt()
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
void CDgnCRCDgnParameter_CSA_19::OnClickUnselectAllOutputOpt()
{
	UpdateData(TRUE);

	// SET TRUE
	m_ChkOutputOpt_ULS1.SetCheck(FALSE);
	m_ChkOutputOpt_ULS2.SetCheck(FALSE);
	m_ChkOutputOpt_ULS3.SetCheck(FALSE);
	m_ChkOutputOpt_ULS4.SetCheck(FALSE);

	//
	m_ChkOutputOpt_SLS1.SetCheck(FALSE); //
	m_ChkOutputOpt_SLS2.SetCheck(FALSE);
	m_ChkOutputOpt_SLS3.SetCheck(FALSE);
	m_ChkOutputOpt_SLS4.SetCheck(FALSE);
	m_ChkOutputOpt_SLS5.SetCheck(FALSE);
	m_ChkOutputOpt_SLS6.SetCheck(FALSE);

	UpdateData(FALSE);
}



void CDgnCRCDgnParameter_CSA_19::Initial_Data()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aTndType      , m_pParamData->iTndnKind);
	CDlgUtil::CtrlRadioSetCheck(this, m_aTndCalcMethod, m_pParamData->ifpsMethod);
	CDlgUtil::CtrlRadioSetCheck(this, m_aRebarEpoxy   , m_pParamData->iCorrosiveCondition);
	CDlgUtil::CtrlRadioSetCheck(this, m_aExposureType , m_pParamData->iExposureClass);
	CDlgUtil::CtrlRadioSetCheck(this, m_aSegment      , m_pParamData->iConstructionType);

	m_ChkOutputOpt_ULS1.SetCheck(m_pParamData->bStrePrint4AASHTO[0]); // 휨
	m_ChkOutputOpt_ULS2.SetCheck(m_pParamData->bStrePrint4AASHTO[1]); // 전단
	m_ChkOutputOpt_ULS3.SetCheck(m_pParamData->bStrePrint4AASHTO[2]); // 비틀림
	m_ChkOutputOpt_ULS4.SetCheck(m_pParamData->bStrePrint4AASHTO[3]); // 크랙

	m_ChkOutputOpt_SLS1.SetCheck(m_pParamData->bServPrint4AASHTO[0]); // 시공중 응력
	m_ChkOutputOpt_SLS2.SetCheck(m_pParamData->bServPrint4AASHTO[1]); // 시공후 응력
	m_ChkOutputOpt_SLS3.SetCheck(m_pParamData->bServPrint4AASHTO[2]); // 긴장재 응력
	m_ChkOutputOpt_SLS4.SetCheck(m_pParamData->bServPrint4AASHTO[3]); // 주응력 - 시공중
	m_ChkOutputOpt_SLS5.SetCheck(m_pParamData->bServPrint4AASHTO[4]); // 주응력 - 시공후-전단
	m_ChkOutputOpt_SLS6.SetCheck(m_pParamData->bServPrint4AASHTO[5]); // 주응력 - 시공후-비틀림

}


BOOL CDgnCRCDgnParameter_CSA_19::Dlg2Data()
{
	// radio
	CDlgUtil::CtrlRadioGetCheck(this, m_aTndType      , m_pParamData->iTndnKind);
	CDlgUtil::CtrlRadioGetCheck(this, m_aTndCalcMethod, m_pParamData->ifpsMethod);
	CDlgUtil::CtrlRadioGetCheck(this, m_aRebarEpoxy   , m_pParamData->iCorrosiveCondition);
	CDlgUtil::CtrlRadioGetCheck(this, m_aExposureType , m_pParamData->iExposureClass);
	CDlgUtil::CtrlRadioGetCheck(this, m_aSegment      , m_pParamData->iConstructionType);

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
