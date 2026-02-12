#include "stdafx.h"
#include "wg_cmd.h"
#include "ExportFunc.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_ModelessDlgCtrl.h"
/*!@#$*/ #include "..\MIT_frx\MBaseTabWnd.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

// Modeless Dialog
#include "CMPropertiesDlg.h"
#include "CMLoadCaseDlg.h"
#include "CMLoadCombDlg.h"
#include "CMLoadCombDefDlg.h"
#include "CMSelectDlg.h"
#include "CMStorDlg.h"
#include "CMLlanDlg.h"
#include "CMLlanCHDlg.h"
#include "CMLlanIDDlg.h"
#include "CMLlanFRDlg.h"
#include "CMLlanJPDlg.h"
#include "CMLlanTrDlg.h"
#include "CMLlanOpDlg.h"
#include "CMSlanDlg.h"
#include "CMSlanCHDlg.h"
#include "CMSlanFRDlg.h"
#include "CMSlanOpDlg.h"
#include "CMHydrCoolElemItem.h"
#include "CMHydStageConsDlg.h"
#include "CMTendonDefDlg.h"
#include "CMAddImpfDlg.h"     // MNET:2966-JHLEE-20070921 // add

#include "CMBndrGrupDefDlg.h" // Modeless Or Modal Dialog
#include "CMLoadGroupDlg.h"
#include "CMStrtGrupDefDlg.h"
#include "CMLdarGrupDefDlg.h"
#include "CMStageDefineDlg.h"
#include "CMStageDefMainDlg.h"
#include "CMStageConsDlg.h"
#include "CMTendonTypeDlg.h"
#include "CMTendonTypeAddDlg.h"
#include "CMStageChangeDlg.h"
#include "CMTendonDlg.h"
#include "CMTendonAddDlg.h"
#include "CMTendonProfileDlg.h"
#include "CMSectScaleFactorDlgNew.h"
#include "CMSectScaleFactorDlgNew_GEN.h"
#include "CMRebarDlg.h"
#include "CMRebarPscDlg.h"
#include "CMRposListDlg.h"
#include "MLoadPNLDDlg.h"
#include "CMElsPlsMatlDlg.h"
#include "CMElsplsMatlAddDlg.h"
#include "CMElsPlsParamDlg.h"
#include "CMGilcListDlg.h"
#include "CMBridgeTypeDlg.h"
#include "CMLoadLineListDlg.h"
#include "CMLoadLineItemDlg.h"
#include "CMGridP1LoadFactorTypeDlg.h"
#include "CMEBeamPageDlg.h"
#include "CMStripPageDlg.h"
#include "CMDropPanelPageDlg.h"
#include "CMColmCapitalPageDlg.h"
#include "CMEBeamItemDlg.h"
#include "CMStripItemDlg.h"
#include "CMDropPanelItemDlg.h"
#include "CMColmCapitalItemDlg.h"
#include "CMTHLoadCombListDlg.h"
#include "CMLoadDirectionGroupDlg.h"
#include "CMLoadFireLoadAnalysDlg.h"
#include "CMTlgtDlg.h"
#include "CmdDampGBCoefCalcDlg.h"

// Bar Tree Dlg
#include "CMSpecDlg.h"
#include "CMNplnDlg.h"
#include "CMNplnDlgNew.h"
#include "CMThMainDlg.h"
#include "CMMoveMainDlg.h"
#include "CMMlspDlg.h"
#include "CMMlsrDlg.h"
#include "CMSinfDlg.h"
#include "CMBgenDlg.h"
#include "PushoverHingeMainDlg.h"
#include "CMIepiMainDlg.h"
#include "InelasticHingeMainDlg.h"
#include "CMHcrpDlg.h"
#include "TdmeItemDlg.h"
#include "CMTdmfItemDlg.h"
#include "CMLLaneJPDlg.h"
#include "CMGridMainGirderListDlg.h"
#include "CMGridMainGirderItemDlg.h"
#include "CMGridCrossBeamListDlg.h"
#include "CMGridCrossBeamItemDlg.h"
#include "CMGridModelAnalDataDlg.h"
#include "CmdBarNmrdDlg.h"
#include "CMSihoMainDlg.h"
#include "CMGridImportDxfDlg.h"
#include "TemperatureRisePropItemDlg.h"

// modal dlg
#include "MActiveIdentityDlg.h"
#include "CMSectionPage.h"
#include "CMDLcrsDlg.h"
#include "CMExTypeForPretenDlg.h"
#include "CMStageCompSectItemDlg.h"
#include "CMDTendonTemplateDlg.h"
#include "CMPsltDlg.h"
#include "CMSdviDlg.h"
#include "CMSdveDlg.h"
#include "CMSdstDlg.h"
#include "CMSdhyDlg.h"
#include "CMSdisDlg.h"
#include "AntivibrationSupportGBItemDlg.h"
#include "DamperGBItemDlg.h"
#include "RSModifyFactorDlg.h"
#include "CMGridAnalCtrlDlg.h"
#include "CMNlaSeqDlg.h"
#include "CMNlacDlg.h"
#include "CMNlacDlg_MEC.h"
#include "FluegasTempListDlg.h"
#include "SteelTimeTempListDlg.h"
#include "FluegasTempItemDlg.h"
#include "SteelTimeTempItemDlg.h"
#include "CMTendonChangeProfDlg.h"
#include "CmdCheckNearNodesDlg.h"
#include "CmdSFRLoadDlg.h"
#include "ImportTHLoadCombListDlg.h"
#include "CmdDampCheckInfoDlg.h"
#include "CmdTotalAppraisalInformationDlg.h"
#include "CmdReinforceInfoDlg.h"
#include "CmdReinforceDesignDlg.h"
#include "CmdReinforceSectDlg.h"
#include "ThisItemDlg.h"
#include "ThisItemDlg_MEC.h"
#include "ThisListDlg.h"
#include "CMPostTensionTendonProfileDlg.h"
#include "CMPostTensionTendonStressingDlg.h"
#include "PostTensionModifyTendonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// code guru : Dialog in DLL 참조

// 다른 DLL에서 wg_cmd에 있는 다이얼로그를 띄울 수 있게 하는
// 창구역할을 하는 함수이다. 새로운 다이얼로그가 추가되면 
// switch문에서 추가해 주어야 한다.

