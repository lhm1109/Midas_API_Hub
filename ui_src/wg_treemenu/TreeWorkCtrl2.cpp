// TreeWorkCtrl.cpp : implementation file
// 
// 추가하려면...
// 0. TreeWorkDefine.h 파일에서 순서에 맞게 ItemID를 Define 해준다. (순서중요)
// 1. SetXX XXX 함수를 추가하여 Item을 추가해준다. (Bitmap 추가)
// 2. UpdateBuffer() 함수에서 데이터 Update에 따른 Item 갱신이 이루어질 수 있도록 코딩.
// 3. OnContextMenu()에서 Context Menu목록을 내용에 맞게 추가해준다.
// 4. Context Menu 목록별로 하나씩 각 Message처리를 해준다.
// 
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreeWorkCtrl.h"
#include "TreeMenuCtrl.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\InternationalUtil.h"
#include "..\wg_base\XMultiLang.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\wg_db_StoryData.h"   // for CStoryData
#include "..\wg_dbLock\LockMgr.h"

#include "..\wg_cmd\wg_cmd_Dialog.h"
#include "..\wg_cmd\CMMvhlItemStdEuroBSDlg.h"
#include "..\wg_cmd\CMMvldItemTrDlg.h"
#include "..\wg_cmd\CMMvldItemEuroBSDlg.h"
#include "..\wg_cmd\CMMvldItemEuroBSNewDlg.h"

//#include "..\wg_cmd2\wg_cmd2_Dialog.h"

#include "..\wg_base\I_GENModelBase.h"


#include <map>
#include "..\wg_db\MITC_CommonTool.h"
#include "..\wg_gr\ILabelMgr_CH.h"

const int BMP_START_FILE1 = 0;
const int BMP_START_FILE2 = 64;
const int BMP_START_FILE3 = 128;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int CTreeWorkCtrl::GetSelectedFipaKeyListWithMultiTreeItemSelect(CArray<T_FIPA_K, T_FIPA_K> &aKey)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}

	CArray<HTREEITEM, HTREEITEM> aItem;
	int selItemNum = GetSelectedItemList(aItem);
	if(selItemNum==0) return FALSE;

	HTREEITEM prvHt,ht,nxtHt;
	int pos = 0,nSt=0;
	ht = aItem[0];

	T_FIPA_D FipaD;
	CArray<T_FIPA_K, T_FIPA_K> aFipaK;
	pDoc->m_pAttrCtrl->GetFipaKeyList(aFipaK);

	for(prvHt = ht;;) { prvHt = GetPrevSiblingItem(ht); if(prvHt==NULL) break; ht = prvHt; } 
	for(nxtHt = ht ; nxtHt!=NULL ; ht=nxtHt, pos++)
	{
		for(int i = 0;i < 4;i++)
		{
			if(pos >= 4)
			{
				ASSERT(FALSE);
				return 0;
			}
			if(m_aFipaItemKey[pos].IsEmpty())
				pos++;
			else
				break;
		}
		for (int i=nSt ; i<aItem.GetSize(); i++)
		{
			if (ht == aItem[i]) 
			{ 
				for(int j = 0;j < aFipaK.GetSize();j++)
				{
					if(pDoc->m_pAttrCtrl->GetFipa(aFipaK[j],FipaD) && FipaD.firetype == pos)
					{
						aKey.Add(aFipaK[j]);
					}
				}
				nSt+=1; 
				break;
			}
		}
		nxtHt = GetNextSiblingItem(ht);
	}
	return aKey.GetSize();
}

void CTreeWorkCtrl::DeleteItemEntity(CArray<HTREEITEM, HTREEITEM> &aItem)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return;
	}

	HTREEITEM parentItem = GetParentItem(aItem[0]);
	UINT parentData = GetItemData(parentItem);
	// 하중을 지울 때
	if (parentData == WORK_STLD_LST) 
	{ 
		if (!DeleteStaticLoad(pDoc,aItem))
		{
			// Delete이 수행되지 못했을 경우 Caret 상태 해제 
			HTREEITEM hCaret = GetSelectedItem();
			if (hCaret)
			{
				EnableMultiSelect(FALSE);
				SelectItem(hCaret);
				EnableMultiSelect(TRUE);
			}
		}
		return; 
	}
#ifdef _MGEN_CH
	if (parentData == WORK_CNLD_TYPE_LST) 
	{ 
		if (!DeleteCnldType(pDoc,aItem))
		{
			ASSERT(FALSE);
		}
		return; 
	}
