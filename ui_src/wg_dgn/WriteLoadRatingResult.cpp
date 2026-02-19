// WriteLoadRatingResult.cpp: implementation of the CWriteLoadRatingResult class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "WriteLoadRatingResult.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"

#include "..\wg_db\DB_ST_DN_RESULT_PSC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWriteLoadRatingResult::CWriteLoadRatingResult()
{
	m_ElemKList.RemoveAll();

	//m_iDgnCode = m_ForceCtrl.Get_Code();

	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
}

CWriteLoadRatingResult::~CWriteLoadRatingResult()
{

}

BOOL CWriteLoadRatingResult::Execute_BridgeLoadRaitng()
{
	if (CDBDoc::GetDocPoint()->IsLimitedTrialVersion())
	{
		//AfxMessageBox(_LS(IDS_TRIAL_NO_LICENSE));
		CDBDoc::GetDocPoint()->ShowLimitedTrialVersionErrMsg();
		return FALSE;
	}

	int iStageNum = m_pDoc->m_pAttrCtrl->GetCountStag();

	T_STAG_K CurStag = m_pDoc->m_pStagCtrl->GetCurStag();
	int iCurStep = m_pDoc->m_pStagCtrl->GetCurStagStepIndex()+1;
		
	if(iStageNum>0)
		m_pDoc->m_pStagCtrl->SetFinalStagWithoutUpdateNotify();
	
	if(!MakeRatingDesignData()) 
	{
		if(iStageNum>0)
			m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);
		return FALSE;
	}

	int nCodeNum = m_DataCtrl.Get_Code();
	
	switch(nCodeNum)
	{
		case AASHTO_LRFD05_PSC_RATING:
		case AASHTO_LRFD12_PSC_RATING:
			{
                if ( !m_DataCtrl.ExecutePSCRatingAASHTO() ) return FALSE;
                break;
            }
        case AASHTO_LRFD19_PSC_RATING:
            {
                if ( !m_DataCtrl.ExecutePSCRatingAASHTO19() ) return FALSE;
                break;
            }
		case KSCE_USD05_RATING:
		case KSCE_USD10_RATING:
		case KSCE_RAIL_USD11_RATING:
		case KSCE_LSD15_RATING:
			{
				if(!m_DataCtrl.ExecuteRatingKS05(nCodeNum))	return FALSE;	  
				break;
			}
		case STL_KSCE_ASD05:
		case STL_KSCE_ASD10:
		case STL_KSCE_RAIL_ASD04:
		case STL_KSCE_RAIL_ASD11:
		case STL_AASHTO_LRFD12:
        case STL_AASHTO_LRFD19:
		case STL_KSCE_LSD15:
        case STL_CS454_20:
		case STL_NR_GN_CIV_025_06:
			{
				if(!m_DataCtrl.ExecuteSTLRating(nCodeNum))	return FALSE;	  
				break;
			}
		case KSCE_USD10_RC_RATING:
		case KSCE_RAIL_USD11_RC_RATING:
		case KSCE_LSD15_RC_RATING:
			{
				// RC rating 검토 
		BOOL bRCBeamRating = FALSE, bRCPlateRating = FALSE;
		if(m_DataCtrl.ExecuteRCRating(nCodeNum))	{bRCBeamRating = TRUE;}
		if(m_DataCtrl.ExecuteRCPlateRating(nCodeNum))	{bRCPlateRating = TRUE;}
		return (bRCBeamRating || bRCPlateRating);
				break;
			}
        case CS_454_20_PSC_RATING:
            {
                if(!m_DataCtrl.ExecuteBD21PSCBridgeAssessment()) return FALSE;
                break;
            }
		default:
		 {
			 ASSERT(0);
		 }
	}

	if(iStageNum>0)
		m_pDoc->m_pStagCtrl->ChangeCurStagStepWithoutUpdateNotify(CurStag, iCurStep);

	return TRUE;
}