// 인자로 void* pExtData를 추가해 주었다. (by TAE)
// 이 변수는 외부 프로젝트에서 CMD Project의 Modaless Dialog에 Data를 Setting 해주기 위한 목적이며,
// 오류의 소지가 많으므로 조심하여 쓰도록 한다. 
// pArg로 Key가 넘어올 수도 있고, Data가 넘어올 수도 있다.
extern __declspec(dllexport) void* CreateOrActivateDlg(CDBDoc* pDoc, UINT nIDTemplate, CWnd* pParent/*=NULL*/, void* pExtData/*=NULL*/)
{
	CCMDlgBase* pDlg = (CCMDlgBase*)(pDoc->m_pDlgCtrl->GetDlg(nIDTemplate));

	DWORD dwLastErr = NO_ERROR;
	if (pDlg == 0) // Dialog가 없으면 만든다.
	{
		// 확장 DLL에서는 AFX_MANAGE_STATE 매크로를 호출하면 안된다.
		//AFX_MANAGE_STATE(AfxGetStaticModuleState());
		switch(nIDTemplate)
		{
		case CCMPropertiesDlg::IDD:
			pDlg = new CCMPropertiesDlg(pParent);
			break;
		case CCMLoadCaseDlg::IDD:
			pDlg = new CCMLoadCaseDlg(pParent);
			break;
		case CCMLoadCombDlg::IDD:
			{
				pDlg = new CCMLoadCombDlg(pParent);
				if(pExtData)
				{
					auto pLcomType = (int*)pExtData;
					LCOM_TYPE nSeisType = LCOM_GENERAL;
					switch (*pLcomType)
					{
					case D_LCOMTYPE_SEISMIC:
						nSeisType = LCOM_SEISMIC;
						break;
					case D_LCOMTYPE_LINEAR_EVAL:
						nSeisType = LCOM_LINEAR;
						break;
					default:
						nSeisType = LCOM_GENERAL;
						break;
					}

					((CCMLoadCombDlg*)pDlg)->SetSeismicEvaluation(nSeisType);
					((CCMLoadCombDlg*)pDlg)->SetLcomType(*pLcomType);
				}
				break;
			}
			case CCMLoadCombDefDlg::IDD:	
				{
					pDlg = new CCMLoadCombDefDlg(pParent);
					break;
				}
		case CCMSelectDlg::IDD:
			pDlg = new CCMSelectDlg(pParent);
			break;
		case CCMStorDlg::IDD:
			pDlg = new CCMStorDlg(pParent);
			break;
		case CCMLlanDlg::IDD:
			pDlg = new CCMLlanDlg(pParent);
			break;
		case CCMLlanCHDlg::IDD:
			pDlg = new CCMLlanCHDlg(pParent);
			break;
		case CCMLlanIDDlg::IDD:
			pDlg = new CCMLlanIDDlg(pParent);
			break;
		case CCMLlanFRDlg::IDD:
			pDlg = new CCMLlanFRDlg(pParent);
			break;
		case CCMLlanJPDlg::IDD:
			pDlg = new CCMLlanJPDlg(pParent);
			break;
		case CCMLlanTrDlg::IDD:
			pDlg = new CCMLlanTrDlg(pParent);
			break;
		case CCMLlanOpDlg::IDD:
			pDlg = new CCMLlanOpDlg(pParent);
			break;
		case CCMSlanDlg::IDD:
			pDlg = new CCMSlanDlg(pParent);
			break;
		case CCMSlanCHDlg::IDD:
			pDlg = new CCMSlanCHDlg(pParent);
			break;
		case CCMSlanFRDlg::IDD:
			pDlg = new CCMSlanFRDlg(pParent);
			break;
		case CCMSlanOpDlg::IDD:
			pDlg = new CCMSlanOpDlg(pParent);
			break;
		case CCMLLaneJPDlg::IDD:
			pDlg = new CCMLLaneJPDlg(pParent);
			break;    
		case CMActiveIdentityDlg::IDD:
			pDlg = new CMActiveIdentityDlg(pParent);
			break;
		case CCMHydrCoolElemItem::IDD:
			pDlg = new CCMHydrCoolElemItem(pParent);
			break;
		case CCMBndrGrupDefDlg::IDD:
			pDlg = new CCMBndrGrupDefDlg(pParent);
			break;
		case CCMLoadGroupDlg::IDD:
			pDlg = new CCMLoadGroupDlg(pParent);
			break;
		case CCMStrtGrupDefDlg::IDD:
			pDlg = new CCMStrtGrupDefDlg(pParent);
			break;
		case CCMLdarGrupDefDlg::IDD:
			pDlg = new CCMLdarGrupDefDlg(pParent);
			break;
		//case CCMStageDefineDlg::IDD:
		//  pDlg = new CCMStageDefineDlg(pParent);
		//  break;
		
		//case CCMStageDefMainDlg::IDD:
		//  pDlg = new CCMStageDefMainDlg(pParent);
		//  break;
		case CCMTendonDlg::IDD:
			pDlg = new CCMTendonDlg(pParent);
			break;
		case CCMTendonProfileDlg::IDD:
			pDlg = new CCMTendonProfileDlg(pParent);
			break;
		case CCMTendonAddDlg::IDD:
			pDlg = new CCMTendonAddDlg(pParent);
			break;
		case CMLoadPNLDDlg::IDD:  //KYE-HONG
			pDlg = new CMLoadPNLDDlg(pParent);
			break;
		case CCMGridMainGirderListDlg::IDD:
			pDlg = new CCMGridMainGirderListDlg(pParent);
			break;
		case CCMGridMainGirderItemDlg::IDD:
			pDlg = new CCMGridMainGirderItemDlg(pParent);
			pDlg->SetInitPos(D_INIT_POS_LB);
			break;
		case CCMGridCrossBeamListDlg::IDD:
			pDlg = new CCMGridCrossBeamListDlg(pParent);
			break;
		case CCMGridCrossBeamItemDlg::IDD:
			pDlg = new CCMGridCrossBeamItemDlg(pParent);
			pDlg->SetInitPos(D_INIT_POS_LB);
			break;
		case CCMLoadLineListDlg::IDD:
			pDlg = new CCMLoadLineListDlg(pParent);
			break;
		case CCMLoadLineItemDlg::IDD:
			pDlg = new CCMLoadLineItemDlg(pParent);
			pDlg->SetInitPos(D_INIT_POS_LB);
			break;
		case CCMTendonDefDlg::IDD: // add
			pDlg = new CCMTendonDefDlg(pParent);
			break;
			/*
		case CCMGridP1LoadFactorTypeDlg::IDD:
			pDlg = new CCMGridP1LoadFactorTypeDlg(pParent);
			break;
			*/
		case CCMGridImportDxfDlg::IDD:
			pDlg = new CCMGridImportDxfDlg(pParent);
			pDlg->SetInitPos(D_INIT_POS_LB);
			break;
		case CCMDTendonTemplateDlg::IDD:
			pDlg = new CCMDTendonTemplateDlg(pParent);
			break;
		case CCMStripPageDlg::IDD:
			pDlg = new CCMStripPageDlg(pParent);
			break;
		case CCMEBeamPageDlg::IDD:
			pDlg = new CCMEBeamPageDlg(pParent);
			break;    
		case CCMDropPanelPageDlg::IDD:
			pDlg = new CCMDropPanelPageDlg(pParent);
			break;      
		case CCMColmCapitalPageDlg::IDD:
			pDlg = new CCMColmCapitalPageDlg(pParent);
			break;    
		case CCMTHLoadCombListDlg::IDD:
			pDlg = new CCMTHLoadCombListDlg(pParent);
			break;    
		case CImportTHLoadCombListDlg::IDD:
			pDlg = new CImportTHLoadCombListDlg(pParent);
			break;
		case CCMLoadDirectionGroupDlg::IDD:
			pDlg = new CCMLoadDirectionGroupDlg(pParent);
			break;
		case CCMTlgtDlg::IDD:
			pDlg = new CCMTlgtDlg(pParent);
			break;
		case CCmdCheckNearNodesDlg::IDD:
			pDlg = new CCmdCheckNearNodesDlg(pParent);
			break;
		case CCmdSFRLoadDlg::IDD:
			pDlg = new CCmdSFRLoadDlg(pParent);
			break;
		case CCMNlacDlg::IDD:
			pDlg = new CCMNlacDlg(pParent);
			break;
		case CCMNlacDlg_MEC::IDD:
			pDlg = new CCMNlacDlg_MEC(pParent);
			break;
		case CThisListDlg::IDD:
			pDlg = new CThisListDlg(pParent);
			break;
		case CThisItemDlg::IDD:
			pDlg = new CThisItemDlg(pParent);
			break;
		case CThisItemDlg_MEC::IDD:
			pDlg = new CThisItemDlg_MEC(pParent);
			break;
		case CCMNlaSeqDlg::IDD:
			pDlg = new CCMNlaSeqDlg(pParent);
			break;
		default:
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Not_registered_Dialog));
			return 0;
		}
		pDlg->m_pDoc = pDoc;
		pDlg->Create(nIDTemplate, pParent);
		dwLastErr = ::GetLastError();
	}
	if (::GetLastError() == NO_ERROR)
		::SetLastError(dwLastErr);

	if(nIDTemplate == CCMTendonDlg::IDD)
	{
		if(!((CCMTendonDlg*)pDlg)->IsShowProfDlg())
			pDlg->ShowWindow(SW_SHOW);
	}
	else if(nIDTemplate == CCMDTendonTemplateDlg::IDD)
	{
		if(((CCMDTendonTemplateDlg*)pDlg)->GetShowWnd())
			pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg->ShowWindow(SW_SHOW);
	}
	
	// 여기서 External Data Setting 작업을 해준다.
	if(pExtData != NULL)  // 넘어온 Data Setting 관련 인자가 있으면 
	{
		switch(nIDTemplate)
		{
		case CCMLlanDlg::IDD:
			{
				T_LLAN_K LlanKey = *((T_LLAN_K *)pExtData);
				((CCMLlanDlg *)pDlg)->ExternalSetting(LlanKey);
			}
			break;
		case CCMLlanCHDlg::IDD:
			{
				T_LLANch_K LlanKey = *((T_LLANch_K *)pExtData);
				((CCMLlanCHDlg *)pDlg)->ExternalSetting(LlanKey);
			}
			break;
		case CCMLlanFRDlg::IDD:
			{
				T_LLANfr_K LlanKey = *((T_LLANfr_K *)pExtData);
				((CCMLlanFRDlg *)pDlg)->ExternalSetting(LlanKey);
			}
			break;
		case CCMLlanJPDlg::IDD:
			{
				T_LLANjp_K LlanKey = *((T_LLANjp_K *)pExtData);
				((CCMLlanJPDlg *)pDlg)->ExternalSetting(LlanKey);
			}
			break;
		case CCMLlanTrDlg::IDD:
			{
				T_LLANtr_K LlantrKey = *((T_LLANtr_K *)pExtData);
				((CCMLlanTrDlg *)pDlg)->ExternalSetting(LlantrKey);
			}
			break;
		case CCMLlanOpDlg::IDD:
			{
				T_LLANop_K LlanopKey = *((T_LLANop_K *)pExtData);
				((CCMLlanOpDlg *)pDlg)->ExternalSetting(LlanopKey);
			}
			break;
		case CCMSlanDlg::IDD:
			{
				T_SLAN_K SlanKey = *((T_SLAN_K *)pExtData);
				((CCMSlanDlg *)pDlg)->ExternalSetting(SlanKey);
			}
			break;
		case CCMSlanCHDlg::IDD:
			{
				T_SLANch_K SlanKey;
				SlanKey = *((T_SLANch_K *)pExtData);
				((CCMSlanCHDlg *)pDlg)->ExternalSetting(SlanKey);
			}
			break;
		case CCMSlanFRDlg::IDD:
			{
				T_SLANfr_K SlanKey;
				SlanKey = *((T_SLANfr_K *)pExtData);
				((CCMSlanFRDlg *)pDlg)->ExternalSetting(SlanKey);
			}
			break;
		case CCMSlanOpDlg::IDD:
			{
				T_SLANop_K SlanopKey = *((T_SLANop_K *)pExtData);
				((CCMSlanOpDlg *)pDlg)->ExternalSetting(SlanopKey);
			}
			break;
		case CCMTendonProfileDlg::IDD:
			{
				T_TDNA_K TdnaK;
				TdnaK = *((T_TDNA_K *)pExtData);
				((CCMTendonProfileDlg *)pDlg)->ExternalSetting(TdnaK);
			}
			break;
		case CCMGridMainGirderItemDlg::IDD:
			{
				T_MGDR_K MgdrKey = *((T_MGDR_K *)pExtData);
				((CCMGridMainGirderItemDlg *)pDlg)->ExternalSetting(MgdrKey);
			}
			break;
		case CCMGridCrossBeamItemDlg::IDD:
			{
				T_CBEM_K CbemKey = *((T_CBEM_K *)pExtData);
				((CCMGridCrossBeamItemDlg *)pDlg)->ExternalSetting(CbemKey);
			}
			break;
		case CCMLoadLineItemDlg::IDD:
			{
				T_LDLN_K LdlnKey = *((T_LDLN_K *)pExtData);
				((CCMLoadLineItemDlg *)pDlg)->ExternalSetting(LdlnKey);
			}
			break;
		case CThisItemDlg::IDD:
			{
			T_THIS_K ThisKey = *((T_THIS_K *)pExtData);
			((CThisItemDlg *)pDlg)->ExternalSetting(ThisKey);
		}
			break;
		case CThisItemDlg_MEC::IDD:
		{
			T_THIS_K ThisKey = *((T_THIS_K*)pExtData);
			((CThisItemDlg_MEC*)pDlg)->ExternalSetting(ThisKey);
		}
		break;
		default: 
			break;
		}
	}

	//pDlg->SetFocus();
	return pDlg;
}