#endif // _MGEN_CH
	HTREEITEM ht,prvHt;
	int pos = 0;
	ht = GetSelectedItem();
	for(prvHt = ht;prvHt!=NULL;ht = prvHt,pos++) prvHt = GetPrevSiblingItem(ht);

	UINT data = GetItemData(aItem[0]);
	BOOL bSuccess=FALSE;

	if (data == WORK_NODE_FLD) // Node로 지울 때
	{
		CArray<T_NODE_K, T_NODE_K> aNodeKeyList;
		pDoc->m_pAttrCtrl->GetNodeKeyList(aNodeKeyList);
		bSuccess = pDoc->m_pDataCtrl->DelNode(aNodeKeyList);
		aNodeKeyList.RemoveAll();
	}
	else if ((data>=WORK_TRUSS_LST && data<=WORK_WALL_LST) ||
					 data==WORK_WALLOPEN_LST) // Element로 지울 때
	{
		CArray<T_ELEM_K, T_ELEM_K> aSelElemKey;
		GetSelectedElemKeyListWithMultiTreeItemSelect(aSelElemKey);
		bSuccess = pDoc->m_pDataCtrl->DelElem(aSelElemKey);
		aSelElemKey.RemoveAll();
	}
	else if (data == WORK_SPAN_LST || data == WORK_SPAN_COMPOSTIE_LST || data == WORK_SPAN_STEELGIRDER_LST || data == WORK_SPAN_COMPOSTIE_VB_LST)  // Span Information을 지울 때
	{
		BOOL bVirtual = (data == WORK_SPAN_COMPOSTIE_VB_LST) ? TRUE : FALSE;

		CArray<T_SPAN_K, T_SPAN_K> aSelSpanKey;
		CArray<CString,CString&> aSpanName;
		CArray<int,int> aSectType;
		GetSelectedSpanKeyListWithMultiTreeItemSelect(aSelSpanKey,aSpanName,aSectType, bVirtual);

		if(bVirtual)
			bSuccess = pDoc->m_pDataCtrl->DelSpav(aSpanName,aSectType);
		else
			bSuccess = pDoc->m_pDataCtrl->DelSpan(aSpanName, aSectType);

		aSelSpanKey.RemoveAll();
	}
	else if (data == WORK_EFWD_LST) // PSC Value Flange Width
	{
		CArray<T_EFWD_K, T_EFWD_K> aSelEfwdKey;
		GetSelectedEfwdKeyListWithMultiTreeItemSelect(aSelEfwdKey);
		bSuccess = pDoc->m_pDataCtrl->DelEfwd(aSelEfwdKey);
		aSelEfwdKey.RemoveAll();
	}
	else if (data == WORK_GRUP_LST) // Group을 지울 때
	{
		CArray<T_GRUP_K, T_GRUP_K> aSelGrupKey;
		GetSelectedGrupKeyListWithMultiTreeItemSelect(aSelGrupKey);
		bSuccess = pDoc->m_pDataCtrl->DelGrup(aSelGrupKey);
		aSelGrupKey.RemoveAll();
	}
	else if (data == WORK_NPLN_LST) // Named Plane을 지울 때 
	{
		CArray<T_NPLN_K, T_NPLN_K> aSelNplnKey;
		GetSelectedNplnKeyListWithMultiTreeItemSelect(aSelNplnKey);

		CArray<CString, CString&> aNplnName;
		T_NPLN_D data;
		for (int i = 0; i < aSelNplnKey.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetNpln(aSelNplnKey[i], data);
			aNplnName.Add(data.PlaneName);
		}
		bSuccess  =pDoc->m_pDataCtrl->DelNpln(aNplnName);
		aNplnName.RemoveAll();
	}
	else if (data == WORK_MATL_LST) // Material을 지울 때 
	{
		CArray<T_MATL_K, T_MATL_K> aSelMatlKey;
		GetSelectedMatlKeyListWithMultiTreeItemSelect(aSelMatlKey);
		bSuccess = pDoc->m_pDataCtrl->DelMatl(aSelMatlKey);
		aSelMatlKey.RemoveAll();
	}
	else if(data == WORK_TDMT_LST) // Time Dependent Material(C&S)을 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedTdmtNameListWithMultiTreeItemSelect(aName);
		pDoc->m_pDataCtrl->DelTdmt(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_TDME_LST) // Time Dependent Material(Elasticity)을 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedTdmeNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelTdme(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_TDMF_LST) // Time Dependent Material Function을 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedTdmfNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelTdmf(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_TMAT_LST) // Time Dependent Material Link를 지울 때 
	{
		CArray<T_TMAT_K, T_TMAT_K> aSelTmatKey;
		GetSelectedTmatKeyListWithMultiTreeItemSelect(aSelTmatKey);
		bSuccess = pDoc->m_pDataCtrl->DelTmat(aSelTmatKey);
		aSelTmatKey.RemoveAll();
	}
	else if (data == WORK_MHTP_LST) // High Temperature Material
	{
		CArray<CString, CString&> aName;
		GetSelecteMhtpNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelMhtp(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_MHAT_LST) // High Temperature Material Link
	{
		CArray<T_MHAT_K, T_MHAT_K> aSelMhatKey;
		GetSelectedMhatKeyListWithMultiTreeItemSelect(aSelMhatKey);
		bSuccess = pDoc->m_pDataCtrl->DelMhat(aSelMhatKey);
		aSelMhatKey.RemoveAll();
	}
	else if (data == WORK_EPMT_LST)  // Plastic Material을 지울 때 
	{
		CArray<CString, CString&> aSelEpmtName;
		GetSelectedEpmtNameListWithMultiTreeItemSelect(aSelEpmtName);
		bSuccess = pDoc->m_pDataCtrl->DelEpmt(aSelEpmtName);
		aSelEpmtName.RemoveAll();
	}
	else if (data == WORK_TSGR_LST) // Tapered Section Group 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedTsgrNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelTsgr(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_SECT_LST) // Section을 지울 때 
	{
		CArray<T_SECT_K, T_SECT_K> aSelSectKey;
		GetSelectedSectKeyListWithMultiTreeItemSelect(aSelSectKey);
		bSuccess = pDoc->m_pDataCtrl->DelSect(aSelSectKey);
		aSelSectKey.RemoveAll();
	}
	else if (data == WORK_CSCS_LST) // Compiste Section for Construction Stage를 지울 때 
	{
		CArray<T_CSCS_K, T_CSCS_K> aSelCscsKey;
		GetSelectedCscsKeyListWithMultiTreeItemSelect(aSelCscsKey);
		bSuccess = pDoc->m_pDataCtrl->DelCscs(aSelCscsKey);
		aSelCscsKey.RemoveAll();
	}
	else if (data == WORK_SECF_LST) // Section Stiffness Scale Factor를 지울 때 
	{
		CArray<T_SECF_K, T_SECF_K> aSelSecfKey;
		GetSelectedSecfKeyListWithMultiTreeItemSelect(aSelSecfKey); 
		bSuccess = pDoc->m_pDataCtrl->DelSecf(aSelSecfKey);
		aSelSecfKey.RemoveAll();
	}
	else if (data == WORK_RPSC_LST) // reinforcement for PSC section
	{
		CArray<T_RPSC_K, T_RPSC_K> aSelRpscKey;
		GetSelectedRpscKeyListWithMultiTreeItemSelect(aSelRpscKey);
		bSuccess = pDoc->m_pDataCtrl->DelRpsc(aSelRpscKey);
		aSelRpscKey.RemoveAll();
	}
	else if (data == WORK_THIK_LST) // Thickness을 지울 때 
	{
		CArray<T_THIK_K, T_THIK_K> aSelThikKey;
		GetSelectedThikKeyListWithMultiTreeItemSelect(aSelThikKey);
		bSuccess = pDoc->m_pDataCtrl->DelThik(aSelThikKey);
		aSelThikKey.RemoveAll();
	}
	else if (data == WORK_MPGB_FLD)   // M-Ø Global Data을 지울 때
	{
		bSuccess = pDoc->m_pDataCtrl->DelMpgb();
	}
	else if (data == WORK_MPHG_LST)   // M-Ø Hinge을 지울 때
	{
		CArray<T_MPHG_K, T_MPHG_K> aSelMphgKey;
		GetSelectedMphgKeyListWithMultiTreeItemSelect(aSelMphgKey);
		bSuccess = pDoc->m_pDataCtrl->DelMphg(aSelMphgKey);
		aSelMphgKey.RemoveAll();
	}
	else if (data == WORK_MPST_LST || data == WORK_ESSF_LST)   // M-Ø Section을 지울 때
	{
		if (data == WORK_MPST_LST)
		{
			CArray<T_MPST_K, T_MPST_K> aSelMpstKey;
			GetSelectedMpstKeyListWithMultiTreeItemSelect(aSelMpstKey);
			bSuccess = pDoc->m_pDataCtrl->DelMpst(aSelMpstKey);
			aSelMpstKey.RemoveAll();
		}
		else
		{
			// element Stiffness Scale Factor를 지울 때
			CArray<T_ESSF_K, T_ESSF_K> aSelEssfKey;
			GetSelectedEssfKeyListWithMultiTreeItemSelect(aSelEssfKey);
			bSuccess = pDoc->m_pDataCtrl->DelEssf(aSelEssfKey);
			aSelEssfKey.RemoveAll();
		}
	}
	else if (data == WORK_WSSF_LST) // Wall Shear Stiffness Reduction Factor를 지울 때
	{
		CArray<T_WSSF_K, T_WSSF_K> aSelWssfKey;
		GetSelectedWssfKeyListWithMultiTreeItemSelect(aSelWssfKey);
		bSuccess = pDoc->m_pDataCtrl->DelWssf(aSelWssfKey);
		aSelWssfKey.RemoveAll();
	}
	else if (data == WORK_PSSF_LST) // Wall Shear Stiffness Reduction Factor를 지울 때
	{
		CArray<T_PSSF_K, T_PSSF_K> aSelPssfKey;
		GetSelectedPssfKeyListWithMultiTreeItemSelect(aSelPssfKey);
		bSuccess = pDoc->m_pDataCtrl->DelPssf(aSelPssfKey);
		aSelPssfKey.RemoveAll();
	}
	else if (data == WORK_CONS_LST) // Support를 지울 때 
	{
		CArray<T_CONS_K, T_CONS_K> aSelConsKey;
		GetSelectedConsKeyListWithMultiTreeItemSelect(aSelConsKey);
		bSuccess = pDoc->m_pDataCtrl->DelCons(aSelConsKey);
		aSelConsKey.RemoveAll();
	}
	else if (data == WORK_NSPR_LIN_LST) // Point Spring Support를 지울 때 
	{
		CArray<T_NSPR_K, T_NSPR_K> aSelNsprKey;
		GetSelectedNsprLinearKeyListWithMultiTreeItemSelect(aSelNsprKey);
		bSuccess = pDoc->m_pDataCtrl->DelNspr(aSelNsprKey);
		aSelNsprKey.RemoveAll();
	}
	else if (data == WORK_NSPR_NON_LST) // Point Spring Support를 지울 때 
	{
		CArray<T_NSPR_K, T_NSPR_K> aSelNsprKey;
		GetSelectedNsprNonKeyListWithMultiTreeItemSelect(aSelNsprKey);
		bSuccess = pDoc->m_pDataCtrl->DelNspr(aSelNsprKey);
		aSelNsprKey.RemoveAll();
	}
	else if (data == WORK_NSPR_MLT_LST)
	{
		CArray<T_NSPR_K, T_NSPR_K> aSelNsprKey;
		GetSelectedNsprMultiKeyListWithMultiTreeItemSelect(aSelNsprKey);
		bSuccess = pDoc->m_pDataCtrl->DelNspr(aSelNsprKey);
		aSelNsprKey.RemoveAll();
		}
	else if (data == WORK_GSTP_LST) // General Spring Support를 지울 때 
	{
		// General Spring Support Type 지우기 
		CArray<T_GSTP_K, T_GSTP_K> aGstpKeyList;  // General spring Support Type
		GetSelectedGstpKeyListWithMultiTreeItemSelect(aGstpKeyList);

		CArray<CString, CString&> aGstpName;
		T_GSTP_D data;
		for (int i = 0; i < aGstpKeyList.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetGstp(aGstpKeyList[i], data);
			aGstpName.Add(data.TypeName);
		}
		bSuccess = pDoc->m_pDataCtrl->DelGstp(aGstpName);
		aGstpKeyList.RemoveAll();
	}

	else if (data == WORK_SSPS_LIN_LST) // Surface Spring Support (Linear) 를 지울 때 
	{
		CArray<T_SSPS_K, T_SSPS_K> aSelSspsKey;
		GetSelectedSspsLinearKeyListWithMultiTreeItemSelect(aSelSspsKey);
		bSuccess = pDoc->m_pDataCtrl->DelSsps(aSelSspsKey);
		aSelSspsKey.RemoveAll();
	}
	else if (data == WORK_SSPS_NON_LST) // Surface Spring Support (Comp/Tens) 를 지울 때 
	{
		CArray<T_SSPS_K, T_SSPS_K> aSelSspsKey;
		GetSelectedSspsNonKeyListWithMultiTreeItemSelect(aSelSspsKey);
		bSuccess = pDoc->m_pDataCtrl->DelSsps(aSelSspsKey);
		aSelSspsKey.RemoveAll();
	}

	else if (data == WORK_NLLP_LST)  
	{
		CArray<CString, CString&> aSelNllpName;
		GetSelectedNllpNameListWithMultiTreeItemSelect(aSelNllpName);
		bSuccess = pDoc->m_pDataCtrl->DelNllp(aSelNllpName);
		aSelNllpName.RemoveAll();
	}
	else if (data == WORK_NLNK_LST)
	{
		//	BOOL DelNlnk2(CArray<T_NLNK_K, T_NLNK_K>& rKey);
		CArray<T_NLNK_K, T_NLNK_K> aSelNlnkKey;
		GetSelectedNlnkKeyListWithMultiTreeItemSelect(aSelNlnkKey);
		bSuccess = pDoc->m_pDataCtrl->DelNlnk2(aSelNlnkKey);
		aSelNlnkKey.RemoveAll();
	}
	else if (data == WORK_IEHP_LST)
	{    
		CArray<T_IEHP_K, T_IEHP_K> aSelIehpKey;
		GetSelectedIehpKeyListWithMultiTreeItemSelect(aSelIehpKey);
		bSuccess = pDoc->m_pDataCtrl->DelIehp(aSelIehpKey);
		aSelIehpKey.RemoveAll();
	}
	else if (data == WORK_IEHG_LST)
	{
		CArray<T_IEHG_K, T_IEHG_K> aSelIehgKey;
		GetSelectedIehgKeyListWithMultiTreeItemSelect(aSelIehgKey);
		bSuccess = pDoc->m_pDataCtrl->DelIehg(aSelIehgKey);
		aSelIehgKey.RemoveAll();
	}
	else if (data == WORK_FRLS_LST) // Beam End Release를 지울 때 
	{
		CArray<T_FRLS_K, T_FRLS_K> aSelFrlsKey;
		GetSelectedFrlsKeyListWithMultiTreeItemSelect(aSelFrlsKey);
		bSuccess = pDoc->m_pDataCtrl->DelFrls(aSelFrlsKey);
		aSelFrlsKey.RemoveAll();
	}
	else if (data == WORK_OFFS_LST) // Beam End Offset를 지울 때 
	{
		CArray<T_OFFS_K, T_OFFS_K> aSelOffsKey;
		GetSelectedOffsKeyListWithMultiTreeItemSelect(aSelOffsKey);
		bSuccess = pDoc->m_pDataCtrl->DelOffs(aSelOffsKey);
		aSelOffsKey.RemoveAll();
	}
	else if (data == WORK_PRLS_LST) // Plate End Release를 지울 때 
	{
		CArray<T_PRLS_K, T_PRLS_K> aSelPrlsKey;
		GetSelectedPrlsKeyListWithMultiTreeItemSelect(aSelPrlsKey);
		bSuccess = pDoc->m_pDataCtrl->DelPrls(aSelPrlsKey);
		aSelPrlsKey.RemoveAll();
	}
	else if (data == WORK_DRLS_FLD) // Plate End Release를 지울 때 
	{
	  CArray<T_DRLS_K, T_DRLS_K> aDrlsKeyList;  
	  pDoc->m_pAttrCtrl->GetDrlsKeyList(aDrlsKeyList);
	  bSuccess = pDoc->m_pDataCtrl->DelDrls(aDrlsKeyList);
	}
	else if (data == WORK_RIGD_LST) // Rigid Link를 지울 때 
	{
		CArray<T_RIGD_K, T_RIGD_K> aSelRigdList;
		GetSelectedRigdKeyListWithMultiTreeItemSelect(aSelRigdList);
		bSuccess = pDoc->m_pDataCtrl->DelRigd(aSelRigdList);
		aSelRigdList.RemoveAll();
	}
	else if (data == WORK_MCON_LST) // Linear Constraints를 지울 때 
	{
		CArray<T_MCON_K, T_MCON_K> aSelMconList;
		GetSelectedMconKeyListWithMultiTreeItemSelect(aSelMconList);
		bSuccess = pDoc->m_pDataCtrl->DelMcon(aSelMconList);
		aSelMconList.RemoveAll();
	}
	else if (data == WORK_SKEW_LST) // Node Local Axis를 지울 때 
	{
		CArray<T_SKEW_K, T_SKEW_K> aSelSkewList;
		GetSelectedSkewKeyListWithMultiTreeItemSelect(aSelSkewList);
		bSuccess = pDoc->m_pDataCtrl->DelSkew(aSelSkewList);
		aSelSkewList.RemoveAll();
	}
	else if (data == WORK_CLDR_LST) // Define Constraint Label Direction
	{
		CArray<T_CLDR_K, T_CLDR_K> aSelCldrList;
		GetSelectedCldrKeyListWithMultiTreeItemSelect(aSelCldrList);
		bSuccess = pDoc->m_pDataCtrl->DelCldr(aSelCldrList);
		aSelCldrList.RemoveAll();
	}
	else if (data == WORK_EWSF_FLD)
	{
		CArray<T_EWSF_K, T_EWSF_K> aEwsfKeyList;
		pDoc->m_pAttrCtrl->GetEwsfKeyList(aEwsfKeyList);
		bSuccess = pDoc->m_pDataCtrl->DelEwsf(aEwsfKeyList);
	}
	else if (data == WORK_CGLP_LST)
	{
		CArray<T_CGLP_K, T_CGLP_K> aSelCglpList;
		GetSelectedCglpKeyListWithMultiTreeItemSelect(aSelCglpList);
		bSuccess = pDoc->m_pDataCtrl->DelCglp(aSelCglpList);
		aSelCglpList.RemoveAll();
	}
	else if (data == WORK_NMAS_LST) // Nodal Mass를 지울 때 
	{
		CArray<T_NMAS_K, T_NMAS_K> aSelNmasList;
		GetSelectedNmasKeyListWithMultiTreeItemSelect(aSelNmasList);
		bSuccess = pDoc->m_pDataCtrl->DelNmas(aSelNmasList);
		aSelNmasList.RemoveAll();
	}
	else if (data == WORK_MLSP_FLD) //  Line Lane Support를 지울 때 
	{
		CArray<T_MLSP_K, T_MLSP_K> aMlspKeyList;  
		pDoc->m_pAttrCtrl->GetMlspKeyList(aMlspKeyList);
		bSuccess = pDoc->m_pDataCtrl->DelMlsp(aMlspKeyList);
	}
	else if (data == WORK_MLTT_FLD) //  Lane Support-Two Trucks를 지울 때
	{
		CArray<T_MLTT_K, T_MLTT_K> aMlttKeyList;
		pDoc->m_pAttrCtrl->GetMlttKeyList(aMlttKeyList);
		bSuccess = pDoc->m_pDataCtrl->DelMltt(aMlttKeyList);
	}
	else if (data == WORK_JIMP_LST)   // impact factor
	{
		CArray<T_JIMP_K, T_JIMP_K> aKey;
		GetSelectedJimpKeyListWithMultiTreeItemSelect(aKey);
		bSuccess = pDoc->m_pDataCtrl->DelJimp(aKey);
	}
	else if (data == WORK_IMPF_FLD) //  Additional Impact Factor를 지울 때  // MNET:2812-JWLEE-20070706
	{
		CArray<T_IMPF_K, T_IMPF_K> aImpfKeyList;  
		pDoc->m_pAttrCtrl->GetImpfKeyList(aImpfKeyList);
		bSuccess = pDoc->m_pDataCtrl->DelImpf(aImpfKeyList);
	}
	else if (data == WORK_DYFG_FLD) // Railway Dynamic Factor 를 지울 때
	{
		bSuccess = pDoc->m_pDataCtrl->DelDyfg();
	}
	else if (data == WORK_DYNF_FLD) //  Railway Dynamic Factor by Element 를 지울 때
	{
		CArray<T_DYNF_K, T_DYNF_K> aDynfKeyList;  
		pDoc->m_pAttrCtrl2->GetDynfKeyList(aDynfKeyList);
		bSuccess = pDoc->m_pDataCtrl->DelDynf(aDynfKeyList);
	}
	else if (data == WORK_DYLA_FLD) // Dynamic Load Allowance 를 지울 때
	{
		//bSuccess = pDoc->m_pDataCtrl->DelDyla();
		CArray<T_DYLA_K, T_DYLA_K> aDylaKeyList;  
		pDoc->m_pAttrCtrl2->GetDylaKeyList(aDylaKeyList);
		bSuccess = pDoc->m_pDataCtrl->DelDyla(aDylaKeyList);
	}
	else if(data == WORK_MLSR_FLD) //  Line Lane Support를 지울 때 
	{
		CArray<T_MLSR_K, T_MLSR_K> aMlsrKeyList;  
		pDoc->m_pAttrCtrl->GetMlsrKeyList(aMlsrKeyList);
		bSuccess = pDoc->m_pDataCtrl->DelMlsr(aMlsrKeyList);
	}
	else if (data == WORK_SINF_FLD || data == WORK_SINFch_FLD) // Plate Elements for Influence Surface를 지울 때 
	{
		CArray<T_SINF_K, T_SINF_K> aSinfKeyList;  
		pDoc->m_pAttrCtrl->GetSinfKeyList(aSinfKeyList);
		bSuccess = pDoc->m_pDataCtrl->DelSinf(aSinfKeyList);
	}
	else if (data == WORK_SPFC_LST) // Spectrum Function을 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedSpfcNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelSpfc(aName);
		aName.RemoveAll();
	}
	else if(data == WORK_SPLC_LST) // Spectrum Function을 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedSplcNameListWithMultiTreeItemSelect(aName);
		pDoc->m_pDataCtrl->DelSplc(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_THNL_LST || data == WORK_THSL_LST || data == WORK_THFC_LST || data == WORK_THIS_LST || 
	         data == WORK_THAT_LST || data == WORK_THRD_LST || data == WORK_THCR_LST || data == WORK_THGA_LST)
	{
		bSuccess =  DeleteItemEntity_TimeHistory(pDoc, data);
	}
	else if (data == WORK_CRAN_LST) // Crane Load를 지울 때 
	{
		CArray<T_CRAN_K, T_CRAN_K> aCranKeyList;  
		GetSelectedCranKeyListWithMultiTreeItemSelect(aCranKeyList);

		CArray<CString, CString&> aCranName;
		T_CRAN_D data;
		for (int i = 0; i < aCranKeyList.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetCran(aCranKeyList[i], data);
			aCranName.Add(data.LoadTypeName);
		}
		bSuccess = pDoc->m_pDataCtrl->DelCran(aCranName);
		aCranKeyList.RemoveAll();
	}
	else if (data == WORK_LLAN_LST) // Line Lane를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedLlanKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelLlan(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_SLAN_LST) // Surface Lane를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedSlanKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelSlan(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_MVHL_LST) // Vehicles를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedMvhlKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelMvhl(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_MVHC_LST) // Vehicles Class를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedMvhcKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelMvhc(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_MVLD_LST) // Moving Load Case를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedMvldKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelMvld(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_LLANjp_LST) // Line Lane를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedLlanjpKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelLlanjp(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_LLANjp_FLD) // Line Lane(JAPAN)를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedLlanjpKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelLlanjp(aName);
		aName.RemoveAll();

	}
	else if (data == WORK_MVLDjp_LST) // Moving Load Case(JAPAN)를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedMvldjpKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelMvldjp(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_LLANch_LST) // Line Lane(CHINA)를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedLlanchKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelLlanch(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_SLANch_LST) // Surface Lane(CHINA)를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedSlanchKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelSlanch(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_MVLDch_LST) // Moving Load Case(CHINA)를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedMvldchKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelMvldch(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_LLANid_LST) // Line Lane(INDIA)를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedLlanidKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelLlanid(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_MVLDid_LST) // Moving Load Case(INDIA)를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedMvldidKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelMvldid(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_MVLDbs_LST) // Moving Load Case(BS)를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedMvldbsKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelMvldbs(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_LLANtr_LST)
	{
		CArray<CString, CString&> aName;
		GetSelectedLlantrKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelLlantr(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_MVHLtr_LST)
	{
		CArray<CString, CString&> aName;
		GetSelectedMvhltrKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelMvhltr(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_MVLDtr_LST)
	{
		CArray<CString, CString&> aName;
		GetSelectedMvldtrKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelMvldtr(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_LLANop_LST)
	{
		CArray<CString, CString&> aName;
		GetSelectedLlanopKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelLlanop(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_SLANop_LST)
	{
		CArray<CString, CString&> aName;
		GetSelectedSlanopKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelSlanop(aName);
		aName.RemoveAll();
	}

	else if (data == WORK_LLANfr_LST) // Line Lane(France)를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedLlanfrKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelLlanfr(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_SLANfr_LST) // Surface Lane(France)를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedSlanfrKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelSlanfr(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_MVLDfr_LST) // Moving Load Case(France)를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedMvldfrKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelMvldfr(aName);
		aName.RemoveAll();
	}

	else if (data == WORK_SMPT_LST) // Settlement Group를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedSmptNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelSmpt(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_SMLC_LST) // Settlement Load Case를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedSmlcNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelSmlc(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_HMTP_LST) // Heat of Hydration Material Type을 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedHmtpNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelHmtp(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_HMAT_LST) // Heat of Hydration Material을 지울 때 
	{
		CArray<T_HMAT_K, T_HMAT_K> aKey;
		GetSelectedHmatKeyListWithMultiTreeItemSelect(aKey);
		bSuccess = pDoc->m_pDataCtrl->DelHmat(aKey);
		aKey.RemoveAll();
	}
	else if (data == WORK_ETFC_LST) // Ambient temperature function을 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedEtfcNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelEtfc(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_CCFC_LST) // Coefficient function을 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedCcfcNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelCcfc(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_HECB_LST) // element convection boundary를 지울 때 
	{
		CArray<T_HECB_K, T_HECB_K> aSelKey;
		GetSelectedHecbKeyListWithMultiTreeItemSelect(aSelKey);
		bSuccess = pDoc->m_pDataCtrl->DelHecb(aSelKey);
		aSelKey.RemoveAll();
	}
	else if (data == WORK_HSPT_LST) // Prescribed Temperature를 지울 때 
	{
		CArray<T_HSPT_K, T_HSPT_K> aSelKey;
		GetSelectedHsptKeyListWithMultiTreeItemSelect(aSelKey);
		bSuccess = pDoc->m_pDataCtrl->DelHspt(aSelKey);
		aSelKey.RemoveAll();
	}
	else if (data == WORK_HSFC_LST) // Heat Source Function를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedHsfcKeyListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelHsfc(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_HPCE_LST) // Pipe Cooling Element를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedHpceNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelHpce(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_THMS_LST) // Multiple Support Exitation
	{
		CArray<T_THMS_K, T_THMS_K> aKey;
		GetSelectedThmsKeyListWithMultiTreeItemSelect(aKey);
		bSuccess = pDoc->m_pDataCtrl->DelThms(aKey);
		aKey.RemoveAll();
	}
	else if (data == WORK_TDNT_LST) // Tendon Property를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedTdntNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelTdnt(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_FLUE_LST)
	{
		CArray<T_FLUE_K,T_FLUE_K> aKey;
		GetSelectedFlueKeyListWithMultiTreeItemSelect(aKey);
		bSuccess = pDoc->m_pDataCtrl->DelFlue(aKey);
		aKey.RemoveAll();
	}
	else if (data == WORK_STTF_LST)
	{
		CArray<T_STTF_K,T_STTF_K> aKey;
		GetSelectedSttfKeyListWithMultiTreeItemSelect(aKey);
		bSuccess = pDoc->m_pDataCtrl->DelSttf(aKey);
		aKey.RemoveAll();
	}
	else if (data == WORK_FIPA_LST)
	{
		CArray<T_FIPA_K,T_FIPA_K> aKey;
		GetSelectedFipaKeyListWithMultiTreeItemSelect(aKey);
		bSuccess = pDoc->m_pDataCtrl->DelFipa(aKey);
		aKey.RemoveAll();
	}
	else if (data == WORK_TDCS_LST) // Tendon Location for Composite Section 을 지울 때 
	{
		CArray<T_TDCS_K, T_TDCS_K> aSelTdcsKey;
		GetSelectedTdcsKeyListWithMultiTreeItemSelect(aSelTdcsKey);
		bSuccess = pDoc->m_pDataCtrl->DelTdcs(aSelTdcsKey);
		aSelTdcsKey.RemoveAll();
	}
	else if (data == WORK_TMLD_FLD) // Time Load를 지울 때 
	{
		CArray<T_TMLD_K, T_TMLD_K> aTmldKey;
	  pDoc->m_pAttrCtrl->GetTmldKeyList(aTmldKey);
		bSuccess = pDoc->m_pDataCtrl->DelTmld(aTmldKey);
	}
	else if (data == WORK_CMCS_FLD) // Camber for Construction Stage 를 지울 때 
	{
		CArray<T_CMCS_K, T_CMCS_K> aCmcsKey;
		pDoc->m_pAttrCtrl2->GetCmcsKeyList(aCmcsKey);
		bSuccess = pDoc->m_pDataCtrl->DelCmcs(aCmcsKey);
	}
	else if (data == WORK_CRPC_FLD) // Creep Coefficient for Construction Stage
	{
		CArray<T_CRPC_K, T_CRPC_K> aCrpcKey;
	  pDoc->m_pAttrCtrl->GetCrpcKeyList(aCrpcKey);
		bSuccess = pDoc->m_pDataCtrl->DelCrpc(aCrpcKey);
	}
	else if (data == WORK_TDNA_LST)   // Tendon Profile
	{
		for(int j=0; j< aItem.GetSize(); j++)
			DisappearTendonProfile(aItem[j]);

		CArray<CString, CString&> aName;
		GetSelectedTdnaNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelTdna(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_ELNK_FLD)
	{
		CArray<T_ELNK_K, T_ELNK_K> aElnkK;
		pDoc->m_pAttrCtrl->GetElnkKeyList(aElnkK);
		bSuccess = pDoc->m_pDataCtrl->DelElnk2(aElnkK);
	}
	else if (data == WORK_PDEL_FLD) // P-Delta Analysis Control Data
	{
		bSuccess = pDoc->m_pDataCtrl->DelPdel();
	}
	else if (data == WORK_BUCK_FLD) // Buckling Analysis Control Data
	{
		bSuccess = pDoc->m_pDataCtrl->DelBuck();
	}
	else if (data == WORK_EIGV_FLD) // Eigenvalue Analysis Control Data
	{
		bSuccess = pDoc->m_pDataCtrl->DelEigv();
	}

	/*else if (data == WORK_SPCT_FLD) // Response Spectrum Analysis Control Data
	{
		bSuccess = pDoc->m_pDataCtrl->DelSpct();
	}*/
 
	// MNET:1950-BJLEE(BBONG)-20060318
	else if (data == WORK_MVCT_FLD) // Movong Load Analysis Control Data
	{
		// 코드에 따른 방식으로 수정 
		T_MVCD_D DataMvcd;
		if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
		switch (DataMvcd.nCodeType)
		{
		case D_MOVE_CODE_NONE:
		case D_MOVE_CODE_AASHTO_STAN:
		case D_MOVE_CODE_AASHTO_LRFD:
		case D_MOVE_CODE_PENDOT:
		case D_MOVE_CODE_TAIWAN:
		case D_MOVE_CODE_KOREA:
		case D_MOVE_CODE_CANADA:
		case D_MOVE_CODE_EURO_BS:
		case D_MOVE_CODE_RUSSIA:
		case D_MOVE_CODE_KOREA_LRFD_2011:
		case D_MOVE_CODE_AUSTRALIA:
		case D_MOVE_CODE_POLAND:
		case D_MOVE_CODE_SOUTH_AFRICA:
		case D_MOVE_CODE_JAPAN_RAIL:
		case D_MOVE_CODE_NEWZEALAND:
		case D_MOVE_CODE_BRAZIL:
			bSuccess = pDoc->m_pDataCtrl->DelMvct();
			break;
		case D_MOVE_CODE_CHINA:
			bSuccess = pDoc->m_pDataCtrl->DelMvctch();
			break;
		case D_MOVE_CODE_JAPAN:
			bSuccess = pDoc->m_pDataCtrl->DelMvctjp();
			break;
		case D_MOVE_CODE_INDIA:
			bSuccess = pDoc->m_pDataCtrl->DelMvctid();
			break;
		case D_MOVE_CODE_BS:
			bSuccess = pDoc->m_pDataCtrl->DelMvctbs();
			break;
		case D_MOVE_CODE_FRANCE:
			bSuccess = pDoc->m_pDataCtrl->DelMvctfr();
			break;
		case D_MOVE_CODE_TRANS:
			bSuccess = pDoc->m_pDataCtrl->DelMvcttr();
			break;
		default: 
			ASSERT(0);
			break;
		}
	}
	else if (data == WORK_HHCT_FLD) // Heat of Hydration Analysis Control
	{
		bSuccess = pDoc->m_pDataCtrl->DelHhct();
	}
	else if (data == WORK_SMCT_FLD) // Settlement Analysis Control Data
	{
		bSuccess = pDoc->m_pDataCtrl->DelSmct();
	}
	else if (data == WORK_NLCT_FLD ||
		     data == WORK_NLCT_LST) // Nonlinear Analysis Control Data
	{
		bSuccess = pDoc->m_pDataCtrl->DelNlct();
	}
	else if (data == WORK_STCT_FLD) // Construction Stage Analysis Control Data
	{
		bSuccess = pDoc->m_pDataCtrl->DelStct();
	}
	else if (data == WORK_SBCT_FLD) // Suspension Bridge Analysis Control
	{
		bSuccess = pDoc->m_pDataCtrl->DelSbct();
	}
	else if (data == WORK_GRDP_LST)
	{
		T_GRDP_D GrdpD;
		if(!pDoc->m_pAttrCtrl->GetGrdp(GrdpD))    GrdpD.Initialize();
		if(GrdpD.bExistElement && pos == 1)
		{
			GrdpD.bExistElement = FALSE;
			if(GrdpD.bExistStrain)
			{
				T_GRDP_D GrdpDtmp = GrdpD;
				GrdpD.Initialize();
				GrdpD.bExistStrain = TRUE;
				GrdpD.dStrainDampingDef = GrdpDtmp.dStrainDampingDef;
				GrdpD.aStrainGrupDamping.Copy(GrdpDtmp.aStrainGrupDamping);
			}
		}
		else
		{
			GrdpD.bExistStrain = FALSE;
			if(GrdpD.bExistElement)
			{
				T_GRDP_D GrdpDtmp;    GrdpDtmp.Initialize();
				GrdpD.dStrainDampingDef = GrdpDtmp.dStrainDampingDef;
				GrdpD.aStrainGrupDamping.RemoveAll();
			}
		}

		if(!GrdpD.bExistElement && !GrdpD.bExistStrain)
			bSuccess = pDoc->m_pDataCtrl->DelGrdp();
		else
			bSuccess = pDoc->m_pDataCtrl->AddGrdp(GrdpD);
	}
	// Grid Analysis Model
	else if (data == WORK_MGDR_LST || data == WORK_CBEM_LST || data == WORK_LDLN_LST || data == WORK_GILC_LST ||
					 data == WORK_BRGT_FLD || data == WORK_BSPN_LST || data == WORK_GIMP_LST || data == WORK_GACD_FLD) 
	{
		 bSuccess = DeleteItemEntity_GridAnalModel(pDoc, data);
	}
	else if (data == WORK_FIMP_LST) // fiber material property
	{
		CArray<CString, CString&> aName;
		GetSelectedFimpNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelFimp(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_FIBR_LST) // section devision 
	{
		CArray<T_FIBR_K,T_FIBR_K> aFibrK;
		GetSelectedFibrKeyListWithMultiTreeItemSelect(aFibrK, D_FIBR_TYPE_IEHP);
		bSuccess = pDoc->m_pDataCtrl->DelFibr(aFibrK, D_FIBR_TYPE_IEHP);
	} 
	else if (data == WORK_POGD_FLD)   // Pushover Analysis Control
	{
		bSuccess = pDoc->m_pDataCtrl->DelPogd();
	}
	else if (data == WORK_THGC_FLD)
	{
		bSuccess = pDoc->m_pDataCtrl->DelThgc();
	}
	else if (data == WORK_BCCT_FLD)   // Bounary Change Assignment to Loadcase/Analysis
	{
		bSuccess = pDoc->m_pDataCtrl->DelBcct();
	}
	else if (data == WORK_AETL_FLD)   // Approximate Estimate of Time Dependent Tendon Losses
	{
		bSuccess = pDoc->m_pDataCtrl->DelAetl();
	}
	else if (data == WORK_POLD_LST)   // Pushover Load Case
	{
		CArray<CString, CString&> aName;
		GetSelectedPolcNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelPolc(aName);
		aName.RemoveAll();
	}

	/////////////////////////////////// error C1061: 컴파일러 한계: 블록이 너무 많이 중첩되었습니다.
	if (data == WORK_TDBM_LST)   // PT beam
	{
		for (int j = 0; j < aItem.GetSize(); j++)
			DisappearPTBTendonProfile(aItem[j]);

		CArray<T_TDBM_K, T_TDBM_K> aKey;
		GetSelectedTdbmNameListWithMultiTreeItemSelect(aKey);
		bSuccess = pDoc->m_pDataCtrl->DelTdbm(aKey);
	}
	else if (data == WORK_PTTN_LST)   // PT Slab
	{
		for (int j = 0; j < aItem.GetSize(); j++)
			DisappearPTSTendonProfile(aItem[j]);

		CArray<T_PTTN_K, T_PTTN_K> aKey;
		GetSelectedPttnNameListWithMultiTreeItemSelect(aKey);
		bSuccess = pDoc->m_pDataCtrl->DelPttn(aKey);
	}
	else if (data == WORK_HNGT_LST)  // Pushover Hinge Property Type 지울때 
	{
		CArray<CString, CString&> aName;
		GetSelectedPhgtNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelPhgt(aName);
		aName.RemoveAll();
	}
	else if( data == WORK_POFC_LST)
	{
		CArray<CString, CString&> aName;
		GetSelectedPofcNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelPofc(aName);
		aName.RemoveAll();    
	}
	else if (data == WORK_FSSF_FLD)
	{
		CArray<T_FSSF_K,T_FSSF_K> aKey;
		pDoc->m_pAttrCtrl->GetFssfKeyList(aKey);
		bSuccess = pDoc->m_pDataCtrl->DelFssf(aKey);
		aKey.RemoveAll();
	}
	else if (data == WORK_HNGE_LST)   // Assign Pushover Hinge
	{
		 CArray<T_PHGE_K, T_PHGE_K> aKey;
		 GetSelectedPhgeKeyListWithMultiTreeItemSelect(aKey);
		 bSuccess = pDoc->m_pDataCtrl->DelPhge(aKey);
		 aKey.RemoveAll();
	}
	else if (data == WORK_IFCT_FLD) // Initial Forces Control Data
	{
		bSuccess = pDoc->m_pDataCtrl->DelEfct();
	}
	else if (data == WORK_PORD_LST) // Pushover Result Function
	{
		CArray<T_PORD_K, T_PORD_K> aKey;
		GetSelectedPordKeyListWithMultiTreeItemSelect(aKey);
		T_PORD_D PordD;
		CArray<CString, CString&> aName;
		for (int i=0; i<aKey.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetPord(aKey[i], PordD);
			aName.Add(PordD.Name);
		}
		bSuccess = pDoc->m_pDataCtrl->DelPord(aName);
		aName.RemoveAll(); aKey.RemoveAll();
	}	
	else if (data == WORK_DIMM_LST) 
	{	
		CArray<T_DIMM_K,T_DIMM_K> aKey;
		GetSelectedDimmKeyListWithMultiTreeItemSelect(aKey);  
		T_DIMM_D DimmD;
		CArray<CString, CString&> aName;	
		for (int i=0; i<aKey.GetSize(); i++)
		{
			DimmD.Initialize();
			if(!pDoc->m_pAttrCtrl->GetDimm(aKey[i], DimmD)) {ASSERT(0); continue;}
			aName.Add(DimmD.strName);
		}
		if(aName.GetSize() > 0)
		{
			bSuccess = pDoc->m_pDataCtrl->DelDimm(aName);    
		  if(bSuccess) I_GENModelBase::GetCurMySelfST()->RemoveCurShowDimensionLineKey(aKey);
		}				
	}	
	else if (data == WORK_REBB_LST)
	{
		CArray<T_REBB_K, T_REBB_K> aSelRebbKey;
		GetSelectedRebbKeyListWithMultiTreeItemSelect(aSelRebbKey);
		bSuccess = pDoc->m_pDataCtrl->DelRebb(aSelRebbKey);
	}
	else if (data == WORK_REBC_LST)
	{
		CArray<T_REBC_K, T_REBC_K> aSelRebcKey;
		GetSelectedRebcKeyListWithMultiTreeItemSelect(aSelRebcKey);
		bSuccess = pDoc->m_pDataCtrl->DelRebc(aSelRebcKey);
	}
	else if (data == WORK_REBR_LST)
	{
		CArray<T_REBR_K, T_REBR_K> aSelRebrKey;
		GetSelectedRebrKeyListWithMultiTreeItemSelect(aSelRebrKey);
		bSuccess = pDoc->m_pDataCtrl->DelRebr(aSelRebrKey);
	}
	else if (data == WORK_REBW_LST)
	{
		CArray<T_REBW_K, T_REBW_K> aSelRebwKey;
		GetSelectedRebwKeyListWithMultiTreeItemSelect(aSelRebwKey);
		bSuccess = pDoc->m_pDataCtrl->DelRebw(aSelRebwKey);
	}
	else if (data == WORK_RBMS_LST)
	{
		CArray<T_RBMS_K, T_RBMS_K> aSelRbmsKey;
		GetSelectedRbmsKeyListWithMultiTreeItemSelect(aSelRbmsKey);
		bSuccess = pDoc->m_pDataCtrl->DelRbms(aSelRbmsKey);
		SetItemTree();
	}
	else if (data == WORK_RBSL_LST)
	{
		CArray<T_RBSL_K, T_RBSL_K> aSelRbslKey;
		GetSelectedRbslKeyListWithMultiTreeItemSelect(aSelRbslKey);
		bSuccess = pDoc->m_pDataCtrl->DelRbsl(aSelRbslKey);
		SetItemTree();
	}
	else if (data == WORK_RBMW_LST)
	{
		CArray<T_RBMW_K, T_RBMW_K> aSelRbmwKey;
		GetSelectedRbmwKeyListWithMultiTreeItemSelect(aSelRbmwKey);
		bSuccess = pDoc->m_pDataCtrl->DelRbmw(aSelRbmwKey);
		SetItemTree();
	}
	else if (data == WORK_RBST_LST)
	{
		CArray<T_RBST_K, T_RBST_K> aSelRbstKey;
		GetSelectedRbstKeyListWithMultiTreeItemSelect(aSelRbstKey);
		bSuccess = pDoc->m_pDataCtrl->DelRbst(aSelRbstKey);
		SetItemTree();
	}
	else if (data == WORK_RIPB_LST)
	{
		CArray<T_RIPB_K, T_RIPB_K> aSelRipbKey;
		GetSelectedRipbKeyListWithMultiTreeItemSelect(aSelRipbKey);
		bSuccess = pDoc->m_pDataCtrl->DelRipb(aSelRipbKey);
	}
	else if (data == WORK_RIPC_LST)
	{
		CArray<T_RIPC_K, T_RIPC_K> aSelRipcKey;
		GetSelectedRipcKeyListWithMultiTreeItemSelect(aSelRipcKey);
		bSuccess = pDoc->m_pDataCtrl->DelRipc(aSelRipcKey);
	}
	else if (data == WORK_MADO_LST)
	{
		CArray<T_MADO_K, T_MADO_K> aSelMadoKey;
		GetSelectedMainDomainKeyListWithMultiTreeItemSelect(aSelMadoKey);
		bSuccess = pDoc->m_pDataCtrl->DelMado(aSelMadoKey);
	}
	else if (data == WORK_SBDO_LST)
	{
		std::vector<T_SBDO_K> aSelSbdoKey;
		GetSelectedDomainKeyListWithMultiTreeItemSelect(aSelSbdoKey);
		bSuccess = pDoc->m_pDataCtrl->DelSbdo(aSelSbdoKey);
	}
	else if (data == WORK_CREF_LST)   // cross reference
	{
		CArray<CString, CString&> aName;
		GetSelectedCrefNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelCref(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_P1LT_LST) // P1 load factor type
	{
		CArray<CString, CString&> aName;
		GetSelectedP1ltNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelP1lt(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_P1LA_LST) // P1 load factor
	{
		CArray<T_P1LA_K, T_P1LA_K> aKey;
		GetSelectedP1laKeyListWithMultiTreeItemSelect(aKey);
		bSuccess = pDoc->m_pDataCtrl->DelP1la(aKey);
		aKey.RemoveAll();
	}
	else if (data == WORK_GSTY_LST) // element structure type
	{
		CArray<T_GSTY_K, T_GSTY_K> aKey;
		GetSelectedGstyKeyListWithMultiTreeItemSelect(aKey);
		bSuccess = pDoc->m_pDataCtrl->DelGsty(aKey);
		aKey.RemoveAll();
	}
	else if (data == WORK_LNKF_LST) // impact/P1 load factor for link
	{
		CArray<T_LNKF_K, T_LNKF_K> aKey;
		GetSelectedLnkfKeyListWithMultiTreeItemSelect(aKey);
		bSuccess = pDoc->m_pDataCtrl->DelLnkf(aKey);
		aKey.RemoveAll();
	}
	else if (data == WORK_GMAS_LST) // Grid Nodal Mass를 지울 때 
	{
		CArray<T_GMAS_K, T_GMAS_K> aSelGmasList;
		GetSelectedGmasKeyListWithMultiTreeItemSelect(aSelGmasList);
		bSuccess = pDoc->m_pDataCtrl->DelGmas(aSelGmasList);
		aSelGmasList.RemoveAll();
	}
	else if (data == WORK_STOR_LST) // Story를 지울 때 
	{
		CArray<T_STOR_K, T_STOR_K> aSelStorKey;
		GetSelectedStorKeyListWithMultiTreeItemSelect(aSelStorKey);
		bSuccess = pDoc->m_pDataCtrl->DelStor(aSelStorKey);
		aSelStorKey.RemoveAll();
	}
	else if(data == WORK_WVEP_LST)
	{
		CArray<T_WVEP_K, T_WVEP_K> aSelWvepList;
		GetSelectedWvepKeyListWithMultiTreitemSelect(aSelWvepList);
		bSuccess = pDoc->m_pDataCtrl->DelWvep(aSelWvepList);
		aSelWvepList.RemoveAll();
	}
	else if(data == WORK_WDFC_LST)
	{
		CArray<T_WDFC_K, T_WDFC_K> aSelWdfcList;
		GetSelectedWdfcKeyListWithMultiTreitemSelect(aSelWdfcList);
		bSuccess = pDoc->m_pDataCtrl->DelWdfc(aSelWdfcList);
		aSelWdfcList.RemoveAll();
	}
	else if(data == WORK_PREB_LST)
	{
		CArray<T_PREB_K, T_PREB_K> aSelPrebList;
		GetSelectedPrebKeyListWithMultiTreeItemSelect(aSelPrebList);
		bSuccess = pDoc->m_pDataCtrl->DelPreb(aSelPrebList);
	}
	else if(data == WORK_PRCS_LST)
	{
		CArray<T_PRCS_K, T_PRCS_K> aSelPrcsList;
		GetSelectedPrcsKeyListWithMultiTreeItemSelect(aSelPrcsList);
		bSuccess = pDoc->m_pDataCtrl->DelPrcs(aSelPrcsList);
	}
	else if(data == WORK_DPAN_LST)
	{
		CArray<T_DPAN_K, T_DPAN_K> aSelDpanList;
		GetSelectedDpanKeyListWithMultiTreeItemSelect(aSelDpanList);
		bSuccess = pDoc->m_pDataCtrl->DelDpan(aSelDpanList);
	}
	else if(data == WORK_CCPT_LST)
	{
		CArray<T_CCPT_K, T_CCPT_K> aSelCcptList;
		GetSelectedCcptKeyListWithMultiTreeItemSelect(aSelCcptList);
		bSuccess = pDoc->m_pDataCtrl->DelCcpt(aSelCcptList);
	}
	else if(data == WORK_MSIS_LST)
	{
		CArray<T_MSIS_K, T_MSIS_K> aSelMsisList;
		GetSelectedMsisKeyListWithMultiTreeItemSelect(aSelMsisList);
		bSuccess = pDoc->m_pDataCtrl->DelMsis(aSelMsisList);
	}
	else if (data == WORK_SDVI_LST)  
	{
		CArray<T_SDVI_K, T_SDVI_K> aSelSdviList;
		GetSelectedSdviKeyListWithMultiTreeItemSelect(aSelSdviList);
		bSuccess = pDoc->m_pDataCtrl->DelSdvi(aSelSdviList);
	}
	else if (data == WORK_SDVE_LST)  
	{
		CArray<T_SDVE_K, T_SDVE_K> aSelSdveList;
		GetSelectedSdveKeyListWithMultiTreeItemSelect(aSelSdveList);
		bSuccess = pDoc->m_pDataCtrl->DelSdve(aSelSdveList);
	}
	else if (data == WORK_SDST_LST)  
	{
		CArray<T_SDST_K, T_SDST_K> aSelSdstList;
		GetSelectedSdstKeyListWithMultiTreeItemSelect(aSelSdstList);
		bSuccess = pDoc->m_pDataCtrl->DelSdst(aSelSdstList);
	}
	else if (data == WORK_SDHY_LST)  
	{
		CArray<T_SDHY_K, T_SDHY_K> aSelSdhyList;
		GetSelectedSdhyKeyListWithMultiTreeItemSelect(aSelSdhyList);
		bSuccess = pDoc->m_pDataCtrl->DelSdhy(aSelSdhyList);
	}
	else if (data == WORK_SDIS_LST)  
	{
		CArray<T_SDIS_K, T_SDIS_K> aSelSdisList;
		GetSelectedSdisKeyListWithMultiTreeItemSelect(aSelSdisList);
		bSuccess = pDoc->m_pDataCtrl->DelSdis(aSelSdisList);
	}
	else if (data == WORK_ASGB_LST)
	{
		CArray<T_ASGB_K, T_ASGB_K> aSelAsgbList;
		GetSelectedAsgbKeyListWithMultiTreeItemSelect(aSelAsgbList);
		bSuccess = pDoc->m_pDataCtrl->DelAsgb(aSelAsgbList);
	}
	else if (data == WORK_DMGB_LST) // Section Stiffness Scale Factor를 지울 때 
	{
		CArray<T_DMGB_K, T_DMGB_K> aSelDmgbKey;
		GetSelectedDmgbKeyListWithMultiTreeItemSelect(aSelDmgbKey);
		bSuccess = pDoc->m_pDataCtrl->DelDmgb(aSelDmgbKey);
		//aSelDmgbKey.RemoveAll();
	}
	else if (data == WORK_THRG_LST) // Time History Smart Result - General Link
	{
		CArray<T_THRG_K, T_THRG_K> aSelThrgList;
		GetSelectedThrgKeyListWithMultiTreeItemSelect(aSelThrgList);
		bSuccess = pDoc->m_pDataCtrl->DelThrg(aSelThrgList);
	}
	else if (data == WORK_THRI_LST) // Time History Smart Result - Inelastic Hinge
	{
		CArray<T_THRI_K, T_THRI_K> aSelThriList;
		GetSelectedThriKeyListWithMultiTreeItemSelect(aSelThriList);
		bSuccess = pDoc->m_pDataCtrl->DelThri(aSelThriList);
	}
	else if (data == WORK_THRS_LST) // Time History Smart Result - Seismic Control Device
	{
		CArray<T_THRS_K, T_THRS_K> aSelThrsList;
		GetSelectedThrsKeyListWithMultiTreeItemSelect(aSelThrsList);
		bSuccess = pDoc->m_pDataCtrl->DelThrs(aSelThrsList);
	}
	else if (data == WORK_THEF_LST) // Time History Smart Result - Element Force Graph
	{
		CArray<T_THEF_K, T_THEF_K> aSelThefList;
		GetSelectedThefKeyListWithMultiTreeItemSelect(aSelThefList);
		bSuccess = pDoc->m_pDataCtrl->DelThef(aSelThefList);
	}
	else if (data == WORK_THSF_LST) // Time History Smart Result - Swept Frequency Response
	{
		CArray<T_THSF_K, T_THSF_K> aSelThsfList;
		GetSelectedThsfKeyListWithMultiTreeItemSelect(aSelThsfList);
		bSuccess = pDoc->m_pDataCtrl->DelThsf(aSelThsfList);
	}
	else if (data == WORK_IEHC_FLD) 
	{
		bSuccess = pDoc->m_pDataCtrl->DelIehc();
	}
	else if (data == WORK_SIHO_FLD) 
	{
		CArray<T_SIHO_K,T_SIHO_K> aSihoK;
		pDoc->m_pAttrCtrl->GetSihoKeyList(aSihoK);
		bSuccess = pDoc->m_pDataCtrl->DelSiho(aSihoK);
	}
	else if (data == WORK_SIHG_FLD) 
	{
		CArray<T_SIHG_K,T_SIHG_K> aSihgK;
		pDoc->m_pAttrCtrl->GetSihgKeyList(aSihgK);
		bSuccess = pDoc->m_pDataCtrl->DelSihg(aSihgK);
	}
	else if (data == WORK_SIHP_FLD)
	{
		CArray<T_SIHP_K, T_SIHP_K> aSihpK;
		pDoc->m_pAttrCtrl->GetSihpKeyList(aSihpK);
		bSuccess = pDoc->m_pDataCtrl->DelSihp(aSihpK);
	}
	else if (data == WORK_FIBW_LST) // section devision (MCPM Wall)
	{
		CArray<T_FIBW_K,T_FIBW_K> aFibwK;
		GetSelectedFibwKeyListWithMultiTreeItemSelect(aFibwK, D_FIBW_TYPE_IEHP);
		bSuccess = pDoc->m_pDataCtrl->DelFibw(aFibwK, D_FIBW_TYPE_IEHP);
	} 
	else if (data == WORK_FIMP_PO_LST) // fiber material property
	{
		CArray<CString, CString&> aName;
		GetSelectedFimpNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelFimp(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_FIBR_PO_LST) // section devision 
	{
		CArray<T_FIBR_K,T_FIBR_K> aFibrK;
		GetSelectedFibrKeyListWithMultiTreeItemSelect(aFibrK, D_FIBR_TYPE_PHGT);
		bSuccess = pDoc->m_pDataCtrl->DelFibr(aFibrK, D_FIBR_TYPE_PHGT);
	} 
	else if (data == WORK_FIBW_PO_LST) // section devision 
	{
		CArray<T_FIBW_K,T_FIBW_K> aFibwK;
		GetSelectedFibwKeyListWithMultiTreeItemSelect(aFibwK, D_FIBR_TYPE_PHGT);
		bSuccess = pDoc->m_pDataCtrl->DelFibw(aFibwK, D_FIBR_TYPE_PHGT);
	} 
	else if (data == WORK_POEF_LST) // Pushover Smart Result - Element Force Graph
	{
		CArray<T_POEF_K, T_POEF_K> aSelPoefList;
		GetSelectedPoefKeyListWithMultiTreeItemSelect(aSelPoefList);
		bSuccess = pDoc->m_pDataCtrl->DelPoef(aSelPoefList);
	}
	else if (data == WORK_PORG_LST) // Pushover Smart Result - General Link
	{
		CArray<T_PORG_K, T_PORG_K> aSelPorgList;
		GetSelectedPorgKeyListWithMultiTreeItemSelect(aSelPorgList);
		bSuccess = pDoc->m_pDataCtrl->DelPorg(aSelPorgList);
	}
	else if (data == WORK_PORE_LST) // Pushover Smart Result - Elastic Link
	{
		CArray<T_PORE_K, T_PORE_K> aSelPoreList;
		GetSelectedPoreKeyListWithMultiTreeItemSelect(aSelPoreList);
		bSuccess = pDoc->m_pDataCtrl->DelPore(aSelPoreList);
	}
	else if (data == WORK_PORI_LST) // Pushover Smart Result - Inelastic Hinge
	{
		CArray<T_PORI_K, T_PORI_K> aSelPoriList;
		GetSelectedPoriKeyListWithMultiTreeItemSelect(aSelPoriList);
		bSuccess = pDoc->m_pDataCtrl->DelPori(aSelPoriList);
	}
	else if (data == WORK_MLFC_LST)
	{
		CArray<T_MLFC_K, T_MLFC_K> aSelMlfcKey;
		GetSelectedMlfcKeyListWithMultiTreeItemSelect(aSelMlfcKey);
		bSuccess = pDoc->m_pDataCtrl->DelMlfc(aSelMlfcKey);
		aSelMlfcKey.RemoveAll();
	}
	else if (data == WORK_POSL_LST)
	{
		CArray<T_POSL_K, T_POSL_K> aSelPoslList;
		GetSelectedPoslKeyListWithMultiTreeItemSelect(aSelPoslList);
		bSuccess = pDoc->m_pDataCtrl->DelPosl(aSelPoslList);
		aSelPoslList.RemoveAll();
	}
	else if (data == WORK_POSP_LST)
	{
		CArray<T_POSP_K, T_POSP_K> aSelPospList;
		GetSelectedPospKeyListWithMultiTreeItemSelect(aSelPospList);
		bSuccess = pDoc->m_pDataCtrl->DelPosp(aSelPospList);
		aSelPospList.RemoveAll();
	}
	else if (data == WORK_RFST_LST)
	{
		CArray<T_RFST_K, T_RFST_K> aSelRfstList;
		GetSelectedRfstKeyListWithMultiTreeItemSelect(aSelRfstList);
		CArray<T_ARST_K, T_ARST_K> aArstKey;
		CArray<T_ARST_K, T_ARST_K> aDelArstKey;
		T_ARST_D ArstData;
		pDoc->m_pAttrCtrl->GetArstKeyList(aArstKey);
		for(int i = 0; i < aArstKey.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetArst(aArstKey[i], ArstData);
			for(int j = 0; j < aSelRfstList.GetSize(); j++)
			{
				if(ArstData.ReinforceSectKey == aSelRfstList[j])
				{
					aDelArstKey.Add(aArstKey[i]);
					break;
				}
			}
		}
		bSuccess = pDoc->m_pDataCtrl->DelArst(aDelArstKey);
	}
	else
	{
		// if문이 너무 많아서 컴파일이 안돼서 else 로 쪼갰습니다.
		// 추가 시 아래에 else if 로 추가해 주세요.

		if (data == WORK_STBK_FLD) // Set-Back for Construction Stage
		{
			CArray<T_STBK_K, T_STBK_K> aStbkKey;
			pDoc->m_pAttrCtrl->GetStbkKeyList(aStbkKey);
			bSuccess = pDoc->m_pDataCtrl->DelStbk(aStbkKey);
		}		
		else if (data == WORK_ULCB_FLD) // Unstrained Length of Cable for Nonlinear Construction Stage
		{
			CArray<T_ULCB_K, T_ULCB_K> aUlcbKey;
			pDoc->m_pAttrCtrl->GetUlcbKeyList(aUlcbKey);
			bSuccess = pDoc->m_pDataCtrl->DelUlcb(aUlcbKey);
		}
		else if (data == WORK_CRGR_LST)
		{
			CArray<T_NODE_K, T_NODE_K> rNode;
			T_CRGR_D CrgrD; CrgrD.Initialize();
			GetSelectedCrgrGrupDataWithMultiTreeItemSelect(CrgrD, rNode);
			if (CrgrD.aSelectedGroup.GetSize() > 0)
				bSuccess = pDoc->m_pDataCtrl->AddCrgr(CrgrD);  // modify
			else
				bSuccess = pDoc->m_pDataCtrl->DelCrgr();
		}
		else if (data == WORK_CJFG_LST)
		{
			CArray<T_NODE_K, T_NODE_K> rNode;
			T_CJFG_D CjfgD; CjfgD.Initialize();
			GetSelectedCjfgGrupDataWithMultiTreeItemSelect(CjfgD, rNode);
			if (CjfgD.aSelectedGroup.GetSize() > 0)
				bSuccess = pDoc->m_pDataCtrl->AddCjfg(CjfgD);  // modify
			else
				bSuccess = pDoc->m_pDataCtrl->DelCjfg();
		}
		else if (data == WORK_TDAS_LST)
		{
			CArray<T_TDAS_K, T_TDAS_K> aSelTdasList;
			GetSelectedTdasKeyListWithMultiTreeItemSelect(aSelTdasList);
			bSuccess = pDoc->m_pDataCtrl->DelTdas(aSelTdasList);
			aSelTdasList.RemoveAll();
		}
		else if (data == WORK_DYNZ_FLD) // Dynamic Load Factor 를 지울 때
		{
			bSuccess = TRUE;
			if (pDoc->m_pAttrCtrl2->ExistDynz())
			{
				bSuccess = pDoc->m_pDataCtrl->DelDynz();
			}
		}
		else if (data == WORK_DYBS_FLD) // Railway Dynamic Factor
		{
			bSuccess = TRUE;
			if (pDoc->m_pAttrCtrl2->ExistDybs())
			{
				bSuccess = pDoc->m_pDataCtrl->DelDybs();
			}
		}
		else if (data == WORK_DSFC_FLD) // Railway Dispersal Factor
		{
			bSuccess = TRUE;
			if (pDoc->m_pAttrCtrl2->ExistDsfc())
			{
				bSuccess = pDoc->m_pDataCtrl->DelDsfc();
			}
		}
		else if (data == WORK_IFBZ_FLD) // Additoinal Impact Factor
		{
			bSuccess = TRUE;
			if (pDoc->m_pAttrCtrl2->ExistIfbz())
			{
				bSuccess = pDoc->m_pDataCtrl->DelIfbz();
			}
		}
		//else if (data == WORK_STBK_FLD) // Set-Back for Construction Stage
		//{
		//	CArray<T_STBK_K, T_STBK_K> aStbkKey;
		//	pDoc->m_pAttrCtrl->GetStbkKeyList(aStbkKey);
		//	bSuccess = pDoc->m_pDataCtrl->DelStbk(aStbkKey);
		//}
	}

	// Delete이 수행되지 못했을 경우 Caret 상태 해제 
	if (!bSuccess)
	{
		HTREEITEM hCaret = GetSelectedItem();
		if (hCaret)
		{
			EnableMultiSelect(FALSE);
			SelectItem(hCaret);
			EnableMultiSelect(TRUE);
		}
	}
	else SelectItem(NULL);
}
BOOL CTreeWorkCtrl::DeleteItemEntity_TimeHistory(CDBDoc* pDoc, UINT data)
{
	BOOL bSuccess = FALSE;

	if (data == WORK_THNL_LST) // Dynamic Nodal Loads를 지울 때 
	{
		CArray<T_THNL_K, T_THNL_K> aSelThnlList;
		GetSelectedThnlKeyListWithMultiTreeItemSelect(aSelThnlList);
		bSuccess = pDoc->m_pDataCtrl->DelThnl(aSelThnlList);
		aSelThnlList.RemoveAll();
	}
	else if (data == WORK_THSL_LST) // Dynamic Spatial Loads를 지울 때     
	{
		CArray<T_THSL_K, T_THSL_K> aSelThslList;
		GetSelectedThslKeyListWithMultiTreeItemSelect(aSelThslList);
		bSuccess = pDoc->m_pDataCtrl->DelThsl(aSelThslList);
		aSelThslList.RemoveAll();
	}
	else if (data == WORK_THFC_LST) // Dynamic Nodal Loads를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedThfcNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelThfc(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_THGA_LST) // Ground Acceleration를 지울 때 
	{
		CArray<T_THGA_K, T_THGA_K> aSelThgaList;
		GetSelectedThgaKeyListWithMultiTreeItemSelect(aSelThgaList);
		bSuccess = pDoc->m_pDataCtrl->DelThga(aSelThgaList);
		aSelThgaList.RemoveAll();
	}
	else if (data == WORK_THIS_LST) // 시간이력 load case를 지울 때 
	{
		CArray<CString, CString&> aName;
		GetSelectedThisNameListWithMultiTreeItemSelect(aName);
		bSuccess = pDoc->m_pDataCtrl->DelThis(aName);
		aName.RemoveAll();
	}
	else if (data == WORK_THAT_LST) // time history animation time
	{
		CArray<T_THAT_K, T_THAT_K> aKey;
		GetSelectedThatKeyListWithMultiTreeItemSelect(aKey);
		bSuccess = pDoc->m_pDataCtrl->DelThat(aKey);
		aKey.RemoveAll();
	}
	else if (data == WORK_THRD_LST) // Time History Result Function
	{
		CArray<T_THRD_K, T_THRD_K> aKey;
		GetSelectedThrdKeyListWithMultiTreeItemSelect(aKey);
		T_THRD_D ThrdD;
		CArray<CString, CString&> aName;
		for (int i=0; i<aKey.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetThrd(aKey[i], ThrdD);
			aName.Add(ThrdD.Name);
		}
		bSuccess = pDoc->m_pDataCtrl->DelThrd(aName);
		aName.RemoveAll(); aKey.RemoveAll();
	}
	else if (data == WORK_THCR_LST) // Time history Concurrent Result Group
	{
		CArray<T_THCR_K, T_THCR_K> aKey;
		GetSelectedThcrKeyListWithMultiTreeItemSelect(aKey);
		T_THCR_D ThcrD;
		CArray<CString, CString&> aName;
		for (int i=0; i<aKey.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetThcr(aKey[i], ThcrD);
			aName.Add(ThcrD.GroupName);
		}
		bSuccess = pDoc->m_pDataCtrl->DelThcr(aKey);
		aName.RemoveAll(); aKey.RemoveAll();
	}
	//////////////////////////////////////////////////////////////////////////

	return bSuccess;
}

BOOL CTreeWorkCtrl::GetSelectedItemKeyList(UINT data,CArray<T_NODE_K, T_NODE_K> &rNode,CArray<T_ELEM_K, T_ELEM_K> &rElem)
{
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) 
	{
		TRACE(_LS(IDS_TMWK__MSG1));
		return FALSE;
	}

	rNode.RemoveAll();
	rElem.RemoveAll();

	//=== 무엇을 선택했을 까??? ===
	if(data == WORK_NODE_FLD) // Node로 selection
	{
	 	pDoc->m_pAttrCtrl->GetNodeKeyList(rNode);
		return TRUE;
	}
	if(data == WORK_SPAN_LST) // Span selection
	{
	  CArray<T_SPAN_K, T_SPAN_K> aSpanKeyList;
		GetSelectedSpanKeyListWithMultiTreeItemSelect(aSpanKeyList,rElem,0);
		return TRUE;
	}
	if(data == WORK_SPAN_COMPOSTIE_LST || data == WORK_SPAN_COMPOSTIE_VB_LST) // Span selection
	{
		BOOL bVirtual = (data == WORK_SPAN_COMPOSTIE_VB_LST) ? TRUE : FALSE;

		CArray<T_SPAN_K, T_SPAN_K> aSpanKeyList;
		GetSelectedSpanKeyListWithMultiTreeItemSelect(aSpanKeyList,rElem,1, bVirtual);
		return TRUE;
	}
	if(data == WORK_SPAN_STEELGIRDER_LST) // Span selection
	{
		CArray<T_SPAN_K, T_SPAN_K> aSpanKeyList;
		GetSelectedSpanKeyListWithMultiTreeItemSelect(aSpanKeyList,rElem,2);
		return TRUE;
	}
	
	if (data == WORK_CRGR_LST)
	{
		T_CRGR_D CrgrD; CrgrD.Initialize();
		GetSelectedCrgrGrupDataWithMultiTreeItemSelect(CrgrD, rNode);
		return TRUE;
	}
	else if (data == WORK_CJFG_LST)
	{
		T_CJFG_D CjfgD; CjfgD.Initialize();
		GetSelectedCjfgGrupDataWithMultiTreeItemSelect(CjfgD, rNode);
		return TRUE;
	}
	
	if((data>=WORK_TRUSS_LST && data<=WORK_WALL_LST) ||
					data==WORK_WALLOPEN_LST) // Element로 selection
	{
		GetSelectedElemKeyListWithMultiTreeItemSelect(rElem);
		return TRUE;
	}
	else if(data == WORK_GRUP_LST) // Group으로 선택
	{
	  CArray<T_GRUP_K, T_GRUP_K> aGrupKeyList;
		GetSelectedGrupKeyListWithMultiTreeItemSelect(aGrupKeyList);
		pDoc->m_pAttrCtrl->GetKeyListGrupUser(&aGrupKeyList, &rNode, &rElem);
		return TRUE;
	}
	else if(data == WORK_NPLN_LST) // Named Plane으로 선택
	{
	  CArray<T_NPLN_K, T_NPLN_K> aNplnKeyList;
		GetSelectedNplnKeyListWithMultiTreeItemSelect(aNplnKeyList);
		pDoc->m_pAttrCtrl->GetKeyListNplnUser(&aNplnKeyList, &rNode, &rElem);
		return TRUE;
	}
	else if(data == WORK_MATL_LST) // Material로 선택
	{
	  CArray<T_MATL_K, T_MATL_K> aMatlKeyList;
		GetSelectedMatlKeyListWithMultiTreeItemSelect(aMatlKeyList);
		pDoc->m_pAttrCtrl->GetKeyListMatlUser(&aMatlKeyList, &rNode, &rElem);
		rNode.RemoveAll();
		return TRUE;
	}
	else if(data == WORK_SECT_LST) // Section으로 선택
	{
	  CArray<T_SECT_K, T_SECT_K> aSectKeyList;
		GetSelectedSectKeyListWithMultiTreeItemSelect(aSectKeyList);
		pDoc->m_pAttrCtrl->GetKeyListSectUser(&aSectKeyList, &rNode, &rElem);
		rNode.RemoveAll();
		return TRUE;
	}
	else if (data == WORK_SECV_LST) // SecV으로 선택
	{
		CArray<T_SECV_K, T_SECV_K> aSecvKeyList;
		GetSelectedSecvKeyListWithMultiTreeItemSelect(aSecvKeyList);
		pDoc->m_pAttrCtrl->GetKeyListSecvUser(&aSecvKeyList, &rNode, &rElem);
		rNode.RemoveAll();
		return TRUE;
	}

	else if(data == WORK_STOR_LST) // Section으로 선택
	{
		// nSelectionType == 0   // 해당층과 층하부로 연결된 절점, 요소
		//                == 1   // 해당층과 층상부로 연결된 절점, 요소
		//                == 2   // 해당층과 층상부와 층하부로 연결된 모든 절점, 요소
		//                == 3   // 해당층과 층하부의 모든 절점, 요소
		//                == 4   // 해당층과 층상부의 모든 절점, 요소
		//                == 5   // 해당층만
		//int nSelectionType = 1;

		CArray<T_STOR_K, T_STOR_K> aStorKey;    aStorKey.RemoveAll();
		GetSelectedStorKeyListWithMultiTreeItemSelect(aStorKey);
		//CStoryData StoryData(pDoc);
		//StoryData.MakeStoryData(nSelectionType);
		CStoryData* pStoryData = pDoc->GetStoryData();
		pStoryData->GetMultiStoryNodeKey(aStorKey, rNode);
		pStoryData->GetMultiStoryElemKey(aStorKey, rElem);

		return TRUE;
	}
	else if(data == WORK_CSCS_LST)
	{
		CArray<T_CSCS_K, T_CSCS_K> aCscsKeyList;
		GetSelectedCscsKeyListWithMultiTreeItemSelect(aCscsKeyList);

		CArray<T_SECT_K, T_SECT_K> aSectKeyList;
		T_CSCS_D CscsD;
		int nCscs = aCscsKeyList.GetSize();
		aSectKeyList.SetSize(nCscs);
		for (int i = 0; i < nCscs; i++)
		{
			pDoc->m_pAttrCtrl->GetCscs(aCscsKeyList[i], CscsD);
			aSectKeyList[i] = CscsD.SectKey;
		}
		pDoc->m_pAttrCtrl->GetKeyListSectUser(&aSectKeyList, &rNode, &rElem);
		rNode.RemoveAll();
		return TRUE;
	}
	else if(data == WORK_TSGR_LST) // Tapered Section Group으로 선택
	{
	  CArray<CString, CString&> aName;
		GetSelectedTsgrNameListWithMultiTreeItemSelect(aName);
		for(int i=0; i<aName.GetSize(); i++)
		{
			T_TSGR_D  tsgrData;
		  pDoc->m_pAttrCtrl->GetTsgr(aName[i], tsgrData);
			for(int j=0; j<tsgrData.aElemList.GetSize(); j++) rElem.Add(tsgrData.aElemList[j]);
		}
		rNode.RemoveAll();
		aName.RemoveAll();
		return TRUE;
	}
	else if(data == WORK_THIK_LST) // Thickness로 선택
	{
	  CArray<T_THIK_K, T_THIK_K> aThikKeyList;
		GetSelectedThikKeyListWithMultiTreeItemSelect(aThikKeyList);
		pDoc->m_pAttrCtrl->GetKeyListThikUser(&aThikKeyList, &rNode, &rElem);
		rNode.RemoveAll();
		return TRUE;
	}
	else if (data == WORK_WSSF_LST) // Wall Shear Stiffness Reduction Factor로 선택 
	{
		CArray<T_WSSF_K, T_WSSF_K> aWssfKeyList;
		GetSelectedWssfKeyListWithMultiTreeItemSelect(aWssfKeyList);

		for(int ii=0; ii<aWssfKeyList.GetSize(); ii++) rElem.Add(aWssfKeyList[ii].key.entity);
		aWssfKeyList.RemoveAll();
		return TRUE;
	}
	else if (data == WORK_PSSF_LST) // Plate Shear Stiffness Reduction Factor로 선택 
	{
		CArray<T_PSSF_K, T_PSSF_K> aPssfKeyList;
		GetSelectedPssfKeyListWithMultiTreeItemSelect(aPssfKeyList);

		for(int ii=0; ii<aPssfKeyList.GetSize(); ii++) rElem.Add(aPssfKeyList[ii].key.entity);
		aPssfKeyList.RemoveAll();
		return TRUE;
	}
	else if(data == WORK_CONS_LST) // Support로 선택
	{
		CArray<T_CONS_K, T_CONS_K> aConsKey;
		GetSelectedConsKeyListWithMultiTreeItemSelect(aConsKey);
		for(int ii=0; ii<aConsKey.GetSize(); ii++) rNode.Add(aConsKey[ii].key.entity);
		aConsKey.RemoveAll();
		return TRUE;
	}
	else if(data == WORK_NSPR_LIN_LST) // Point Spring Support로 선택
	{
		CArray<T_NSPR_K, T_NSPR_K> aNsprKey;
		GetSelectedNsprLinearKeyListWithMultiTreeItemSelect(aNsprKey);
		for(int ii=0; ii<aNsprKey.GetSize(); ii++) rNode.Add(aNsprKey[ii].key.entity);
		aNsprKey.RemoveAll();
		return TRUE;
	}
	else if(data == WORK_NSPR_NON_LST) // Point Spring Support로 선택
	{
		CArray<T_NSPR_K, T_NSPR_K> aNsprKey;
		GetSelectedNsprNonKeyListWithMultiTreeItemSelect(aNsprKey);
		for(int ii=0; ii<aNsprKey.GetSize(); ii++) rNode.Add(aNsprKey[ii].key.entity);
		aNsprKey.RemoveAll();
		return TRUE;
	}
	else if(data == WORK_NSPR_MLT_LST)
	{
		CArray<T_NSPR_K, T_NSPR_K> aNsprKey;
		GetSelectedNsprMultiKeyListWithMultiTreeItemSelect(aNsprKey);
		for(int ii=0; ii<aNsprKey.GetSize(); ii++) rNode.Add(aNsprKey[ii].key.entity);
		aNsprKey.RemoveAll();
		return TRUE;
	}
	else if(data == WORK_GSTP_LST) // General Spring Supports로 선택
	{
		CArray<T_GSPR_K, T_GSPR_K> aGsprKey;
		GetSelectedGsprKeyListWithMultiTreeItemSelect(aGsprKey);
		for(int ii = 0; ii < aGsprKey.GetSize(); ii++) rNode.Add(aGsprKey[ii].key.entity);
		return TRUE;
	}

	else if(data == WORK_SSPS_LIN_LST) //Surface Spring Support (Linear)
	{
		CArray<T_SSPS_K, T_SSPS_K> aSspsKey;
		GetSelectedSspsLinearKeyListWithMultiTreeItemSelect(aSspsKey);
		for(int ii=0; ii<aSspsKey.GetSize(); ii++) rElem.Add(aSspsKey[ii].key.entity);
		aSspsKey.RemoveAll();
		return TRUE;
	}
	else if(data == WORK_SSPS_NON_LST) //Surface Spring Support (Comp/Tens)
	{
		CArray<T_SSPS_K, T_SSPS_K> aSspsKey;
		GetSelectedSspsNonKeyListWithMultiTreeItemSelect(aSspsKey);
		for(int ii=0; ii<aSspsKey.GetSize(); ii++) rElem.Add(aSspsKey[ii].key.entity);
		aSspsKey.RemoveAll();
		return TRUE;
	}

	else if(data == WORK_FRLS_LST) // Beam End Release로 선택
	{
		CArray<T_FRLS_K, T_FRLS_K> aFrlsKey;
		GetSelectedFrlsKeyListWithMultiTreeItemSelect(aFrlsKey);
		for(int ii=0; ii<aFrlsKey.GetSize(); ii++) rElem.Add(aFrlsKey[ii].key.entity);
		aFrlsKey.RemoveAll();
		return TRUE;
	}
	else if(data == WORK_OFFS_LST) // Beam End Offset으로 선택
	{
		CArray<T_OFFS_K, T_OFFS_K> aOffsKey;
		GetSelectedOffsKeyListWithMultiTreeItemSelect(aOffsKey);
		for(int ii=0; ii<aOffsKey.GetSize(); ii++) rElem.Add(aOffsKey[ii].key.entity);
		aOffsKey.RemoveAll();
		return TRUE;
	}
	else if(data == WORK_PRLS_LST) // Plate End Release로 선택
	{
		CArray<T_PRLS_K, T_PRLS_K> aPrlsKey;
		GetSelectedPrlsKeyListWithMultiTreeItemSelect(aPrlsKey);
		for(int ii = 0 ; ii < aPrlsKey.GetSize(); ii++) rElem.Add(aPrlsKey[ii].key.entity);
		aPrlsKey.RemoveAll();
		return TRUE;
	}
	else if(data == WORK_RIGD_LST) // Rigid Link의 Slave Nodes로 선택
	{
		CArray<T_RIGD_K, T_RIGD_K> aSelRigdList;
		GetSelectedRigdKeyListWithMultiTreeItemSelect(aSelRigdList);
		T_RIGD_D DataRigd;

		for(int i=0; i<aSelRigdList.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetRigd(aSelRigdList[i],DataRigd);
			for(int j=0;j<DataRigd.arSlave.GetSize();j++)
			{
				rNode.Add(DataRigd.arSlave[j]);
			}
		}
		return TRUE;
	}
	else if(data == WORK_MCON_LST) // Linear Constraints의 Slave Nodes로 선택 MNET:2790-JHLEE-20070911
	{
		CArray<T_MCON_K, T_MCON_K> aSelMconList;
		GetSelectedMconKeyListWithMultiTreeItemSelect(aSelMconList);
		T_MCON_D DataMcon;
		
		for(int i=0; i<aSelMconList.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetMcon(aSelMconList[i],DataMcon);   // DataMcon이 T_MCON_DT의 array를 가지므로
			for(int j=0;j<DataMcon.arSlave.GetSize();j++)           // rNode에 직접 넣을수 없다. 
			{                                                       // DataMcon.arSlave[j] -> DataMcon.arSlave[j].NodeK
				rNode.Add(DataMcon.arSlave[j].NodeK);
			}
		}
		return TRUE;
	}
	else if(data == WORK_DRLS_FLD) // Diaphragm disconnect된 Nodes로 선택
	{
		CArray<T_DRLS_K, T_DRLS_K> aDrlsKeyList;  // Diaphragm disconnect
	  pDoc->m_pAttrCtrl->GetDrlsKeyList(aDrlsKeyList);

		for(int i=0;i<aDrlsKeyList.GetSize();i++) rNode.Add(aDrlsKeyList[i]);
		return TRUE;
	}
	else if(data == WORK_SKEW_LST) // Node Local Axis가 할당된 절점으로 선택
	{
		GetSelectedSkewKeyListWithMultiTreeItemSelect(rNode);
		return TRUE;
	}
	else if (data == WORK_EWSF_FLD)   // effective width scale factor
	{
		CArray<T_EWSF_K, T_EWSF_K> aEwsfKeyList;
		pDoc->m_pAttrCtrl->GetEwsfKeyList(aEwsfKeyList);
		for (int i =0 ; i < aEwsfKeyList.GetSize(); i++) rElem.Add(aEwsfKeyList[i].key.entity);
		return TRUE;
	}
	else if(data == WORK_CLDR_LST) // Define Constraint Label Direction
	{
		GetSelectedCldrKeyListWithMultiTreeItemSelect(rNode);
		return TRUE;
	}
	else if (data == WORK_CGLP_LST)   // change general link property
	{
		CArray<T_CGLP_K, T_CGLP_K> aCglpKeyList;
		GetSelectedCglpKeyListWithMultiTreeItemSelect(aCglpKeyList);

		T_CGLP_D DataCglp;
		T_NLNK_D DataNlnk;
		for (int i = 0; i < aCglpKeyList.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetCglp(aCglpKeyList[i], DataCglp);
			pDoc->m_pAttrCtrl->GetNlnk(DataCglp.GLinkK, DataNlnk);
			rNode.Add(DataNlnk.Node1);
			rNode.Add(DataNlnk.Node2);
		}
		return TRUE;
	}
	else if(data == WORK_NMAS_LST) // Nodal Mass로 선택
	{
		GetSelectedNmasKeyListWithMultiTreeItemSelect(rNode);
		return TRUE;
	}
	else if(data == WORK_SINF_FLD || data == WORK_SINFch_FLD) // Plate Elements for Influence Line
	{
	  CArray<T_SINF_K, T_SINF_K> aSinfKeyList;  // Influence Surface
	  pDoc->m_pAttrCtrl->GetSinfKeyList(aSinfKeyList);
		for(int i=0; i<aSinfKeyList.GetSize(); i++) rElem.Add(aSinfKeyList[i]);
		return TRUE;
	}
	else if(data == WORK_MLSP_FLD) // Lane Supports
	{
	  CArray<T_MLSP_K, T_MLSP_K> aMlspKeyList;  // Line Lane Support
	  pDoc->m_pAttrCtrl->GetMlspKeyList(aMlspKeyList);
		for(int i=0; i<aMlspKeyList.GetSize(); i++)
		{
			if(aMlspKeyList[i].key.serial==0)
				rElem.Add(aMlspKeyList[i].key.entity);
		}
		return TRUE;
	}
	else if(data == WORK_MLSR_FLD) // Lane Supports
	{
	  CArray<T_MLSR_K, T_MLSR_K> aMlsrKeyList;  // Line Lane Support
	  pDoc->m_pAttrCtrl->GetMlsrKeyList(aMlsrKeyList);
		for(int i=0; i<aMlsrKeyList.GetSize(); i++) rNode.Add(aMlsrKeyList[i]);
		return TRUE;
	}
	else if (data == WORK_MLTT_FLD) // Lane Support-Two Trucks
	{
		CArray<T_MLTT_K, T_MLTT_K> aMlttKeyList;  // Line Lane Support
		pDoc->m_pAttrCtrl->GetMlttKeyList(aMlttKeyList);
		for (int i = 0; i < aMlttKeyList.GetSize(); i++)
		{
			int nType = aMlttKeyList[i].key.serial;
			int nKey  = aMlttKeyList[i].key.entity;

			if (nType == 0 || nType == 1 || nType == 2)
			{
				rElem.Add(nKey);
			}
			else if (nType == 3)
			{
				T_ELNK_D ElnkD;
				if (!pDoc->m_pAttrCtrl->GetElnk(nKey, ElnkD)) continue;
				rNode.Add(ElnkD.Node1);
				rNode.Add(ElnkD.Node2);
			}
			else if (nType == 4)
			{
				T_NLNK_D NlnkD;
				if (!pDoc->m_pAttrCtrl->GetNlnk(nKey, NlnkD)) continue;
				rNode.Add(NlnkD.Node1);
				rNode.Add(NlnkD.Node2);
			}
		}
		return TRUE;
	}
	else if (data == WORK_JIMP_LST)
	{
		CArray<T_JIMP_K, T_JIMP_K> aKey;
		GetSelectedJimpKeyListWithMultiTreeItemSelect(aKey);

		T_JIMP_D JimpD;
		for (int i = 0; i < aKey.GetSize(); i++) 
		{
			if (!pDoc->m_pAttrCtrl->GetJimp(aKey[i], JimpD)) continue;
			if      (JimpD.nTgtType == D_JIMP_ELEM) rElem.Add(JimpD.nTgtKey);
			else if (JimpD.nTgtType == D_JIMP_NODE) rNode.Add(JimpD.nTgtKey);
			else if (JimpD.nTgtType == D_JIMP_ELNK) {}
			else if (JimpD.nTgtType == D_JIMP_GLNK) {}
			else ASSERT(0);
		}
	}
	else if(data == WORK_IMPF_FLD) // Additional Impact Factor    // MNET:2812-JWLEE-20070706
	{
		CArray<T_IMPF_K, T_IMPF_K> aImpfKeyList;  
		pDoc->m_pAttrCtrl->GetImpfKeyList(aImpfKeyList);  
		for(int i=0; i<aImpfKeyList.GetSize(); i++) rElem.Add(aImpfKeyList[i].key.entity);
		return TRUE;
	}
	else if(data == WORK_DYNF_FLD) // Railway Dynamic Factor by Element
	{
		CArray<T_DYNF_K, T_DYNF_K> aDynfKeyList;  
		pDoc->m_pAttrCtrl2->GetDynfKeyList(aDynfKeyList);  
		for(int i=0; i<aDynfKeyList.GetSize(); i++) rElem.Add(aDynfKeyList[i]);
		return TRUE;
	}
	else if(data == WORK_THNL_LST) // Dynamic Nodal Loads로 선택
	{
	  CArray<T_THNL_K, T_THNL_K> aThnlKeyList;  
		GetSelectedThnlKeyListWithMultiTreeItemSelect(aThnlKeyList);
		for(int i=0;i<aThnlKeyList.GetSize();i++) rNode.Add(aThnlKeyList[i].key.entity);
		return TRUE;
	}
	/*
	else if(data == WORK_THSL_LST) // Dynamic Spatial Loads로 선택
	{
		GetSelectedThslKeyListWithMultiTreeItemSelect(rElem);
		return TRUE;
	}
	*/
	else if(data == WORK_SMPT_LST) // Settlement Group으로 선택
	{
		CArray<T_SMPT_K, T_SMPT_K> aKey;
		GetSelectedSmptKeyListWithMultiTreeItemSelect(aKey);
		T_SMPT_D smptData;

		for(int i=0; i<aKey.GetSize(); i++)
		{
			pDoc->m_pAttrCtrl->GetSmpt(aKey[i], smptData);
			for(int j=0; j<smptData.aNodeData.GetSize(); j++) rNode.Add(smptData.aNodeData[j]);
		}
		return TRUE;
	}
	else if(data == WORK_HECB_LST) // Element Convection Boundary로 선택
	{
		CArray<T_HECB_K, T_HECB_K> aKey;
		GetSelectedHecbKeyListWithMultiTreeItemSelect(aKey);
		for(int i=0; i<aKey.GetSize(); i++) rElem.Add(aKey[i].key.entity);
		aKey.RemoveAll();
		return TRUE;
	}
	else if(data == WORK_HSPT_LST) // Prescribed Temperature로 선택
	{
		//GetSelectedHsptKeyListWithMultiTreeItemSelect(rNode);
		CArray<T_HSPT_K,T_HSPT_K> aKey;
		GetSelectedHsptKeyListWithMultiTreeItemSelect(aKey);
		for(int i = 0; i < aKey.GetSize(); i++)
		{
			rNode.Add(aKey[i].key.entity);
			rElem.Add(aKey[i].keymap);
		}

		return TRUE;
	}
	else if(data == WORK_HSFC_LST) // Heat Source Function로 선택
	{
		GetSelectedHahsKeyListWithMultiTreeItemSelect(rElem);
		return TRUE;
	}
	else if(data == WORK_HPCE_LST) // Pipe Cooling Element로 선택
	{
		CArray<T_HPCE_D, T_HPCE_D> aData;
		GetSelectedHpcDataListWithMultiTreeItemSelect(aData);

		for(int i=0; i<aData.GetSize(); i++)
		{
			for(int j=0; j<aData[i].aNode.GetSize(); j++) rNode.Add(aData[i].aNode[j].KeyNode);
		}
		return TRUE;
	}
	else if (data == WORK_THMS_LST) // Multiple Support Exitation
	{
		CArray<T_THMS_K, T_THMS_K> aKey;
		int nNum = GetSelectedThmsKeyListWithMultiTreeItemSelect(aKey);
		for (int i=0; i<nNum; i++) rNode.Add(aKey[i].key.entity);
	}
	else if (data == WORK_NLNK_LST)
	{
		T_NLNK_D NlnkD;
		CArray<T_NLNK_K, T_NLNK_K> aKey;
		int nNum = GetSelectedNlnkKeyListWithMultiTreeItemSelect(aKey);
		for (int i=0; i<nNum; i++) 
		{
			if (!pDoc->m_pAttrCtrl->GetNlnk(aKey[i], NlnkD)) ASSERT(0);
			rNode.Add(NlnkD.Node1);
			rNode.Add(NlnkD.Node2);
		}
	}
	else if (data == WORK_FIPA_LST)
	{
		CArray<T_FIPA_K,T_FIPA_K> aKey;
		GetSelectedFipaKeyListWithMultiTreeItemSelect(aKey);
		rElem.Copy(aKey);
	}
	else if (data == WORK_IEHP_LST)
	{
		CArray<T_IEHG_K, T_IEHG_K> aKey;
		CArray<CString,CString&> aStringKey;
		GetSelectedIehpNameListWithMultiTreeItemSelect(aStringKey);
		T_IEHP_D IehpD;
		if(!pDoc->m_pAttrCtrl->GetIehp(aStringKey[0], IehpD)) ASSERT(0);

		pDoc->m_pAttrCtrl->GetIehgKeyList(aKey);
		for (int i = aKey.GetSize() - 1; i >= 0; i--)
		{
			T_IEHG_D IehgD;
			T_IEHP_D tempIehpD;
			if (!pDoc->m_pAttrCtrl->GetIehg(aKey[i], IehgD) ||
				!pDoc->m_pAttrCtrl->GetIehp(IehgD.IehpKey, tempIehpD) ||
				IehpD.strName.CompareNoCase(tempIehpD.strName) != 0)
			{
				aKey.RemoveAt(i);
			}
		}

		GetNodeElemKeyListByIehg(pDoc, aKey, rNode, rElem);
	}
	else if (data == WORK_IEHG_LST)
	{
		CArray<T_IEHG_K, T_IEHG_K> aKey;
		GetSelectedIehgKeyListWithMultiTreeItemSelect(aKey);
		GetNodeElemKeyListByIehg(pDoc, aKey, rNode, rElem);
		//for (int i = 0; i < aKey.GetSize(); i++) rElem.Add(aKey[i]);
	}
	else if (data == WORK_TDNA_LST)
	{
		T_TDNA_D TdnaD;
		CArray<CString, CString&> aName;
		int nNum = GetSelectedTdnaNameListWithMultiTreeItemSelect(aName);
		for (int i = 0; i < nNum; i++)
		{
			if (!pDoc->m_pAttrCtrl->GetTdna(aName[i], TdnaD)) continue;      
			for (int k=0; k<TdnaD.aElemList.GetSize(); k++) rElem.Add(TdnaD.aElemList[k]);
		}
	}
	else if (data == WORK_TDBM_LST)
	{
		T_TDBM_D TdbmD;
		CArray<T_TDBM_K, T_TDBM_K> aKey;
		int nNum = GetSelectedTdbmNameListWithMultiTreeItemSelect(aKey);
		for (int i = 0; i < nNum; i++)
		{
			if (!pDoc->m_pAttrCtrl2->GetTdbm(aKey[i], TdbmD)) continue;
			for (int k=0; k< TdbmD.aElemK.GetSize(); k++) rElem.Add(TdbmD.aElemK[k]);
		}
	}
	else if (data == WORK_PTTN_LST)
	{
		ASSERT(0);
// 		T_PTTN_D PttnD;
// 		CArray<T_PTTN_K, T_PTTN_K> aKey;
// 		int nNum = GetSelectedPttnNameListWithMultiTreeItemSelect(aKey);
// 		for (int i = 0; i < nNum; i++)
// 		{
// 			if (!pDoc->m_pAttrCtrl2->GetPttn(aKey[i], PttnD)) continue;
// 			for (int k = 0; k < PttnD.aElemK.GetSize(); k++) rElem.Add(TdbmD.aElemK[k]);
// 		}
	}

	if (data == WORK_GIMP_LST)
	{
		CArray<T_GIMP_K, T_GIMP_K> aKey;
		GetSelectedGimpKeyListWithMultiTreeItemSelect(aKey);

		T_GIMP_D GimpD;
		for (int i = 0; i < aKey.GetSize(); i++) 
		{
			if (!pDoc->m_pAttrCtrl->GetGimp(aKey[i], GimpD)) continue;
			if      (GimpD.nTgtType == 0) rElem.Add(GimpD.nTgtKey);
			else if (GimpD.nTgtType == 1) rNode.Add(GimpD.nTgtKey);
			else ASSERT(0);
		}
	}
	else if (data == WORK_P1LA_LST)
	{
		CArray<T_P1LA_K, T_P1LA_K> aKey;
		GetSelectedP1laKeyListWithMultiTreeItemSelect(aKey);

		T_P1LA_D P1laD;
		for (int i = 0; i < aKey.GetSize(); i++) 
		{
			if (!pDoc->m_pAttrCtrl->GetP1la(aKey[i], P1laD)) continue;
			if (P1laD.nLoadItem == 0 || P1laD.nLoadItem == 1)
				rNode.Add(P1laD.NodeElemK);
			else if (P1laD.nLoadItem == 2)
				rElem.Add(P1laD.NodeElemK);
			else ASSERT(0);
		}
	}
	else if (data == WORK_GSTY_LST)
	{
		CArray<T_GSTY_K, T_GSTY_K> aKey;
		GetSelectedGstyKeyListWithMultiTreeItemSelect(aKey);

		T_GSTY_D GstyD;
		for (int i=0; i<aKey.GetSize(); i++)
		{
			if (!pDoc->m_pAttrCtrl->GetGsty(aKey[i], GstyD)) continue;
			if (GstyD.nTgtType == 0) rElem.Add(GstyD.nTgtKey);
			else ASSERT(0);
		}
	}
	else if(data == WORK_GMAS_LST) // Grid Nodal Mass로 선택
	{
		GetSelectedGmasKeyListWithMultiTreeItemSelect(rNode);
		return TRUE;
	}
	else if (data == WORK_HNGT_LST)
	{
		CArray<T_PHGT_K, T_PHGT_K> aKey;
		GetSelectedPhgtKeyListWithMultiTreeItemSelect(aKey);
		GetNodeElemKeyListByPhgt(pDoc, aKey, rNode, rElem);
	}
	else if (data == WORK_HNGE_LST)   // Pushover Hinge Property로 선택 
	{
		CArray<T_PHGE_K, T_PHGE_K> aPhgeK;
		GetSelectedPhgeKeyListWithMultiTreeItemSelect(aPhgeK);
		GetNodeElemKeyListByPhge(pDoc, aPhgeK, rNode, rElem);
	}
	else if(data == WORK_BCCT_LST) // MNET1199, KJH20050217
	{
		CArray<T_BNGR_K, T_BNGR_K> aBngrK;
		CString strSel(_T(""));
		GetSelectedBcctBngrKeyListWithMultiTreeItemSelect(aBngrK, strSel);
		GetNodeElemKeyListByBngr(strSel, aBngrK, rNode, rElem);
	}
	else if(data == WORK_REBB_LST)
	{
		CArray<T_REBB_K, T_REBB_K> aRebbK;
		CString strSel(_T(""));
		int nRebbCount = GetSelectedRebbKeyListWithMultiTreeItemSelect(aRebbK);
		CArray<T_ELEM_K,T_ELEM_K> aTempElemK;
		for(int i = 0; i < nRebbCount; i++)
		{
			pDoc->m_pAttrCtrl->GetAllElemKeyListUseRebb(aRebbK[i], aTempElemK);
			rElem.Append(aTempElemK);
		}
	}
	else if(data == WORK_REBC_LST)
	{
		CArray<T_REBC_K, T_REBC_K> aRebcK;
		CString strSel(_T(""));
		int nRebcCount = GetSelectedRebcKeyListWithMultiTreeItemSelect(aRebcK);
		CArray<T_ELEM_K,T_ELEM_K> aTempElemK;
		for(int i = 0; i < nRebcCount; i++)
		{
			pDoc->m_pAttrCtrl->GetAllElemKeyListUseRebc(aRebcK[i], aTempElemK);
			rElem.Append(aTempElemK);
		}
	}
	else if(data == WORK_REBR_LST)
	{
		CArray<T_REBR_K, T_REBR_K> aRebrK;
		CString strSel(_T(""));
		int nRebrCount = GetSelectedRebrKeyListWithMultiTreeItemSelect(aRebrK);
		CArray<T_ELEM_K,T_ELEM_K> aTempElemK;
		for(int i = 0; i < nRebrCount; i++)
		{
			pDoc->m_pAttrCtrl->GetAllElemKeyListUseRebr(aRebrK[i], aTempElemK);
			rElem.Append(aTempElemK);
		}
	}
	else if(data == WORK_REBW_LST)
	{
		CArray<T_REBW_K, T_REBW_K> aRebwK;
		CString strSel(_T(""));
		int nRebwCount = GetSelectedRebwKeyListWithMultiTreeItemSelect(aRebwK);
		CArray<T_ELEM_K,T_ELEM_K> aTempElemK;
		for(int i = 0; i < nRebwCount; i++)
		{
			pDoc->m_pAttrCtrl->GetElemKeyListUseRebw(aRebwK[i], aTempElemK);
			rElem.Append(aTempElemK);
		}
	}
	else if(data == WORK_RBMS_LST)
	{
		CArray<T_RBMS_K, T_RBMS_K> aRbmsK;
		CString strSel(_T(""));
		int nRbmsCount = GetSelectedRbmsKeyListWithMultiTreeItemSelect(aRbmsK);
		CArray<T_ELEM_K,T_ELEM_K> aTempElemK;
		for(int i = 0; i < nRbmsCount; i++)
		{
			pDoc->m_pAttrCtrl->GetElemKeyListUseRbms(aRbmsK[i], aTempElemK);
			rElem.Append(aTempElemK);
		}
	}
	else if(data == WORK_RBSL_LST)
	{
		CArray<T_RBSL_K, T_RBSL_K> aRbslK;
		CString strSel(_T(""));
		int nRbslCount = GetSelectedRbslKeyListWithMultiTreeItemSelect(aRbslK);
		CArray<T_ELEM_K,T_ELEM_K> aTempElemK;
		for(int i = 0; i < nRbslCount; i++)
		{
			pDoc->m_pAttrCtrl->GetElemKeyListUseRbsl(aRbslK[i], aTempElemK);
			rElem.Append(aTempElemK);
		}
	}
	else if(data == WORK_RBMW_LST)
	{
		CArray<T_RBMW_K, T_RBMW_K> aRbmwK;
		CString strSel(_T(""));
		int nRbmwCount = GetSelectedRbmwKeyListWithMultiTreeItemSelect(aRbmwK);
		CArray<T_ELEM_K,T_ELEM_K> aTempElemK;
		for(int i = 0; i < nRbmwCount; i++)
		{
			pDoc->m_pAttrCtrl->GetElemKeyListUseRbmw(aRbmwK[i], aTempElemK);
			rElem.Append(aTempElemK);
		}
	}
	else if(data == WORK_RBST_LST)
	{
		CArray<T_RBST_K, T_RBST_K> aRbstK;
		CString strSel(_T(""));
		int nRbstCount = GetSelectedRbstKeyListWithMultiTreeItemSelect(aRbstK);
		CArray<T_ELEM_K,T_ELEM_K> aTempElemK;
		for(int i = 0; i < nRbstCount; i++)
		{
			pDoc->m_pAttrCtrl2->GetElemKeyListUseRbst(aRbstK[i], aTempElemK);
			rElem.Append(aTempElemK);
		}
	}
	else if(data == WORK_RIPB_LST)
	{
		CArray<T_RIPB_K, T_RIPB_K> aRipbK;
		CString strSel(_T(""));
		int nRipbCount = GetSelectedRipbKeyListWithMultiTreeItemSelect(aRipbK);
		CArray<T_ELEM_K, T_ELEM_K> aElemK;
		pDoc->m_pAttrCtrl2->GetElemKeyListUseRipb(aRipbK, aElemK);
		rElem.Append(aElemK);
	}
	else if(data == WORK_RIPC_LST)
	{
		CArray<T_RIPC_K, T_RIPC_K> aRipcK;
		CString strSel(_T(""));
		int nRipcCount = GetSelectedRipcKeyListWithMultiTreeItemSelect(aRipcK);
		CArray<T_ELEM_K, T_ELEM_K> aElemK;
		pDoc->m_pAttrCtrl2->GetElemKeyListUseRipc(aRipcK, aElemK);
		rElem.Append(aElemK);
	}
	else if(data == WORK_MADO_LST || data == WORK_SBDO_LST)
	{
		std::map<T_ELEM_K,T_ELEM_K> mapElemK;
		typedef std::pair<T_ELEM_K,T_ELEM_K> pairElemK;
		std::vector<T_ELEM_K> aTempElemK;
		std::vector<T_ELEM_K> rElemTemp;

		CArray<T_DOEL_K, T_DOEL_K> aDoelK;
		pDoc->m_pAttrCtrl->GetDoelKeyList(aDoelK);

		if(data == WORK_MADO_LST)
		{
			CArray<T_MADO_K, T_MADO_K> aMadoK;
			int nMadoCount = GetSelectedMainDomainKeyListWithMultiTreeItemSelect(aMadoK);
			for(int i=0; i<nMadoCount; i++)
			{
				int nElSize = pDoc->m_pAttrCtrl->GetElemListMadoK(aMadoK[i], aTempElemK, aDoelK);
				rElemTemp.insert(rElemTemp.end(), aTempElemK.begin(), aTempElemK.end());
				for(int e=0 ; e<nElSize ; ++e)
				{
					mapElemK.insert(pairElemK(aTempElemK[e], aTempElemK[e]));
				}
			}
		}
		else
		{
			std::vector<T_SBDO_K> aSbdoK;
			int nSbdoCount = GetSelectedDomainKeyListWithMultiTreeItemSelect(aSbdoK);
			for(int i=0; i<nSbdoCount; i++)
			{
				int nElSize = pDoc->m_pAttrCtrl->GetElemListSbdoK(aSbdoK[i], aTempElemK, aDoelK);
				rElemTemp.insert(rElemTemp.end(), aTempElemK.begin(), aTempElemK.end());
				for(int e=0 ; e<nElSize ; ++e)
				{
					mapElemK.insert(pairElemK(aTempElemK[e], aTempElemK[e]));
				}
			}
		}

		std::vector<T_NODE_K> rNodeTemp;  rNodeTemp.clear();
		std::vector<T_NODE_K> aTempNodeK; aTempNodeK.clear();
		pDoc->m_pAttrCtrl->GetElemNodeKeyList(rElemTemp, aTempNodeK);

		int nNodeSize = (int)aTempNodeK.size();
		rNodeTemp.reserve(nNodeSize);
		for(int i=0 ; i<nNodeSize ; ++i)
		{
			T_NODE_K NodeK = aTempNodeK[i];
			aTempElemK.clear();
			pDoc->m_pAttrCtrl->GetConnectedElem(NodeK, aTempElemK);      
			int nSizeTempElem = (int)aTempElemK.size();
			if(nSizeTempElem == 0)
			{
				rNodeTemp.push_back(NodeK);
				continue;
			}
			BOOL bFreeNode = TRUE;
			for(int e=0 ; e<nSizeTempElem ; ++e)
			{
				T_ELEM_K ElemK = aTempElemK[e];
				if(mapElemK.find(ElemK) == mapElemK.end())
				{
					bFreeNode = FALSE;
					break;
				}
			}
			if(bFreeNode)
				rNodeTemp.push_back(NodeK);
		}

		rElem.SetSize(rElemTemp.size());
		memcpy(rElem.GetData(), rElemTemp.data(), sizeof(T_ELEM_K)*rElemTemp.size());
		rNode.SetSize(rNodeTemp.size());
		memcpy(rNode.GetData(), rNodeTemp.data(), sizeof(T_NODE_K)*rNodeTemp.size());

	}
	else if(data==WORK_PREB_LST)
	{
		GetSelectedElemKeyListByPrebWithMultiTreeItemSelect(rElem);
	}
	else if(data==WORK_PRCS_LST)
	{
		GetSelectedElemKeyListByPrcsWithMultiTreeItemSelect(rElem);
	}
	else if(data==WORK_DPAN_LST)
	{
		GetSelectedElemKeyListByDpanWithMultiTreeItemSelect(rElem);
	}
	else if(data==WORK_CCPT_LST)
	{
		GetSelectedElemKeyListByCcptWithMultiTreeItemSelect(rElem);
	}
	else if(data==WORK_MSIS_LST)
	{
		GetSelectedElemKeyListByMsisWithMultiTreeItemSelect(rElem);
	}
// 	else if (data == WORK_SECF_LST) // element Stiffness Scale Factor로 선택 
// 	{
// 		CArray<T_SECF_K, T_SECF_K> aSecfKeyList;
// 		GetSelectedSecfKeyListWithMultiTreeItemSelect(aSecfKeyList);
// 
// 		CArray<T_SECT_K, T_SECT_K> aSectKeyList;
// 		for (int ii=0; ii<aSecfKeyList.GetSize(); ii++) aSectKeyList.Add(aSecfKeyList[ii].key.entity);
// 		
// 		pDoc->m_pAttrCtrl->GetKeyListSectUser(&aSectKeyList, &rNode, &rElem);
// 		rNode.RemoveAll();
// 	}
	else if (data == WORK_ESSF_LST) // element Stiffness Scale Factor로 선택 
	{
		CArray<T_ESSF_K, T_ESSF_K> aEssfKeyList;
		GetSelectedEssfKeyListWithMultiTreeItemSelect(aEssfKeyList);

		for (int ii=0; ii<aEssfKeyList.GetSize(); ii++) rElem.Add(aEssfKeyList[ii].key.entity);
		aEssfKeyList.RemoveAll();
	}
	else if (data == WORK_CNLD_FLD) // Static Load와 연결된 객체 선택
	{
		CArray<T_CNLD_K, T_CNLD_K> aCnldKeyList;
		int nSize = GetSelectedCnldKeyListWithMultiTreeItemSelect(aCnldKeyList);
		for (int ii = 0; ii < nSize; ii++) rNode.Add(aCnldKeyList[ii].key.entity);
		aCnldKeyList.RemoveAll();
	}
	else if (data == WORK_BMLD_ELEM_FLD)
	{
		CArray<T_BMLD_K, T_BMLD_K> aBmldKeyList; 
		int nSize = GetSelectedBmldKeyListWithMultiTreeItemSelect(aBmldKeyList, D_BMLDTYPE_BEAM);
		for (int ii = 0; ii < nSize; ii++) rElem.Add(aBmldKeyList[ii].key.entity);
		aBmldKeyList.RemoveAll();
	}
	else if (data == WORK_BMLD_TYPC_FLD)
	{
		CArray<T_BMLD_K, T_BMLD_K> aBmldKeyList;
		int nSize = GetSelectedBmldKeyListWithMultiTreeItemSelect(aBmldKeyList, D_BMLDTYPE_TYPICAL);
		for (int ii = 0; ii < nSize; ii++) rElem.Add(aBmldKeyList[ii].key.entity);
		aBmldKeyList.RemoveAll();
	}
	else if (data == WORK_PRES_FLD)
	{
		CArray<T_PRES_K, T_PRES_K> aPresKeyList;
		int nSize = GetSelectedPresKeyListWithMultiTreeItemSelect(aPresKeyList);
		for (int ii = 0; ii < nSize; ii++) rElem.Add(aPresKeyList[ii].key.entity);
		aPresKeyList.RemoveAll();
	}
	else if (data == WORK_NTMP_FLD)
	{
		CArray<T_NTMP_K, T_NTMP_K> aNtmpKeyList;
		int nSize = GetSelectedNtmpKeyListWithMultiTreeItemSelect(aNtmpKeyList);
		for (int ii = 0; ii < nSize; ii++) rNode.Add(aNtmpKeyList[ii].key.entity);
		aNtmpKeyList.RemoveAll();
	}
	else if (data == WORK_ETMP_FLD)
	{
		CArray<T_ETMP_K, T_ETMP_K> aEtmpKeyList;
		int nSize = GetSelectedEtmpKeyListWithMultiTreeItemSelect(aEtmpKeyList);
		for (int ii = 0; ii < nSize; ii++) rElem.Add(aEtmpKeyList[ii].key.entity);
		aEtmpKeyList.RemoveAll();
	}
	else if (data == WORK_SDSP_FLD)
	{
		CArray<T_SDSP_K, T_SDSP_K> aSdspKeyList;
		int nSize = GetSelectedSdspKeyListWithMultiTreeItemSelect(aSdspKeyList);
		for (int ii = 0; ii < nSize; ii++) rNode.Add(aSdspKeyList[ii].key.entity);
		aSdspKeyList.RemoveAll();
	}
	else if (data == WORK_RFST_LST)
	{
		CArray<T_RFST_K, T_RFST_K> aRfstKeyList;
		GetSelectedRfstKeyListWithMultiTreeItemSelect(aRfstKeyList);
		pDoc->m_pAttrCtrl->GetKeyListRfstUser(&aRfstKeyList, NULL, &rElem);
		return TRUE;
	}
	else return FALSE;  

	return TRUE;
}



// Static Node Type Loads Item
HTREEITEM CTreeWorkCtrl::SetStaticCnldTypeLoadsItemTree(HTREEITEM hTI)
{
#ifndef _CH
	return NULL;
#endif
	ILabelMgr_CH::instance()->m_DrawCnldTypeKey.RemoveAll();
	ILabelMgr_CH::instance()->m_bDrawCnldType = false;
	//
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	//++++++++++++++++++++++++++++++++++++++++++++
	//T_UNIT_INDEX CurUnit,TempUnit;
	//pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurUnit);
	//TempUnit = CurUnit;
	//TempUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
	//TempUnit.nBase_Force	= D_UNITSYS_FORCE_INDEX_KN;
	//pDoc->m_pUnitCtrl->SetUnitIndexCurrent(TempUnit);
	//++++++++++++++++++++++++++++++++++++++++++++

	HTREEITEM ThTi;
	CString	tStr;
	CString	strFor = _T("FX:%g,FY:%g,FZ:%g,MX:%g,MY:%g,MZ:%g,(%d)");
	CArray<T_STLD_K, T_STLD_K> aStldKeyList;
	
	pDoc->m_pAttrCtrl->GetStldKeyList(aStldKeyList);
	int nStldCount = aStldKeyList.GetSize();
	// === Static Load Case 
	ThTi = InsertTreeItem(hTI,WORK_CNLD_TYPE_FLD,_LS(IDS_WG_TREEMENU_Nodal_Loads_Type), _T("needtoimage"));
	hTI = ThTi;
	T_STLD_D stldData;
	//
	//CMapEx<UINT,UINT,UINT,UINT> mapKeyList; mapKeyList.InitHashTable(999);
	//pDoc->m_pPostCtrl->GetCrackedLCKeyList(&mapKeyList);
	//
	for(int i=0; i < nStldCount; i++)
	{
		//if(mapKeyList.Lookup(aStldKeyList[i], CrackedLCKey)) continue;
		//
		T_STLD_K KeyStld = aStldKeyList[i];
		CArray<T_CNLD_D,T_CNLD_D&> aData;
		CArray<CArrayEx<T_CNLD_KEY,T_CNLD_KEY&>, CArrayEx<T_CNLD_KEY,T_CNLD_KEY&>&> aArrCnldK;
		pDoc->m_pAttrCtrl->GetCnldTypeList(KeyStld,aData,aArrCnldK);
		if(aArrCnldK.IsEmpty())
			continue;

		pDoc->m_pAttrCtrl->GetStld(aStldKeyList[i], stldData);
		tStr.Format(_LS(IDS_TMWK__MSG15),i+1,stldData.LoadCaseName,stldData.Description);
		ThTi = InsertTreeItem(hTI,WORK_CNLD_TYPE_LST,tStr, _T("needtoimage"));
		for(int j = 0;j < aData.GetSize();j++)
		{
			tStr.Format(aData[j].strType + _T(":") + strFor,aData[j].Force[0],aData[j].Force[1],aData[j].Force[2],
				aData[j].Force[3],aData[j].Force[4],aData[j].Force[5],aArrCnldK[j].GetSize());
			InsertTreeItem(ThTi,WORK_CNLD_SUB_LST,tStr, _T("needtoimage"));
		}
	}
	//pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurUnit);
	return hTI;
}

int CTreeWorkCtrl::GetSelectedCnldType_Elements(T_STLD_K stldK,int pos,CArray<T_CNLD_KEY, T_CNLD_KEY&>& aCnldKey,T_CNLD_D& cnld)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<T_CNLD_D, T_CNLD_D &> aData;
	CArray<CArrayEx<T_CNLD_KEY,T_CNLD_KEY&>, CArrayEx<T_CNLD_KEY,T_CNLD_KEY&>&> aArrCnldK;
	BOOL bExist = pDoc->m_pAttrCtrl->GetCnldTypeList(stldK,aData,aArrCnldK);
	cnld.Initialize();
	if(bExist)
	{
		ASSERT(aData.GetSize() >= pos);
		cnld = aData[pos-1];
		aCnldKey.RemoveAll();
		aCnldKey.Copy(aArrCnldK[pos-1]);
	}
	return aCnldKey.GetSize();
}

void CTreeWorkCtrl::EditProperty_CnldType(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos)
{
	CArray<T_NODE_K, T_NODE_K>		aKey;
	T_CNLD_K key;
	T_CNLD_D cnld;
	T_STLD_K stldK = FindCnldType(ht);
	CArray<T_CNLD_KEY, T_CNLD_KEY&>	aCnldKey;
	//
	GetSelectedCnldType_Elements(stldK,pos,aCnldKey,cnld);
	for(int i = 0;i < aCnldKey.GetSize();i++)
	{
		key.keymap = aCnldKey[i];
		aKey.Add(key.key.entity);
	}
	MITC_CommonPara::instance()->m_Cnld = cnld;
	pDoc->m_pViewCtrl->UnSelectAllNode(NULL);
	pDoc->m_pViewCtrl->SelectNode(NULL,aKey);
	if(CTreeMenuBarBase::GetModeST() == __TMMODE_STRUCT_STLD_CNLD_TYPE__)
	{
		CTreeMenuBarBase::GetMySelfST()->SetModeOnly(-1);
	}
	//else
	CTreeMenuBarBase::SetModeST(__TMMODE_STRUCT_STLD_CNLD_TYPE__);
}

T_STLD_K CTreeWorkCtrl::FindCnldType(HTREEITEM ht)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	HTREEITEM parentItem = GetParentItem(ht);
	HTREEITEM prvHt;
	int stldpos = 0;
	for (prvHt = parentItem; prvHt != NULL; parentItem = prvHt, stldpos++) prvHt = GetPrevSiblingItem(parentItem);
	CArray<T_STLD_K, T_STLD_K> aStldKeyList;
	CArray<T_CNLD_KEY, T_CNLD_KEY&>	aCnldKey;
	CArray<T_NODE_K, T_NODE_K>		aKey;
	T_CNLD_D cnld;
	pDoc->m_pAttrCtrl->GetStldKeyList(aStldKeyList);
	int nStldCount = aStldKeyList.GetSize();
	int ndbpos = 0; T_STLD_K stldK = 0;
	for (int i = 0; i < nStldCount; i++)
	{
		T_STLD_K KeyStld = aStldKeyList[i];
		CArray<T_CNLD_D, T_CNLD_D&> aData;
		CArray<CArrayEx<T_CNLD_KEY, T_CNLD_KEY&>, CArrayEx<T_CNLD_KEY, T_CNLD_KEY&>&> aArrCnldK;
		pDoc->m_pAttrCtrl->GetCnldTypeList(KeyStld, aData, aArrCnldK);
		if (aArrCnldK.IsEmpty())
			continue;
		ndbpos++;
		if (ndbpos == stldpos)
		{
			stldK = KeyStld;
			break;
		}
	}
	return stldK;
}