BOOL CWriteLoadRatingResult::MakeRatingDesignData()
{  
	m_DataCtrl.InitialBridgeLoadRating();   // 하중조합 초기세팅

	int nCode = m_DataCtrl.Get_Code();
		
	switch(nCode)
	{
	case AASHTO_LRFD05_PSC_RATING:
	case AASHTO_LRFD12_PSC_RATING:
    case AASHTO_LRFD19_PSC_RATING:
		{
			if(!m_DataCtrl.Cal_RatingCaseDataForAASHTO())	return FALSE;
			if(!m_DataCtrl.Cal_TdgrDataForDesignAll())	return FALSE;
			break;
		}
	case KSCE_USD05_RATING:
	case KSCE_USD10_RATING:
	case KSCE_RAIL_USD11_RATING:
	case KSCE_LSD15_RATING:
	case STL_KSCE_ASD05:
	case STL_KSCE_ASD10:
	case STL_KSCE_RAIL_ASD04:
	case STL_KSCE_RAIL_ASD11:
	case STL_KSCE_LSD15:
		{
			if(!m_DataCtrl.Cal_RatingCaseDataForKSCE05(nCode))	return FALSE;	  
			break;
		}
	case STL_AASHTO_LRFD12:
    case STL_AASHTO_LRFD19:
		{
			if(!m_DataCtrl.Cal_RatingCaseStlDataForAASHTO())	return FALSE;
			break;
		}
	case KSCE_USD10_RC_RATING:
	case KSCE_RAIL_USD11_RC_RATING:
	case KSCE_LSD15_RC_RATING:
		{
			BOOL bCheck_beam = TRUE;
			if(!m_DataCtrl.Cal_RatingCaseDataForKSCE05(nCode))	bCheck_beam=FALSE;	  

			BOOL bCheck_plate = TRUE;
			if(!m_DataCtrl.Cal_RatingCaseDataForKSCE_Plate(nCode))	bCheck_plate=FALSE;	  

			if(!bCheck_beam && !bCheck_plate) return FALSE;

			break;
		}
    case CS_454_20_PSC_RATING:
        {
            if(!m_DataCtrl.Cal_RatingCaseDataForBD21())	return FALSE;	  
            break;
        }
    case STL_CS454_20:
        {
            if ( !m_DataCtrl.Cal_RatingCaseDataForBD21() )	return FALSE;
            break;
        }
	case STL_NR_GN_CIV_025_06:
		{
			if (!m_DataCtrl.Cal_RatingCaseDataForBD21())	return FALSE;
			break;
	    }
    default:
		ASSERT(0);
	}


	// Get ElemList for Design.
	m_ElemKList.RemoveAll();
	m_MembKList.RemoveAll();
	switch(nCode)
	{
	case STL_KSCE_ASD05:
	case STL_KSCE_ASD10:
	case STL_KSCE_RAIL_ASD04:
	case STL_KSCE_RAIL_ASD11:
	case STL_AASHTO_LRFD12:
    case STL_AASHTO_LRFD19:
	case STL_KSCE_LSD15:
    case STL_CS454_20:
	case STL_NR_GN_CIV_025_06:
		{
			if(!m_DataCtrl.Get_ElemListForBlrDgn_STL(FALSE, m_ElemKList)) return FALSE;
			break;
		}
	case KSCE_USD10_RC_RATING:
	case KSCE_RAIL_USD11_RC_RATING:
	case KSCE_LSD15_RC_RATING:
		{
			BOOL bCheck_beam = TRUE;
			CArray<T_ELEM_K, T_ELEM_K> aElemKList;
			if(!m_DataCtrl.Get_ElemListForBlrDgn_RC(FALSE,aElemKList))	bCheck_beam=FALSE;
			m_DataCtrl.ConvertElemKeyToElemPairKey(aElemKList, m_ElemKList);

			BOOL bCheck_plate = TRUE;
			if(!m_DataCtrl.Get_ElemListForBlrDgn_RC_Plate(FALSE, m_MembKList))	bCheck_plate=FALSE;	  

			if(!bCheck_beam && !bCheck_plate) return FALSE;
			break;
		}
	default:
		{
			if(!m_DataCtrl.Get_ElemListForBlrDgn(FALSE,m_ElemKList)) return FALSE;
			break;
		}
	}

	return TRUE;
}