///////////////////////////////////////////////////////////////////////////////////
// Tree Menu related export functions
//!@#$extern __declspec(dllexport) void AddTabToTabWnd(SEC3DTabWnd* pTabWnd, CRuntimeClass* pClass, CString sTabName)
extern __declspec(dllexport) void AddTabToTabWnd(mit::frx::MBaseTabWnd* pTabWnd, CRuntimeClass* pClass, CString sTabName)
{
	pTabWnd->AddTab(pClass, sTabName);
}

//!@#$extern __declspec(dllexport) void CMD_CreateBarTreeDlg(CWnd* pParent, SEC3DTabWnd* pTabWnd, int nWhich)
extern __declspec(dllexport) void CMD_CreateBarTreeDlg(CWnd* pParent, mit::frx::MBaseTabWnd* pTabWnd, int nWhich, CString& curString)
{
	if(!::IsWindow(pTabWnd->GetSafeHwnd()))
	{
		pTabWnd->Create(pParent,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
		switch(nWhich)
		{
		case __TMMODE_CMD_SPEC__:
			pTabWnd->AddTab(RUNTIME_CLASS(CCMSpecDlg),_LS(IDS_WG_CMD__ADDD__Response_Spectrum_Load_Cases));	
#ifdef _MGEN
			pTabWnd->SetChildMaxWidth(124);
#endif // _MGEN			
			curString = _LS(IDS_WG_CMD__ADDD__Response_Spectrum_Load_Cases);
			break;
		case __TMMODE_CMD_NPLN__:
			//pTabWnd->AddTab(RUNTIME_CLASS(CCMNplnDlg),_LS(IDS_WG_CMD__ADDD__Named_Plane));
			pTabWnd->AddTab(RUNTIME_CLASS(CCMNplnDlgNew),_LS(IDS_WG_CMD__ADDD__Named_Plane));
			curString = _LS(IDS_WG_CMD__ADDD__Named_Plane);
			break;
			
		case __TMMODE_CMD_THGA__:
		case __TMMODE_CMD_THNL__:
		case __TMMODE_CMD_THSP__:
		case __TMMODE_CMD_THMS__:
		case __TMMODE_CMD_THRD__:
		case __TMMODE_CMD_THRD_DISP__:
		case __TMMODE_CMD_THRD_TRUSS__:
		case __TMMODE_CMD_THRD_BEAM__:
		case __TMMODE_CMD_THRD_PLANE_STRESS__:
		case __TMMODE_CMD_THRD_PLANE_STRAIN__:
		case __TMMODE_CMD_THRD_PLATE__:
		case __TMMODE_CMD_THRD_SOLID__:
		case __TMMODE_CMD_THRD_GENERAL_LINK__:
		case __TMMODE_CMD_THRD_INELASTIC_HINGE__:
		case __TMMODE_CMD_THRC_TIME_INCREMENT__:
		case __TMMODE_CMD_THRD_STEP_TIMESTEP__:
		case __TMMODE_CMD_THRD_STEP_DISPVELACC__:
		case __TMMODE_CMD_THRD_STEP_TRUSS__:
		case __TMMODE_CMD_THRD_STEP_BEAM__:
		case __TMMODE_CMD_THRD_STEP_DESIGDOF__:
		case __TMMODE_CMD_THAT__:
		case __TMMODE_CMD_THCR__:
			pTabWnd->AddTab(RUNTIME_CLASS(CCMThMainDlg),_LS(IDS_WG_CMD__Time_History_Analysis_Data));
			curString = _LS(IDS_WG_CMD__Time_History_Analysis_Data);
			break;
		
/*    case __TMMODE_CMD_MLSP__:
			pTabWnd->AddTab(RUNTIME_CLASS(CCMMlspDlg),_LS(IDS_WG_CMD__ADDD__Lane_Supports));
			break;
		case __TMMODE_CMD_MLSR__:
			pTabWnd->AddTab(RUNTIME_CLASS(CCMMlsrDlg),_LS(IDS_WG_CMD__ADDD__Lane_Supports));
			break;
		case __TMMODE_CMD_SINF__:
			pTabWnd->AddTab(RUNTIME_CLASS(CCMSinfDlg),_LS(IDS_WG_CMD__ADDD__Plate_Element_for_Influence_Surfa));
			break;
*/
		case __TMMODE_CMD_MLSP__:
		case __TMMODE_CMD_MLSR__:
		case __TMMODE_CMD_MLTT__:
		case __TMMODE_CMD_ADD_IMPF__:         // MNET:2966-JHLEE-20070921
		case __TMMODE_CMD_SINF__:
		case __TMMODE_CMD_DYNF__:
		case __TMMODE_CMD_JIMP__:
			pTabWnd->AddTab(RUNTIME_CLASS(CCMMoveMainDlg),_LS(IDS_CMD_MOVING_LOAD));
			curString = _LS(IDS_CMD_MOVING_LOAD);
			break;

		case __TMMODE_CMD_BGEN__:
			pTabWnd->AddTab(RUNTIME_CLASS(CCMBgenDlg),_LS(IDS_WG_CMD__ADDD__Building_Generation));
			curString = _LS(IDS_WG_CMD__ADDD__Building_Generation);
			break;
		//case __TMMODE_CMD_POHA__://Pushover Hinge Assign
		//	pTabWnd->AddTab(RUNTIME_CLASS(CPushOverHingeAssignDlg),_LS(IDS_WG_CMD__ADDD__Assign_PushOver_Hinge));
		//  break;
 		case __TMMODE_CMD_IEHA__://Assign Inelastic Hinges
			pTabWnd->AddTab(RUNTIME_CLASS(CInelasticHingeMainDlg),_LS(IDS_WG_CMD__ADDD__Assign_Inelastic_Hinge));
			curString = _LS(IDS_WG_CMD__ADDD__Assign_Inelastic_Hinge);
//			pTabWnd->AddTab(RUNTIME_CLASS(CInelasticHingeAssignDlg),_LS(IDS_WG_CMD__ADDD__Assign_Inelastic_Hinge));
			break;
 		case __TMMODE_CMD_POHA__:   //Assign Pushover Hinges
			pTabWnd->AddTab(RUNTIME_CLASS(CPushoverHingeMainDlg), _LS(IDS_WG_CMD_Pushover_Analysis));
			curString = _LS(IDS_WG_CMD_Pushover_Analysis);
			break;
		case __TMMODE_CMD_IEPI__:
			pTabWnd->AddTab(RUNTIME_CLASS(CCMIepiMainDlg), _LS(IDS_DB_DT_IEPI));
			curString = _LS(IDS_DB_DT_IEPI);
			break;
		case __TMMODE_CMD_BSPN__:
		case __TMMODE_CMD_GIMP__:
		case __TMMODE_CMD_P1LA__:
		case __TMMODE_CMD_CREF__:
		case __TMMODE_CMD_GLTM__:
		case __TMMODE_CMD_LNKF__:
		case __TMMODE_CMD_GSTY__:
			pTabWnd->AddTab(RUNTIME_CLASS(CCMGridModelAnalDataDlg),_LS(IDS_CMD_GRID_MODEL_ANAL_DATA));
			curString = _LS(IDS_CMD_GRID_MODEL_ANAL_DATA);
			break;
		case __TMMODE_CMD_NMRD__:
			pTabWnd->AddTab(RUNTIME_CLASS(CCmdBarNmrdDlg),_LS(IDS_CMD_Reduction_Moment_at_the_inner_support));
			curString = _LS(IDS_CMD_Reduction_Moment_at_the_inner_support);
			break;  
		case __TMMODE_CMD_SIHO_ELEM__	:
		case __TMMODE_CMD_SIHO_GLINK__	:
		case __TMMODE_CMD_SIHO_PSSPOT__ :
			pTabWnd->AddTab(RUNTIME_CLASS(CCMSihoMainDlg),_LS(IDS_DB_DT_SIHO));
			curString = _LS(IDS_DB_DT_SIHO);
			break;
		case __TMMODE_CMD_SPHO_ELEM__	:
		case __TMMODE_CMD_SPHO_GLINK__	:
			pTabWnd->AddTab(RUNTIME_CLASS(CCMSihoMainDlg),_LS(IDS_DB_DT_SPHO));
			curString = _LS(IDS_DB_DT_SPHO);
			break;
		}
	}
	pTabWnd->ActivateTab(0);

	switch(nWhich)
	{
	case __TMMODE_CMD_THGA__:
	case __TMMODE_CMD_THNL__:
	case __TMMODE_CMD_THSP__:
	case __TMMODE_CMD_THMS__:
	case __TMMODE_CMD_THRD__:
	case __TMMODE_CMD_THRD_DISP__:
	case __TMMODE_CMD_THRD_TRUSS__:
	case __TMMODE_CMD_THRD_BEAM__:
	case __TMMODE_CMD_THRD_PLANE_STRESS__:
	case __TMMODE_CMD_THRD_PLANE_STRAIN__:
	case __TMMODE_CMD_THRD_PLATE__:
	case __TMMODE_CMD_THRD_SOLID__:
	case __TMMODE_CMD_THRD_GENERAL_LINK__:
	case __TMMODE_CMD_THRD_INELASTIC_HINGE__:
	case __TMMODE_CMD_THRC_TIME_INCREMENT__:
	case __TMMODE_CMD_THRD_STEP_TIMESTEP__:
	case __TMMODE_CMD_THRD_STEP_DISPVELACC__:
	case __TMMODE_CMD_THRD_STEP_TRUSS__:
	case __TMMODE_CMD_THRD_STEP_BEAM__:
	case __TMMODE_CMD_THRD_STEP_DESIGDOF__:
	case __TMMODE_CMD_THAT__:
	case __TMMODE_CMD_THCR__:
		{
			//!@#$
			CWnd * pWnd = NULL;
			pTabWnd->GetActiveTab(pWnd);
			if (pWnd)
			{
				CCMThMainDlg* pSttlDlgView = (CCMThMainDlg*)pWnd;
				pSttlDlgView->ShowChildDlgByID(nWhich);

				int nSel = pSttlDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pSttlDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				curString = strSelected;
			}
		}
		break;
	case __TMMODE_CMD_MLSP__:
	case __TMMODE_CMD_MLSR__:
	case __TMMODE_CMD_MLTT__:
	case __TMMODE_CMD_ADD_IMPF__:   // MNET:2966-JHLEE-20070921
	case __TMMODE_CMD_SINF__:
	case __TMMODE_CMD_DYNF__:
	case __TMMODE_CMD_JIMP__:
		{
			CWnd * pWnd = NULL;
			pTabWnd->GetActiveTab(pWnd);
			if (pWnd)
			{
				CCMMoveMainDlg* pDlgView = (CCMMoveMainDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nWhich);

				int nSel = pDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				curString = strSelected;
			}
		}
		break;
	case __TMMODE_CMD_IEHA__:
		{
			CWnd * pWnd = NULL;
			pTabWnd->GetActiveTab(pWnd);
			if (pWnd)
			{
				CInelasticHingeMainDlg* pDlgView = (CInelasticHingeMainDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nWhich);

				int nSel = pDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				curString = strSelected;
			}
		}
		break;
	case __TMMODE_CMD_POHA__:
		{
			CWnd * pWnd = NULL;
			pTabWnd->GetActiveTab(pWnd);
			if (pWnd)
			{
				CPushoverHingeMainDlg* pDlgView = (CPushoverHingeMainDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nWhich);

				int nSel = pDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				curString = strSelected;
			}
		}
		break;
	case __TMMODE_CMD_IEPI__:
		{
			CWnd * pWnd = NULL;
			pTabWnd->GetActiveTab(pWnd);
			if (pWnd)
			{
				CCMIepiMainDlg* pDlgView = (CCMIepiMainDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nWhich);

				int nSel = pDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				curString = strSelected;
			}
		}
		break;
	case __TMMODE_CMD_BSPN__:
	case __TMMODE_CMD_GIMP__:
	case __TMMODE_CMD_P1LA__:
	case __TMMODE_CMD_CREF__:
	case __TMMODE_CMD_GLTM__:
	case __TMMODE_CMD_LNKF__:
	case __TMMODE_CMD_GSTY__:
		{
			CWnd * pWnd = NULL;
			pTabWnd->GetActiveTab(pWnd);
			if (pWnd)
			{
				CCMGridModelAnalDataDlg* pDlgView = (CCMGridModelAnalDataDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nWhich);

				int nSel = pDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				curString = strSelected;
			}
			break;
		}
	case __TMMODE_CMD_SIHO_ELEM__:
	case __TMMODE_CMD_SIHO_GLINK__:
	case __TMMODE_CMD_SIHO_PSSPOT__:
	case __TMMODE_CMD_SPHO_ELEM__:
	case __TMMODE_CMD_SPHO_GLINK__:
		{
			CWnd * pWnd = NULL;
			pTabWnd->GetActiveTab(pWnd);
			if (pWnd)
			{
				CCMSihoMainDlg* pDlgView = (CCMSihoMainDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nWhich);

				int nSel = pDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				curString = strSelected;
			}
		}
		break;
	}  
}

//////////////////////////////////////////////////////////////////////////
// CMD_DoModal 와 같이 외부 프로젝트 실행 함수에 호출 클래스가 많아지면 스택 오버플로우가 일어난다.
// 호출 클래스 개수는 30개가 넘지 않도록하며 CMD_DoModal을 계속 분리하도록한다.
extern __declspec(dllexport) void CMD_DoModal(int nWhich, void* Key/* = NULL*/)
{
	switch(nWhich)
	{
	case D_CMD_SECT_SCALE_FACTOR:
		{
#if defined (_CIVIL)
			CCMSectScaleFactorDlgNew* pDlg = new CCMSectScaleFactorDlgNew;
#else
			CCMSectScaleFactorDlgNew_GEN* pDlg = new CCMSectScaleFactorDlgNew_GEN;
#endif
			pDlg->DoModal();
			delete pDlg;
			//CCMSectScaleFactorDlg Dlg;
			//pDlg->DoModal();
		}
		break;
	case D_CMD_TDMF_MODIFY     :
		{
			CCMTdmfItemDlg* pDlg = new CCMTdmfItemDlg;
			if(Key)
			{
				T_TDMF_D TdmfD;
				if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdmf(*((T_TDMF_K*)Key),TdmfD))
				{
					pDlg->SetParamData(TdmfD);
					pDlg->DoModal();
					delete pDlg;
				}
			}
		}
		break;
	case D_CMD_TDME_ELASTICITY     :
		{
			CTdmeItemDlg* pDlg = new CTdmeItemDlg;
			if(Key)
			{
				T_TDME_D TdmeD;
				if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdme(*((T_TDME_K*)Key),TdmeD))
				{
					pDlg->SetParamData(TdmeD);
					pDlg->DoModal();
					delete pDlg;
				}
			}
		}
		break;
	case D_CMD_MHTP_DEFINE_DLG     :
		{
			CTemperatureRisePropItemDlg* pDlg = new CTemperatureRisePropItemDlg;
			if(Key)
			{
				T_MHTP_D MhtpD;
				if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetMhtp(*((T_MHTP_K*)Key),MhtpD))
				{
					pDlg->SetParamData(MhtpD);
					pDlg->DoModal();
					delete pDlg;
				}
			}
		}
		break;
	case D_CMD_CCMSECTPAGE4DESIGN:
		{
			CCMSectPage4Design* pDlg = new CCMSectPage4Design;
			pDlg->DoModal();
			delete pDlg;
		}
		break;
	case D_CMD_CCMSECTPAGE4DESIGN_VIR:
		{
			CCMSectPage4Design* pDlg = new CCMSectPage4Design;
			pDlg->SetVirtual(TRUE);
			pDlg->DoModal();
			delete pDlg;
		}
		break;
	case D_CMD_CMACTIVEIDENTITY:
		{
			CMActiveIdentityDlg* pDlg = new CMActiveIdentityDlg;
			pDlg->DoModal();
			delete pDlg;
	}
		break;
	case D_CMD_TENDON_TYPE:
		{
			CCMTendonTypeDlg* pDlg = new CCMTendonTypeDlg;
			pDlg->DoModal();
			delete pDlg;
	}
		break;
	case D_CMD_TENDON_TYPE_ADD:
		{
			CCMTendonTypeAddDlg* pDlg = new CCMTendonTypeAddDlg;
			if(Key)
				pDlg->SetTDNTD(*((T_TDNT_K*)Key));
			pDlg->DoModal();
			delete pDlg;
	}
		break;
	case D_CMD_RPOS_LIST_DLG:
		{
			CCMRposListDlg* pDlg = new CCMRposListDlg;
			pDlg->DoModal();
			delete pDlg;
	}
		break;
	case D_CMD_ELSPLS_MATL_DLG:
		{
			CCMElsPlsMatlDlg* pDlg = new CCMElsPlsMatlDlg;
			pDlg->DoModal();
			delete pDlg;
			break;
		}
	case D_CMD_ELSPLS_MATL_ADD_DLG:
		{
			CCMElsPlsMatlAddDlg* pDlg = new CCMElsPlsMatlAddDlg;
			pDlg->DoModal();
			delete pDlg;
			break;
		}
	case D_CMD_ELDPLS_MICRO_PARAM_DLG:
		{
			CCMElsPlsParamDlg* pDlg = new CCMElsPlsParamDlg;
			pDlg->DoModal();
			delete pDlg;
			break;
		}
	case D_CMD_GRIDLINE_LOAD_CASES:
		{
			CCMGilcListDlg* pDlg = new CCMGilcListDlg;
			pDlg->SetInitPos(D_INIT_POS_LT);
			pDlg->DoModal();
			delete pDlg;
			break;
		}
	case D_CMD_BRIDGE_TYPE_CASES:
		{
			CCMBridgeTypeDlg* pDlg = new CCMBridgeTypeDlg;
			pDlg->DoModal();
			delete pDlg;
			break;
		}
	case  D_CMD_RESULT_LCRS_DLG:
		{
			CCMDLcrsDlg* pDlg = new CCMDLcrsDlg;
			pDlg->DoModal();
			delete pDlg;
			break;
		}
	case D_CMD_EXTERNAL_TYPE_DLG:
		{
			CCMExTypeForPretenDlg* pDlg = new CCMExTypeForPretenDlg;
			pDlg->DoModal();
			delete pDlg;
			break;
		}
	case D_CMD_CSCS_ITEM_DLG:
		{
			CCMStageCompSectItemDlg* pDlg = new CCMStageCompSectItemDlg;
			if (Key)
			{
				T_CSCS_K CscsK;
				T_CSCS_D CscsD;
				CscsK = *((T_CSCS_K*)Key);
				if (CDBDoc::GetDocPoint()->m_pAttrCtrl->GetCscs(CscsK, CscsD))
				{
					pDlg->SetParam(CscsK, CscsD);
					pDlg->DoModal();
					delete pDlg;
				}
			}
			break;
		}
	case D_CMD_PREB_ITEM_DLG:
		{
			CCMEBeamItemDlg* pDlg = new CCMEBeamItemDlg;
			if(Key!=NULL) 
			{
				T_PREB_K PrebK = *((T_PREB_K*)Key);
				pDlg->SetModifyData(PrebK);
			}
			pDlg->DoModal();
			delete pDlg;
			break;
		}
	case D_CMD_PRCS_ITEM_DLG:
		{
			CCMStripItemDlg* pDlg = new CCMStripItemDlg;
			if(Key!=NULL) 
			{
				T_PRCS_K PrcsK = *((T_PRCS_K*)Key);
				pDlg->SetModifyData(PrcsK);
			}      
			pDlg->DoModal();
			delete pDlg;
			break;
		}
	case D_CMD_DPAN_ITEM_DLG:
		{
			CCMDropPanelItemDlg* pDlg = new CCMDropPanelItemDlg;
			if(Key!=NULL) 
			{
				T_DPAN_K DpanK = *((T_DPAN_K*)Key);
				T_DPAN_D DpanD;
				CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetDpan(DpanK, DpanD);
				pDlg->m_bModify = TRUE;
				pDlg->SetData(DpanK, DpanD);
			}
			pDlg->DoModal();
			delete pDlg;
			break;
		}
	case D_CMD_CCPT_ITEM_DLG:
		{
			CCMColmCapitalItemDlg* pDlg = new CCMColmCapitalItemDlg;
			if(Key!=NULL)
			{
				T_CCPT_K CcptK = *((T_CCPT_K*)Key);
				T_CCPT_D CcptD;
				CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetCcpt(CcptK, CcptD);
				pDlg->m_bModify = TRUE;
				pDlg->SetData(CcptK, CcptD);
			}      
			pDlg->DoModal();
			delete pDlg;
			break;
		}
	case D_CMD_PSLT_ITEM_DLG:
		{
			CCMPsltDlg* pDlg = new CCMPsltDlg;
			if(Key!=NULL)
			{
				T_PSLT_K PsltK = *((T_PSLT_K*)Key);
				pDlg->SetStartPsltK(PsltK);
			} 
			pDlg->DoModal();
			delete pDlg;
	}
		break;
	case D_CMD_REINFORCE_ASSESSMENT_INFO_DLG:
	{
		CmdTotalAppraisalInformationDlg dlg;
		dlg.DoModal();
	}
	break;
	case D_CMD_REINFORCE_DEFINE_SECT_DLG:
	{
		CCmdReinforceSectDlg dlg(NULL, (T_RFST_K*)Key);
		dlg.DoModal();
	}
	break;
	case D_CMD_REINFORCE_CHECK_DESIGN_INFO_DLG:
	{
		CCmdReinforceDesignDlg dlg;
		dlg.DoModal();
	}
	break;
	default:
		TRACE(_LS(IDS_WG_CMD__ADDD__Error___Unregistered_dialog_));
		break;
	}
}

extern __declspec(dllexport) void CMD_DoModal1(int nWhich, void* Key/* = NULL*/)
{
	switch (nWhich)
	{
	case D_CMD_SDVI_ITEM_DLG:
	{
		CCMSdviDlg* pDlg = new CCMSdviDlg;
		if (Key != NULL)
		{
			T_SDVI_K SdviK = *((T_SDVI_K*)Key);
			T_SDVI_D SdviD;
			if (CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetSdvi(SdviK, SdviD)) pDlg->SetParamData(SdviK, SdviD);
		}
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_SDVE_ITEM_DLG:
	{
		CCMSdveDlg* pDlg = new CCMSdveDlg;
		if (Key != NULL)
		{
			T_SDVE_K SdveK = *((T_SDVE_K*)Key);
			T_SDVE_D SdveD;
			if (CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetSdve(SdveK, SdveD)) pDlg->SetParamData(SdveK, SdveD);
		}
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_SDST_ITEM_DLG:
	{
		CCMSdstDlg* pDlg = new CCMSdstDlg;
		if (Key != NULL)
		{
			T_SDST_K SdstK = *((T_SDST_K*)Key);
			T_SDST_D SdstD;
			if (CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetSdst(SdstK, SdstD)) pDlg->SetParamData(SdstK, SdstD);
		}
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_SDHY_ITEM_DLG:
	{
		CCMSdhyDlg* pDlg = new CCMSdhyDlg;
		if (Key != NULL)
		{
			T_SDHY_K SdhyK = *((T_SDHY_K*)Key);
			T_SDHY_D SdhyD;
			if (CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetSdhy(SdhyK, SdhyD)) pDlg->SetParamData(SdhyK, SdhyD);
		}
		pDlg->DoModal();
		delete pDlg;
	}
	case D_CMD_SDIS_ITEM_DLG:
	{
		CCMSdisDlg* pDlg = new CCMSdisDlg;
		if (Key != NULL)
		{
			T_SDIS_K SdisK = *((T_SDIS_K*)Key);
			T_SDIS_D SdisD;
			if (CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetSdis(SdisK, SdisD)) pDlg->SetParamData(SdisK, SdisD);
		}
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_ASGB_ITEM_DLG:
	{
		CAntivibrationSupportGBItemDlg* pDlg = new CAntivibrationSupportGBItemDlg;
		if (Key != NULL)
		{
			T_ASGB_K AsgbK = *((T_ASGB_K*)Key);
			T_ASGB_D AsgbD;
			if (CDBDoc::GetDocPoint()->m_pAttrCtrl->GetAsgb(AsgbK, AsgbD)) pDlg->SetParamData(AsgbK, AsgbD);
		}
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_DMGB_ITEM_DLG:
	{
		CDamperGBItemDlg* pDlg = new CDamperGBItemDlg;
		if (Key != NULL)
		{
			T_DMGB_K DmgbK = *((T_DMGB_K*)Key);
			T_DMGB_D DmgbD;
			if (CDBDoc::GetDocPoint()->m_pAttrCtrl->GetDmgb(DmgbK, DmgbD)) pDlg->SetParamData(DmgbK, DmgbD);
		}
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_RSMODFACT_DLG:
	{
		CRSModifyFactorDlg* pDlg = new CRSModifyFactorDlg;
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_GRIDANALCTRL_DLG:
	{
		CCMGridAnalCtrlDlg* pDlg = new CCMGridAnalCtrlDlg;
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_FLUE_GAS_TEMP_DLG:
	{
		CFlueListDlg* pDlg = new CFlueListDlg;
		pDlg->SetInitPos(D_INIT_POS_LT, 0, 50);
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_STEEL_TIME_TEMP_DLG:
	{
		CSteelTimeTempListDlg* pDlg = new CSteelTimeTempListDlg;
		pDlg->SetInitPos(D_INIT_POS_LT, 0, 50);
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_FLUE_GAS_TEMP_ITEM_DLG:
	{
		CFlueItemDlg* pDlg = new CFlueItemDlg(*((T_FLUE_K*)Key));
		pDlg->SetInitPos(D_INIT_POS_LT, 0, 50);
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_STEEL_TIME_TEMP_ITEM_DLG:
	{
		CSteelTimeTempItemDlg* pDlg = new CSteelTimeTempItemDlg(*((T_STTF_K*)Key));
		pDlg->SetInitPos(D_INIT_POS_LT, 0, 50);
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_TENDON_CHANGE_PROF_DLG:
	{
		CCMTendonChangeProfDlg* pDlg = new CCMTendonChangeProfDlg;
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_DAMP_CODE_CHECK_INFO_DLG:
	{
		CCmdDampCheckInfoDlg* pDlg = new CCmdDampCheckInfoDlg;
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_DAMP_CODE_CHECK_PARAM_DLG:
	{
		CCmdDampingGBCoefCalcDlg dlg;
		dlg.DoModal();
	}
	break;
	case D_CMD_PT_SET_TENDON_PROFILE_DLG:
		{
			CCMPostTensionTendonProfileDlg dlg;
			dlg.DoModal();
		}
		break;
	case D_CMD_PT_SET_TENDON_STRESSING_DLG:
		{
			CCMPostTensionTendonStressingDlg dlg;
			dlg.DoModal();
		}
		break;
	case D_CMD_PT_SET_TENDON_PROFILE_MATCH_DLG:
		{
			T_PTTN_K PttnK = *((T_PTTN_K*)Key);

			CPostTensionModifyTendonDlg dlg;
			dlg.SetPttnK(PttnK);
 			dlg.DoModal();
		}
		break;
	default:
		TRACE(_LS(IDS_WG_CMD__ADDD__Error___Unregistered_dialog_));
		break;
	}
}

extern __declspec(dllexport) void CMD_DoModal_Stage(int nWhich, void* Key /*= NULL*/)
{
	switch (nWhich)
	{
	case D_CMD_CONSTRUCTION_STAGE:
	{
		CCMStageConsDlg* pDlg = new CCMStageConsDlg;
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_CONSTRUCTION_STAGE_HYD:
	{
		CCMHydStageConsDlg* pDlg = new CCMHydStageConsDlg;
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_STAGE_DEFINE:
	{
		CCMStageDefMainDlg* pDlg = new CCMStageDefMainDlg;
		if (Key)
			pDlg->SetCurrentStage(*((T_STAG_K*)Key), 0);
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	case D_CMD_STAGE_CHANGE:
	{
		CCMStageChangeDlg* pDlg = new CCMStageChangeDlg;
		pDlg->DoModal();
		delete pDlg;
	}
	break;
	default:
		TRACE(_LS(IDS_WG_CMD__ADDD__Error___Unregistered_dialog_));
		break;
	}
}

extern __declspec(dllexport) void CMD_DoModal_CreepShrinkage(void* Key /*= NULL*/)
{
	CCMHcrpDlg* pDlg = new CCMHcrpDlg;
	if (Key)
	{
		T_TDMT_D TdmtD;
		if (CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdmt(*((T_TDMT_K*)Key), TdmtD))
		{
			pDlg->SetTimeDepMatMode(TRUE, 1);
			pDlg->SetCurTDMTData(&TdmtD);
			pDlg->DoModal();
		}
	}
	delete pDlg;
}

typedef int  (fnDoModal)(LPCTSTR, void*);
typedef void (fnLinkResource)();
int DLG_DoModal(LPCTSTR lpszDllName, LPCTSTR lpszDlgClassName, void* pExtraData)
{
	HINSTANCE hDLL;
	hDLL = ::LoadLibrary(lpszDllName);
	if (!hDLL) return -1;
	fnLinkResource* pfuncLinkResource = 
		(fnLinkResource*)::GetProcAddress(hDLL, "DynamicLinkResourceInRegularDLL");
	pfuncLinkResource();

	fnDoModal* pfuncDoModal = (fnDoModal*)::GetProcAddress(hDLL, "DoModal");
	if (!pfuncDoModal) 
	{
		::FreeLibrary(hDLL);
		return -1;
	}
	int nResult = pfuncDoModal(lpszDlgClassName, pExtraData);
	::FreeLibrary(hDLL);
	return nResult;
}

extern __declspec(dllexport) 
int CMD_DoModal(LPCTSTR lpszDlgClassName, void* pExtraData)
{
	CString csDllName, csPostfix;
#if defined(_DEBUG)
	csPostfix = _T("d");
#endif
	
	CString csClassName = lpszDlgClassName;

	if (
			csClassName == _T("CCMEtcBoltDlg")        ||
			csClassName == _T("CCmEtcDVMTDlg")        ||
			csClassName == _T("CCmEtcMTBGDlg")        ||
			csClassName == _T("CCMTowerLoadMainDlg")
		 )
		 csDllName = _T("wg_dlg_tower");
	else { ASSERT(0); return -1; }

	return DLG_DoModal(csDllName+csPostfix+_T(".dll"), lpszDlgClassName, pExtraData);
}


extern __declspec(dllexport) 
 void* CreateOrActivatePropty(CDBDoc* pDoc, int nActivePage)
{
		if(pDoc == nullptr)
				pDoc = CDBDoc::GetDocPoint();
		CCMPropertiesDlg *pPropDlg = (CCMPropertiesDlg*)CreateOrActivateDlg(pDoc, CCMPropertiesDlg::IDD);
		pPropDlg->SetActivePage(nActivePage);	
		return pPropDlg;
}