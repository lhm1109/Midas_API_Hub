#if !defined(AFX_TREEWORKCTRL_H__CE53B22D_F5CE_4C32_8EAD_1EB5F74EB88C__INCLUDED_)
#define AFX_TREEWORKCTRL_H__CE53B22D_F5CE_4C32_8EAD_1EB5F74EB88C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeWorkCtrl.h : header file
//
#include "TreeWorkDefine.h"
#include "..\wg_base\wg_base_ColorTreeCtrl.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\MIT_frx\MBaseTreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////////////////
// CTreeWorkCtrl window
//
/////////////////////////////////////////////////////////////////////////////////////////
class CTreeWorkCtrl : public mit::frx::MBaseTreeCtrl, public CDBUpdateConnector
{
// Construction
public:
	DECLARE_DYNAMIC(CTreeWorkCtrl);
	CTreeWorkCtrl();
	virtual ~CTreeWorkCtrl();

// Attributes
protected:
	//////////////////////////////////////////////////////////////////////////////////////////
	// Member Data
	// 실제로는 없는, 같은 값을 가진 데이터 타입을 만들기 위해 필요
	CArray<T_CONS_D, T_CONS_D&>  m_aConsInfo;
	//CArray<T_NSPR_D, T_NSPR_D&>  m_aNsprInfo;
	CArray<T_NSPR_D, T_NSPR_D&>  m_aNsprLinearInfo;
	CArray<T_NSPR_D, T_NSPR_D&>  m_aNsprCompTensInfo;
	CArray<T_NSPR_D, T_NSPR_D&>  m_aNsprMultiInfo;
	CArray<T_SSPS_D, T_SSPS_D&>  m_aSspsLinearInfo;
	CArray<T_SSPS_D, T_SSPS_D&>  m_aSspsCompTensInfo;
	CArray<T_FRLS_D, T_FRLS_D&>  m_aFrlsInfo;
	CArray<T_OFFS_D, T_OFFS_D&>  m_aOffsInfo;
	CArray<T_PRLS_D, T_PRLS_D&>  m_aPrlsInfo;
	CArray<T_SKEW_D, T_SKEW_D&>  m_aSkewInfo;
	CArray<T_NMAS_D, T_NMAS_D&>  m_aNmasInfo;
	CArray<T_THNL_D, T_THNL_D&>  m_aThnlInfo;
	CArray<T_THMS_D, T_THMS_D&>  m_aThmsInfo;
	CArray<T_HSPT_D, T_HSPT_D&>  m_aHsptInfo;
	CArray<T_HECB_D, T_HECB_D&>  m_aHecbInfo;
	CArray<T_NLNK_D, T_NLNK_D&>  m_aNlnkInfo;
	CArray<T_THSL_D, T_THSL_D&>  m_aThslInfo;
	CArray<T_IEHG_D, T_IEHG_D&>  m_aIehgInfo;
	CArray<T_ESSF_D, T_ESSF_D&>  m_aEssfInfo;
	CArray<T_WSSF_D, T_WSSF_D&>  m_aWssfInfo;
	CArray<T_PSSF_D, T_PSSF_D&>  m_aPssfInfo;
	CArray<T_GIMP_D, T_GIMP_D&>  m_aGimpInfo;
	CArray<T_JIMP_D, T_JIMP_D&>  m_aJimpInfo;
	CArray<T_P1LA_D, T_P1LA_D&>  m_aP1laInfo;
	CArray<T_GSTY_D, T_GSTY_D&>  m_aGstyInfo;
	CArray<T_GMAS_D, T_GMAS_D&>  m_aGmasInfo;
	CArray<T_MLFC_D, T_MLFC_D&>  m_aMlfcInfo;
	CArray<T_RLFC_D, T_RLFC_D&>  m_aRlfcInfo;
	//CArray<T_LNKF_D, T_LNKF_D>  m_aLnkfInfo;
	// 사용된것, 사용되지 않은 것, Undefined를 모두 포함하는 리스트를 만들기 위해 필요
	CArray<T_ESSF_K, T_ESSF_K>  m_aEssfItemKey;
	CArray<T_WSSF_K, T_WSSF_K>  m_aWssfItemKey;
	CArray<T_MATL_K, T_MATL_K>  m_aMatlItemKey;
	CArray<T_SECT_K, T_SECT_K>  m_aSectItemKey;
	CArray<T_SECV_K, T_SECV_K>  m_aSecvItemKey;
	CArray<T_THIK_K, T_THIK_K>  m_aThikItemKey;
	CArray<T_NLLP_K, T_NLLP_K>  m_aNllpItemKey;
	CArray<T_IEHP_K, T_IEHP_K>  m_aIehpItemKey;
	CArray<T_THAT_K, T_THAT_K>  m_aThatItemKey;
	CArray<T_CRAN_K, T_CRAN_K>  m_aCranItemKey;
	CArray<T_IMPF_K, T_IMPF_K>  m_aImpfItemKey;        // MNET:2812-JWLEE-20070626
	CArray<T_DYNF_K, T_DYNF_K>  m_aDynfItemKey;
	CArray<T_PHGT_K, T_PHGT_K>  m_aPhgtItemKey;
	CArray<UINT, UINT>          m_aCldrDir;
	CArray<T_CLDR_K, T_CLDR_K>  m_aCldrXPKey;
	CArray<T_CLDR_K, T_CLDR_K>  m_aCldrXNKey;
	CArray<T_CLDR_K, T_CLDR_K>  m_aCldrYPKey;
	CArray<T_CLDR_K, T_CLDR_K>  m_aCldrYNKey;
	CArray<T_CLDR_K, T_CLDR_K>  m_aCldrZPKey;
	CArray<T_CLDR_K, T_CLDR_K>  m_aCldrZNKey;
	CArray<T_WVEP_K, T_WVEP_K>  m_aWvepKey;
	CArray<T_WDFC_K, T_WDFC_K>  m_aWdfcKey;
	CArray<T_POSL_K, T_POSL_K>  m_aPoslKey;
	CArray<T_POSP_K, T_POSP_K>  m_aPospKey;
	CArray<T_FIBR_K, T_FIBR_K>  m_aFibrItemKey_TH;
	CArray<T_FIBW_K, T_FIBW_K>  m_aFibwItemKey_TH;
	CArray<T_FIBR_K, T_FIBR_K>  m_aFibrItemKey_PO;
	CArray<T_FIBW_K, T_FIBW_K>  m_aFibwItemKey_PO;
	//Fire Steel
	//CArray<T_FLUE_K, T_FLUE_K>  m_aFlueItemKey;
	//CArray<T_STTF_K, T_STTF_K>  m_aSttfItemKey;
	CArray<T_FIPA_K, T_FIPA_K>  m_aFipaItemKey[4];
	int nStldFirePos; //Only One For Stld Fire 
	CStringArray m_aMvcdStr;
	CArray<UINT, UINT> m_aMvcdData;
	CArray<int, int> m_aUsedPhgt;
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Member Var.
	BOOL      m_bDrag;     // Drag 여부 판단, TRUE==드래깅 중 
	BOOL      m_bDragPoss; // Drag 가능여부 판단, TRUE==드래깅 가능
	HTREEITEM m_hDragItem; // Drag한 tree Item의 handle 값

	HTREEITEM                     m_hRootTi;
	CArray<HTREEITEM, HTREEITEM>  m_arrSub1List;
	//////////////////////////////////////////////////////////////////////////////////////////

// Operations
public:
	//////////////////////////////////////////////////////////////////////////////////////////
	// Create / Destroy
	void CreateTree();
	void DestroyTree();
	void FreeWorkTreeBuffer();
	void FreeBoundaryInfo();

protected:
	COLORREF GetBkColor();
//   COLORREF GetBkColorLeft() const;
//   COLORREF GetBkColorRight() const;
//   COLORREF GetBkColorMask() const;
	COLORREF GetBkColorTitle();
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Use / UnUse
	int  IsUsedMatl(HTREEITEM hItem); // 1:사용됨, 0:사용안됨, -1:정의안됨
	int  IsUsedSect(HTREEITEM hItem); // 1:사용됨, 0:사용안됨, -1:정의안됨
	int  IsUsedThik(HTREEITEM hItem); // 1:사용됨, 0:사용안됨, -1:정의안됨
	int  IsUsedSecv(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedSpfc(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedThfc(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedLlan(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedLlanch(HTREEITEM hItem);// 1:사용됨, 0:사용안됨
	BOOL IsUsedLlanid(HTREEITEM hItem);// 1:사용됨, 0:사용안됨
	BOOL IsUsedLlanfr(HTREEITEM hItem);// 1:사용됨, 0:사용안됨
	BOOL IsUsedLlanjp(HTREEITEM hItem);// 1:사용됨, 0:사용안됨
	BOOL IsUsedLlantr(HTREEITEM hItem);// 1:사용됨, 0:사용안됨
	BOOL IsUsedLlanop(HTREEITEM hItem);// 1:사용됨, 0:사용안됨
	BOOL IsUsedSlan(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedSlanch(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedSlanfr(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedSlanop(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedMvhl(HTREEITEM hItem, int nMovingLoadCode); // 1:사용됨, 0:사용안됨
	BOOL IsUsedMvhc(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedMvhlch(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedMvhlid(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedMvhlfr(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedMvhltr(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedMvhcch(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedSmpt(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedGrup(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedTdmt(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedEpmt(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedTdme(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedTdmf(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedTdnt(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedNllp(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedFimp(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedFibr(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedFibw(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedFimp_PO(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedFibr_PO(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedFibw_PO(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedIehp(HTREEITEM hItem); // 1:사용됨, 0:사용안됨 
	BOOL IsUsedPhgt(HTREEITEM hItem, BOOL bUpdateStatus=FALSE); // 1:사용됨, 0:사용안됨 , bUpdateStatus=TRUE 이면 업데이트만 함
	BOOL IsUsedImpf(HTREEITEM hItem); // 1:사용됨, 0:사용안됨      // MNET:2812-JWLEE-20070626
	BOOL IsUsedDynf(HTREEITEM hItem); // 1:사용됨, 0:사용안됨
	BOOL IsUsedDimm(HTREEITEM hItem); // 1:사용됨, 0:사용안됨      
	BOOL IsUsedAcop(UINT Data);       // 1:사용됨, 0:사용안됨   
	BOOL IsUsedRebb(HTREEITEM hItem);
	BOOL IsUsedRebc(HTREEITEM hItem);
	BOOL IsUsedRebr(HTREEITEM hItem);
	BOOL IsUsedRebw(HTREEITEM hItem);
	BOOL IsUsedRbms(HTREEITEM hItem);
	BOOL IsUsedRbsl(HTREEITEM hItem);
	BOOL IsUsedRbmw(HTREEITEM hItem);  
	BOOL IsUsedRbst(HTREEITEM hItem);
	BOOL IsUsedRipb(HTREEITEM hItem);
	BOOL IsUsedRipc(HTREEITEM hItem);
	BOOL IsUsedPreb(HTREEITEM hItem);
	BOOL IsUsedPrcs(HTREEITEM hItem);
	BOOL IsUsedDpan(HTREEITEM hItem);
	BOOL IsUsedCcpt(HTREEITEM hItem);
	BOOL IsUsedMsis(HTREEITEM hItem);
	BOOL IsUsedSdvi(HTREEITEM hItem);
	BOOL IsUsedSdve(HTREEITEM hItem);
	BOOL IsUsedSdst(HTREEITEM hItem);
	BOOL IsUsedSdhy(HTREEITEM hItem);
	BOOL IsUsedSdis(HTREEITEM hItem);
	BOOL IsUsedMhtp(HTREEITEM hItem); 
	BOOL IsUsedAsgb(HTREEITEM hItem);
	BOOL IsUsedDmgb(HTREEITEM hItem);
	BOOL IsUsedRfst(HTREEITEM hItem);
	BOOL GetSelectedRbmsKey(HTREEITEM& curHt, int pos, T_RBMS_K& RbmsK);
	BOOL GetSelectedRbstKey(HTREEITEM& curHt, int pos, T_RBST_K& RbstK);
	BOOL GetSelectedRbslKey(HTREEITEM& curHt, int pos, T_RBSL_K& RbslK);
	BOOL GetSelectedRipbKey(HTREEITEM& curHt, int pos, T_RIPB_K& RipbK);
	BOOL GetSelectedRipcKey(HTREEITEM& curHt, int pos, T_RIPC_K& RipcK);


	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Set Tree Item
	void SetItemTree(BOOL bInit=TRUE);
	BOOL IsSameNlnkData(CDBDoc* pDoc, T_NLNK_D &Data1, T_NLNK_D &Data2);
	BOOL IsSameGimpData(CDBDoc* pDoc, T_GIMP_D &Data1, T_GIMP_D &Data2);
	BOOL IsSameJimpData(CDBDoc* pDoc, T_JIMP_D& Data1, T_JIMP_D& Data2);
	BOOL IsSameLnkfData(CDBDoc* pDoc, T_LNKF_D &Data1, T_LNKF_D &Data2);

	HTREEITEM SetGeometryItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetGroupItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetNamedPlaneItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetPropertyItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetInelasticPropertyItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetBoundaryItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetMassItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetStaticLoadsItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetStaticLoadsTypeItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetStaticCnldTypeLoadsItemTree(HTREEITEM hTI);
	HTREEITEM SetEarthPressureParamItemTree(CDBDoc* pDoc, BOOL bInit = TRUE);
	HTREEITEM SetResponseSpectrumItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetTimeHistoryItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetCraneLoadItemTree(CDBDoc* pDoc, BOOL bInit=TRUE);
	HTREEITEM SetMovingLoadItemTree(CDBDoc* pDoc, BOOL bInit=TRUE);
	HTREEITEM SetMovingLoadogItemTree(CDBDoc* pDoc, int nCode, BOOL bInit=TRUE);
	HTREEITEM SetMovingLoadjpItemTree(CDBDoc* pDoc, int nCode, BOOL bInit=TRUE);
	HTREEITEM SetMovingLoadjpItemTree_Old(CDBDoc* pDoc, int nCode, BOOL bInit = TRUE);
	HTREEITEM SetMovingLoadchItemTree(CDBDoc* pDoc, int nCode, BOOL bInit=TRUE);
	HTREEITEM SetMovingLoadidItemTree(CDBDoc* pDoc, int nCode, BOOL bInit=TRUE);
	HTREEITEM SetMovingLoadbsItemTree(CDBDoc* pDoc, int nCode, BOOL bInit=TRUE);
	HTREEITEM SetMovingLoadfrItemTree(CDBDoc* pDoc, int nCode, BOOL bInit = TRUE);
	HTREEITEM SetMovingLoadEurobsItemTree(CDBDoc* pDoc, int nCode, BOOL bInit=TRUE);
	HTREEITEM SetMovingLoadtrItemTree(CDBDoc* pDoc, int nCode, BOOL bInit=TRUE);
	HTREEITEM SetSettlementItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetConCurrentReactionItemTree(CDBDoc* pDoc, BOOL bInit = TRUE);
	HTREEITEM SetConCurrentJointFoeceItemTree(CDBDoc* pDoc, BOOL bInit = TRUE);
	HTREEITEM SetFireSteelItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	//	HTREEITEM SetErectionSequenceItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetCompositeBridgeItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM GetPreviousItemToInsertAfter(UINT insItem);
	HTREEITEM SetSub1TreeItem(UINT insItem,BOOL bInit,int nCount,UINT imageID);
	HTREEITEM SetHydrationItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetStageItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetTendonItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetAnalysisItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetGridModelItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetPushoverItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetDimensionItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetTracingItemTree(CDBDoc* pDoc, BOOL bInit=TRUE);
	HTREEITEM SetRebarDataItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	void SetRebarRebbDataItemTree(CDBDoc* pDoc, int nRebbCount, CArray<T_REBB_K, T_REBB_K>& aRebbK, HTREEITEM& hTI);
	void SetRebarRebcDataItemTree(CDBDoc* pDoc, int nRebcCount, CArray<T_REBC_K, T_REBC_K>& aRebcK, HTREEITEM& hTI);
	void SetRebarRebrDataItemTree(CDBDoc* pDoc, int nRebrCount, CArray<T_REBR_K, T_REBR_K>& aRebrK, HTREEITEM& hTI);
	void SetRebarRebwDataItemTree(CDBDoc* pDoc, int nRebwCount, CArray<T_REBW_K, T_REBW_K>& aRebwK, HTREEITEM& hTI);
	void SetRebarRbmsDataItemTree(CDBDoc* pDoc, int nRbmsCount, CArray<T_RBMS_K, T_RBMS_K>& aRbmsK, HTREEITEM& hTI);
	void SetRebarRbmwDataItemTree(CDBDoc* pDoc, int nRbmwCount, CArray<T_RBMW_K, T_RBMW_K>& aRbmwK, HTREEITEM& hTI);
	void SetRebarRbstDataItemTree(CDBDoc* pDoc, int nRbstCount, CArray<T_RBST_K, T_RBST_K>& aRbstK, HTREEITEM& hTI);
	void SetRebarRbslDataItemTree(CDBDoc* pDoc, int nRbslCount, CArray<T_RBSL_K, T_RBSL_K>& aRbslK, HTREEITEM& hTI);
	void SetRebarRipbDataItemTree(CDBDoc* pDoc, int nRipbCount, CArray<T_RIPB_K, T_RIPB_K>& aRipbK, HTREEITEM& hTI);
	void SetRebarRipcDataItemTree(CDBDoc* pDoc, int nRipcCount, CArray<T_RIPC_K, T_RIPC_K>& aRipcK, HTREEITEM& hTI);
	HTREEITEM SetDomainDataItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetDomainDataItemTree_Test(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetEBeamDataItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetStripDataItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetDropPanelDataItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetColmCapitalDataItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	HTREEITEM SetInfillStrutDataItemTree(CDBDoc* pDoc,BOOL bInit=TRUE);
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Active / Inactive Item
	void ActiveEntity(UINT data);
	void ActiveEntityPlus(UINT data);
	void InactiveEntity(UINT data);
	//////////////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////////////////
	// Edit Property (Stack Overflow 문제 때문에 분할)
	void EditProperty(HTREEITEM ht);
	void EditProperty_AnalysisControl(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_Structure(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_NamedPlane(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_Property(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_InelasticProperty(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_Boundary(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_Mass(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_StaticLoad(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_ResponseSpectrum(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_Tendon(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_TimeHistoryLoad(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_MovingLoad(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_MovingLoad_Common(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_MovingLoad_Vehicle(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);

	void DoModalUsr(T_MVHL_D& mvhlData);
	void DoModalUsrIndia(T_MVHL_D& mvhlData);
	void DoModalUsrSouthAfrica(T_MVHL_D& mvhlData);
	void DoModalUsrPoland(T_MVHL_D& mvhlData);
	void DoModalUsrAustralia(T_MVHL_D& mvhlData);
	void DoModalUsrKRLRFD11(T_MVHL_D& mvhlData);
	void DoModalUsrRus(T_MVHL_D& mvhlData);
	void DoModalUserEuroBSNew(T_MVHL_D& mvhlData);
	void DoModalUsrJpRail(T_MVHL_D& mvhlData);
	void DoModalUsrNewZealand(T_MVHL_D& mvhlData);
	void DoModalUsrBrazil(T_MVHL_D& mvhlData);

	void DoModalStd(T_MVHL_D& mvhlData);
	void DoModalStdFR(T_MVHL_D& mvhlData);
	void DoModalStdBS(T_MVHL_D& mvhlData);
	void DoModalStdIndia(T_MVHL_D& mvhlData);
	void DoModalStdSouthAfrica(T_MVHL_D& mvhlData);
	void DoModalStdPoland(T_MVHL_D& mvhlData);
	void DoModalStdAustralia(T_MVHL_D& mvhlData);
	void DoModalStdNewZealand(T_MVHL_D& mvhlData);
	void DoModalStdBrazil(T_MVHL_D& mvhlData);
	void DoModalStdKRLRFD11(T_MVHL_D& mvhlData);
	void DoModalStdRus(T_MVHL_D& mvhlData);
	void DoModalStdEuroBSnew(T_MVHL_D& mvhlData);
	void DoModalStdJpRail(T_MVHL_D& mvhlData);

	void EditProperty_Settlement(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_Craneload(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_CompositeBridge(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_Hydration(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_ConstructionStage(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_GridAnalysisModel(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_Pushover(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_Pushover_Hinge(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_MembAssignment(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_DynamicReportFigure(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos){};
	void EditProperty_RebarData(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_WindVelocity(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_WindFunction(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_Flue(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_Sttf(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_CnldType(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_SeismicLoad(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_SoilProperty(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	//////////////////////////////////////////////////////////////////////////////////////////
	
	void OpenTables(HTREEITEM ht);
	
	//////////////////////////////////////////////////////////////////////////////////////////
	// Delete Item
	void DeleteItemEntity(CArray<HTREEITEM, HTREEITEM> &aItem);
	BOOL DeleteStaticLoad(CDBDoc* pDoc,CArray<HTREEITEM, HTREEITEM> &aItem);
	BOOL DeleteItemEntity_TimeHistory(CDBDoc* pDoc, UINT data);
	BOOL DeleteItemEntity_GridAnalModel(CDBDoc* pDoc, UINT data);
	BOOL DeleteCnldType(CDBDoc* pDoc,CArray<HTREEITEM, HTREEITEM> &aItem);
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Assign Attribute
	void EndDrag(CPoint point);
	void AssignAttribute(HTREEITEM ht,CArray<T_NODE_K, T_NODE_K> &aNodeKeyList, CArray<T_ELEM_K, T_ELEM_K> &aElemKeyList);
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Display / Disappear
	void DisplayStaticLoad(HTREEITEM ht,BOOL fg=1); // fg==0 : 하중조건을 나타내는 아이템, fg==1 : 하중조건 안의 하중종류를 나타내는 아이템
	void DisplayEtcLoad(HTREEITEM ht);
	void DisplayBoundaries(HTREEITEM ht);
	void DisplayMasses(HTREEITEM ht);
	void DisplayDynamicLoad(HTREEITEM ht);
	void DisplayMovingLoad(HTREEITEM ht);
	void DisplayMovingLoadjp(HTREEITEM ht);
	void DisplayMovingLoadch(HTREEITEM ht);
	void DisplayMovingLoadid(HTREEITEM ht);
	void DisplayMovingLoadfr(HTREEITEM ht);
	void DisplayMovingLoadtr(HTREEITEM ht);
	void DisplayMovingLoadOp(HTREEITEM ht);
	void DisplaySettlement(HTREEITEM ht);
	void DisplayTendonProfile(HTREEITEM ht);
	void DisplayPTBTendonProfile(HTREEITEM ht);
	void DisplayPTSTendonProfile(HTREEITEM ht);
	void DisplayHydration(HTREEITEM ht);
	void DisplayGridAnalModel(HTREEITEM ht);
	void DisplayPushover(HTREEITEM ht);
	void DisplayDimension(HTREEITEM ht);
	void DisplayStoryTracingAndLineGrid(HTREEITEM ht);
	void DisplayStoryTracingAndLineGridAll(HTREEITEM ht);
	void DisplayStoryTracingAndLineGrid(const CArray<T_STOR_K, T_STOR_K>& akey);
	void DisplayStor(HTREEITEM ht);
	void DisplayPrcs(HTREEITEM ht);
	void DisplayDpan(HTREEITEM ht);
	void DisplayCcpt(HTREEITEM ht);
	void DisplayMsis(HTREEITEM ht);
	void DIsplayPsll(HTREEITEM ht);
	void DisplayFireLoad(HTREEITEM ht);
	void DisplayCnldType(HTREEITEM ht);
	void DisplayDstr(HTREEITEM ht);
	void DisplayTdln(HTREEITEM ht);
	void DisplaySptl(HTREEITEM ht);

	void DisappearStaticLoad(HTREEITEM ht,BOOL fg=1); // fg==0 : 하중조건을 나타내는 아이템, fg==1 : 하중조건 안의 하중종류를 나타내는 아이템
	void DisappearEtcLoad(HTREEITEM ht);
	void DisappearBoundaries(HTREEITEM ht);
	void DisappearMasses(HTREEITEM ht);
	void DisappearDynamicLoad(HTREEITEM ht);
	void DisappearMovingLoad(HTREEITEM ht);
	void DisappearMovingLoadjp(HTREEITEM ht);
	void DisappearMovingLoadid(HTREEITEM ht);
	void DisappearMovingLoadfr(HTREEITEM ht);
	void DisappearMovingLoadch(HTREEITEM ht);
	void DisappearMovingLoadtr(HTREEITEM ht);
	void DisappearMovingLoadop(HTREEITEM ht);
	void DisappearSettlement(HTREEITEM ht);
	void DisappearTendonProfile(HTREEITEM ht);
	void DisappearPTBTendonProfile(HTREEITEM ht);
	void DisappearPTSTendonProfile(HTREEITEM ht);
	void DisappearHydration(HTREEITEM ht);
	void DisappearGridAnalModel(HTREEITEM ht);
	void DisappearPushover(HTREEITEM ht);
	void DisappearDimension(HTREEITEM ht);
	void DisappearStoryTracingAndLineGrid(HTREEITEM ht);
	void DisappearStoryTracingAndLineGridAll(HTREEITEM ht);
	void DisappearStoryTracingAndLineGrid(const CArray<T_STOR_K, T_STOR_K>& akey);
	void DisappearStor(HTREEITEM ht);
	void DisappearPrcs(HTREEITEM ht);
	void DisappearDpan(HTREEITEM ht);
	void DisappearCcpt(HTREEITEM ht);
	void DisappearMsis(HTREEITEM ht);
	void DisappearPsll(HTREEITEM ht);
	void DisappearFireLoad(HTREEITEM ht);
	void DisappearCnldType(HTREEITEM ht);
	void DisappearDstr(HTREEITEM ht);
	void DisappearTdln(HTREEITEM ht);
	void DisappearSptl(HTREEITEM ht);
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Select / Unselect
	BOOL GetSelectedItemKeyList(UINT data,CArray<T_NODE_K, T_NODE_K> &rNode,CArray<T_ELEM_K, T_ELEM_K> &rElem);

	BOOL SelectEntity(UINT data,BOOL bReplace=TRUE);
	void UnselectEntity(UINT data);

	int GetSelectedGrupKeyListWithMultiTreeItemSelect(CArray<T_GRUP_K, T_GRUP_K> &aKey);
	int GetSelectedElemKeyListWithMultiTreeItemSelect(CArray<T_ELEM_K, T_ELEM_K> &aKey);
	int GetSelectedStorKeyListWithMultiTreeItemSelect(CArray<T_STOR_K, T_STOR_K> &aKey);
	int GetSelectedNplnKeyListWithMultiTreeItemSelect(CArray<T_NPLN_K, T_NPLN_K> &aKey);
	int GetSelectedMatlKeyListWithMultiTreeItemSelect(CArray<T_MATL_K, T_MATL_K> &aKey);
	int GetSelectedSectKeyListWithMultiTreeItemSelect(CArray<T_SECT_K, T_SECT_K> &aKey);
	int GetSelectedSecvKeyListWithMultiTreeItemSelect(CArray<T_SECV_K, T_SECV_K>& aKey);
	int GetSelectedCscsKeyListWithMultiTreeItemSelect(CArray<T_CSCS_K, T_CSCS_K> &aKey);
	void SortCscsKey(CDBDoc* pDoc, CArray<T_CSCS_K, T_CSCS_K>& raCscsK);
	int GetSelectedThikKeyListWithMultiTreeItemSelect(CArray<T_THIK_K, T_THIK_K> &aKey);
	int GetSelectedMphgKeyListWithMultiTreeItemSelect(CArray<T_MPHG_K, T_MPHG_K> &aKey);
	int GetSelectedMpstKeyListWithMultiTreeItemSelect(CArray<T_MPST_K, T_MPST_K> &aKey);
	int GetSelectedConsDataListWithMultiTreeItemSelect(CArray<T_CONS_D, T_CONS_D> &aKey);
	int GetSelectedConsKeyListWithMultiTreeItemSelect(CArray<T_CONS_K, T_CONS_K> &aKey);

	int GetSelectedNsprLinearDataListWithMultiTreeItemSelect(CArray<T_NSPR_D, T_NSPR_D> &aKey);
	int GetSelectedNsprLinearKeyListWithMultiTreeItemSelect(CArray<T_NSPR_K, T_NSPR_K> &aKey);

	int GetSelectedNsprNonDataListWithMultiTreeItemSelect(CArray<T_NSPR_D, T_NSPR_D> &aKey);
	int GetSelectedNsprNonKeyListWithMultiTreeItemSelect(CArray<T_NSPR_K, T_NSPR_K> &aKey);

	int GetSelectedNsprMultiDataListWithMultiTreeItemSelect(CArray<T_NSPR_D, T_NSPR_D> &aKey);  
	int GetSelectedNsprMultiKeyListWithMultiTreeItemSelect(CArray<T_NSPR_K, T_NSPR_K> &aKey);

	int GetSelectedCranKeyListWithMultiTreeItemSelect(CArray<T_CRAN_K, T_CRAN_K> &aKey);

	int GetSelectedGstpKeyListWithMultiTreeItemSelect(CArray<T_GSTP_K, T_GSTP_K> &aKey);
	int GetSelectedGsprKeyListWithMultiTreeItemSelect(CArray<T_GSPR_K, T_GSPR_K> &aKey);

	int GetSelectedSspsLinearDataListWithMultiTreeItemSelect(CArray<T_SSPS_D, T_SSPS_D> &aKey);
	int GetSelectedSspsLinearKeyListWithMultiTreeItemSelect(CArray<T_SSPS_K, T_SSPS_K> &aKey);

	int GetSelectedSspsNonDataListWithMultiTreeItemSelect(CArray<T_SSPS_D, T_SSPS_D> &aKey);
	int GetSelectedSspsNonKeyListWithMultiTreeItemSelect(CArray<T_SSPS_K, T_SSPS_K> &aKey);

	int GetSelectedFrlsDataListWithMultiTreeItemSelect(CArray<T_FRLS_D, T_FRLS_D> &aKey);
	int GetSelectedFrlsKeyListWithMultiTreeItemSelect(CArray<T_FRLS_K, T_FRLS_K> &aKey);
	int GetSelectedOffsDataListWithMultiTreeItemSelect(CArray<T_OFFS_D, T_OFFS_D> &aKey);
	int GetSelectedOffsKeyListWithMultiTreeItemSelect(CArray<T_OFFS_K, T_OFFS_K> &aKey);
	int GetSelectedPrlsDataListWithMultiTreeItemSelect(CArray<T_PRLS_D, T_PRLS_D> &aKey);
	int GetSelectedPrlsKeyListWithMultiTreeItemSelect(CArray<T_PRLS_K, T_PRLS_K> &aKey);
	int GetSelectedRigdKeyListWithMultiTreeItemSelect(CArray<T_RIGD_K, T_RIGD_K> &aKey);
	int GetSelectedMconKeyListWithMultiTreeItemSelect(CArray<T_MCON_K, T_MCON_K> &aKey); // MNET:2790-JHLEE-20070911
	int GetSelectedSkewDataListWithMultiTreeItemSelect(CArray<T_SKEW_D, T_SKEW_D> &aKey);
	int GetSelectedSkewKeyListWithMultiTreeItemSelect(CArray<T_SKEW_K, T_SKEW_K> &aKey);
	int GetSelectedCldrKeyListWithMultiTreeItemSelect(CArray<T_CLDR_K, T_CLDR_K> &aKey);
	int GetSelectedNmasDataListWithMultiTreeItemSelect(CArray<T_NMAS_D, T_NMAS_D> &aKey);
	int GetSelectedCglpKeyListWithMultiTreeItemSelect(CArray<T_CGLP_K, T_CGLP_K>& aKey);
	int GetSelectedNmasKeyListWithMultiTreeItemSelect(CArray<T_NMAS_K, T_NMAS_K> &aKey);
	int GetSelectedGmasDataListWithMultiTreeItemSelect(CArray<T_GMAS_D, T_GMAS_D> &aData);
	int GetSelectedGmasKeyListWithMultiTreeItemSelect(CArray<T_GMAS_K, T_GMAS_K> &aKey);
	int GetSelectedSpfcNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedSplcNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedThnlDataListWithMultiTreeItemSelect(CArray<T_THNL_D, T_THNL_D> &aKey);
	int GetSelectedThnlKeyListWithMultiTreeItemSelect(CArray<T_THNL_K, T_THNL_K> &aKey);
	int GetSelectedThfcNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedThgaKeyListWithMultiTreeItemSelect(CArray<T_THGA_K, T_THGA_K> &aKey);
	int GetSelectedThmsDataListWithMultiTreeItemSelect(CArray<T_THMS_D, T_THMS_D> &aKey);
	int GetSelectedThmsKeyListWithMultiTreeItemSelect(CArray<T_THMS_K, T_THMS_K> &aKey);
	int GetSelectedThisNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedThatKeyListWithMultiTreeItemSelect(CArray<T_THAT_K, T_THAT_K> &aKey);
	int GetSelectedLlanKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedLlanjpKeyListWithMultiTreeItemSelect(CArray<CString, CString&>& aName);
	int GetSelectedLlanchKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedLlanidKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedLlanfrKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedLlantrKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedLlanopKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedSlanKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedSlanchKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedSlanfrKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedSlanopKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedMvhlKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedMvhltrKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedMvhcKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedMvldKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedMvldjpKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedMvldchKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedMvldidKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedMvldbsKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedMvldfrKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedMvldtrKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedSmptKeyListWithMultiTreeItemSelect(CArray<T_SMPT_K, T_SMPT_K> &aKey);
	int GetSelectedSmptNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedSmlcNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedHmtpNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedHmatKeyListWithMultiTreeItemSelect(CArray<T_HMAT_K, T_HMAT_K> &aKey);
	int GetSelectedEtfcNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedCcfcNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedHecbKeyListWithMultiTreeItemSelect(CArray<T_HECB_K, T_HECB_K> &aKey);
	int GetSelectedHsptKeyListWithMultiTreeItemSelect(CArray<T_HSPT_K, T_HSPT_K> &aKey);
	int GetSelectedHahsKeyListWithMultiTreeItemSelect(CArray<T_HAHS_K, T_HAHS_K> &aKey);
	int GetSelectedHsfcKeyListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedHpceNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedHpcDataListWithMultiTreeItemSelect(CArray<T_HPCE_D, T_HPCE_D> &aData);
	int GetSelectedTdmtNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedTdmeNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedTdmfNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedSecfKeyListWithMultiTreeItemSelect(CArray<T_SECF_K, T_SECF_K> &aKey);
	int GetSelectedRpscKeyListWithMultiTreeItemSelect(CArray<T_RPSC_K, T_RPSC_K> &aKey);
	int GetSelectedEfwdKeyListWithMultiTreeItemSelect(CArray<T_EFWD_K, T_EFWD_K> &aKey);
	int GetSelectedCrgrGrupDataWithMultiTreeItemSelect(T_CRGR_D& CrgrD, CArray<T_NODE_K, T_NODE_K> &rNode);
	int GetSelectedCjfgGrupDataWithMultiTreeItemSelect(T_CJFG_D& CjfgD, CArray<T_NODE_K, T_NODE_K> &rNode);
	int GetSelectedTmatKeyListWithMultiTreeItemSelect(CArray<T_TMAT_K, T_TMAT_K> &aKey);
	int GetSelecteMhtpNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedMhatKeyListWithMultiTreeItemSelect(CArray<T_MHAT_K, T_MHAT_K> &aKey);	//Add by tss 2019.9
	int GetSelectedEpmtNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedTsgrNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedTdntNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedTdnaNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedTdbmNameListWithMultiTreeItemSelect(CArray<T_TDBM_K, T_TDBM_K> &aKey);
	int GetSelectedPttnNameListWithMultiTreeItemSelect(CArray<T_PTTN_K, T_PTTN_K> &aKey);
	int GetSelectedTdcsKeyListWithMultiTreeItemSelect(CArray<T_TDCS_K, T_TDCS_K> &aKey);
	int GetSelectedNlnkKeyListWithMultiTreeItemSelect(CArray<T_NLNK_K, T_NLNK_K> &aKey);
	int GetSelectedThslKeyListWithMultiTreeItemSelect(CArray<T_THSL_K, T_THSL_K> &aKey);
	int GetSelectedThslDataListWithMultiTreeItemSelect(CArray<T_THSL_D, T_THSL_D> &aKey);
	int GetSelectedNllpKeyListWithMultiTreeItemSelect(CArray<T_NLLP_K, T_NLLP_K> &aKey);
	int GetSelectedNllpNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedIehpNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedIehpKeyListWithMultiTreeItemSelect(CArray<T_IEHP_K, T_IEHP_K> &aKey);
	int GetSelectedIehgKeyListWithMultiTreeItemSelect(CArray<T_IEHG_K, T_IEHG_K> &aKey);
	int GetSelectedEssfKeyListWithMultiTreeItemSelect(CArray<T_ESSF_K, T_ESSF_K> &aKey);
	int GetSelectedWssfKeyListWithMultiTreeItemSelect(CArray<T_WSSF_K, T_WSSF_K> &aKey);
	int GetSelectedPssfKeyListWithMultiTreeItemSelect(CArray<T_PSSF_K, T_PSSF_K> &aKey);
	int GetSelectedMgdrNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedCbemNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedLdlnNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedBspnKeyListWithMultiTreeItemSelect(CArray<T_BSPN_K, T_BSPN_K> &aKey);
	int GetSelectedGimpDataListWithMultiTreeItemSelect(CArray<T_GIMP_D, T_GIMP_D> &aKey);
	int GetSelectedGimpKeyListWithMultiTreeItemSelect(CArray<T_GIMP_K, T_GIMP_K> &aKey);
	int GetSelectedJimpDataListWithMultiTreeItemSelect(CArray<T_JIMP_D, T_JIMP_D>& aKey);
	int GetSelectedJimpKeyListWithMultiTreeItemSelect(CArray<T_JIMP_K, T_JIMP_K>& aKey);
	int GetSelectedCrefNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedGilcNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedP1ltNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedP1laDataListWithMultiTreeItemSelect(CArray<T_P1LA_D, T_P1LA_D> &aKey);
	int GetSelectedP1laKeyListWithMultiTreeItemSelect(CArray<T_P1LA_K, T_P1LA_K> &aKey);
	int GetSelectedGstyDataListWithMultiTreeItemSelect(CArray<T_GSTY_D, T_GSTY_D> &aKey);
	int GetSelectedGstyKeyListWithMultiTreeItemSelect(CArray<T_GSTY_K, T_GSTY_K> &aKey);
	int GetSelectedLnkfDataListWithMultiTreeItemSelect(CArray<T_LNKF_D, T_LNKF_D> &aKey);
	int GetSelectedLnkfKeyListWithMultiTreeItemSelect(CArray<T_LNKF_K, T_LNKF_K> &aKey);
	int GetSelectedFimpNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedFibrNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName, int nAssignType);
	int GetSelectedFibwNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName, int nAssignType);
	int GetSelectedFibrKeyListWithMultiTreeItemSelect(CArray<T_FIBR_K, T_FIBR_K> &aKey, int nAssignType);
	int GetSelectedFibwKeyListWithMultiTreeItemSelect(CArray<T_FIBW_K, T_FIBW_K> &aKey, int nAssignType);
	int GetSelectedThrdKeyListWithMultiTreeItemSelect(CArray<T_THRD_K, T_THRD_K> &aKey);
	int GetSelectedThcrKeyListWithMultiTreeItemSelect(CArray<T_THCR_K, T_THCR_K> &aKey);
	int GetSelectedFlueKeyListWithMultiTreeItemSelect(CArray<T_FLUE_K, T_FLUE_K> &aKey);
	int GetSelectedSttfKeyListWithMultiTreeItemSelect(CArray<T_STTF_K, T_STTF_K> &aKey);
	int GetSelectedFipaKeyListWithMultiTreeItemSelect(CArray<T_FIPA_K, T_FIPA_K> &aKey);
	int GetSelectedPolcNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedPofcNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedPhgtNameListWithMultiTreeItemSelect(CArray<CString, CString&> &aName);
	int GetSelectedPhgtKeyListWithMultiTreeItemSelect(CArray<T_PHGT_K, T_PHGT_K> &aKey);
	int GetSelectedPhgeKeyListWithMultiTreeItemSelect(CArray<T_PHGE_K, T_PHGE_K> &aKey);
	int GetSelectedPordKeyListWithMultiTreeItemSelect(CArray<T_PORD_K, T_PORD_K> &aKey);
	int GetSelectedBcctBngrKeyListWithMultiTreeItemSelect(CArray<T_BNGR_K, T_BNGR_K> &aKey, CString& strSel);
	int GetSelectedSpanKeyListWithMultiTreeItemSelect(CArray<T_SPAN_K, T_SPAN_K> &aKey, CArray<T_ELEM_K, T_ELEM_K> &rElem, int nSectType, BOOL bVirtual=FALSE);
	int GetSelectedSpanKeyListWithMultiTreeItemSelect(CArray<T_SPAN_K, T_SPAN_K> &aKey, CArray<CString, CString&> &aStrName, CArray<int, int> &aSectType, BOOL bVirtual=FALSE);
	int GetSelectedDimmKeyListWithMultiTreeItemSelect(CArray<T_DIMM_K, T_DIMM_K> &aKey);
	int GetSelectedRebbKeyListWithMultiTreeItemSelect(CArray<T_REBB_K, T_REBB_K> &aKey);
	int GetSelectedRebcKeyListWithMultiTreeItemSelect(CArray<T_REBC_K, T_REBC_K> &aKey);
	int GetSelectedRebrKeyListWithMultiTreeItemSelect(CArray<T_REBR_K, T_REBR_K> &aKey);
	int GetSelectedRebwKeyListWithMultiTreeItemSelect(CArray<T_REBW_K, T_REBW_K> &aKey);
	int GetSelectedRbmsKeyListWithMultiTreeItemSelect(CArray<T_RBMS_K, T_RBMS_K> &aKey);
	int GetSelectedRbslKeyListWithMultiTreeItemSelect(CArray<T_RBSL_K, T_RBSL_K> &aKey);
	int GetSelectedRbmwKeyListWithMultiTreeItemSelect(CArray<T_RBMW_K, T_RBMW_K> &aKey);
	/**/int GetSelectedDomainKeyListWithMultiTreeItemSelect(std::vector<T_SBDO_K> &aKey);
	int GetSelectedMainDomainKeyListWithMultiTreeItemSelect(CArray<T_MADO_K, T_MADO_K> &aKey);
	int GetSelectedWvepKeyListWithMultiTreitemSelect(CArray<T_WVEP_K, T_WVEP_K>& rKey);
	int GetSelectedWdfcKeyListWithMultiTreitemSelect(CArray<T_WDFC_K, T_WDFC_K>& rKey);
	/**/T_MADO_K GetMadoKeyItemSbdo(HTREEITEM SbdoHt);
	int GetSelectedPrebKeyListWithMultiTreeItemSelect(CArray<T_PREB_K, T_PREB_K>& rKey);
	int GetSelectedPrcsKeyListWithMultiTreeItemSelect(CArray<T_PRCS_K, T_PRCS_K>& rKey);
	int GetSelectedDpanKeyListWithMultiTreeItemSelect(CArray<T_DPAN_K, T_DPAN_K>& aKey);
	int GetSelectedCcptKeyListWithMultiTreeItemSelect(CArray<T_CCPT_K, T_CCPT_K>& aKey);
	int GetSelectedMsisKeyListWithMultiTreeItemSelect(CArray<T_MSIS_K, T_MSIS_K>& aKey);
	int GetSelectedRbstKeyListWithMultiTreeItemSelect(CArray<T_RBST_K, T_RBST_K> &aKey);
	int GetSelectedRipbKeyListWithMultiTreeItemSelect(CArray<T_RIPB_K, T_RIPB_K> &aKey);
	int GetSelectedRipcKeyListWithMultiTreeItemSelect(CArray<T_RIPC_K, T_RIPC_K> &aKey);
	int GetSelectedElemKeyListByPrebWithMultiTreeItemSelect(CArray<T_ELEM_K,T_ELEM_K>& rKey);
	int GetSelectedElemKeyListByPrcsWithMultiTreeItemSelect(CArray<T_ELEM_K,T_ELEM_K>& rKey);
	int GetSelectedElemKeyListByDpanWithMultiTreeItemSelect(CArray<T_ELEM_K,T_ELEM_K>& aKey);
	int GetSelectedElemKeyListByCcptWithMultiTreeItemSelect(CArray<T_ELEM_K,T_ELEM_K>& aKey);
	int GetSelectedElemKeyListByMsisWithMultiTreeItemSelect(CArray<T_ELEM_K,T_ELEM_K>& aKey);
	int GetSelectedSdviKeyListWithMultiTreeItemSelect(CArray<T_SDVI_K, T_SDVI_K>& aKey);
	int GetSelectedSdveKeyListWithMultiTreeItemSelect(CArray<T_SDVE_K, T_SDVE_K>& aKey);
	int GetSelectedSdstKeyListWithMultiTreeItemSelect(CArray<T_SDST_K, T_SDST_K>& aKey);
	int GetSelectedSdhyKeyListWithMultiTreeItemSelect(CArray<T_SDHY_K, T_SDHY_K>& aKey);
	int GetSelectedSdisKeyListWithMultiTreeItemSelect(CArray<T_SDIS_K, T_SDIS_K>& aKey);
	int GetSelectedAsgbKeyListWithMultiTreeItemSelect(CArray<T_ASGB_K, T_ASGB_K>& aKey);
	int GetSelectedDmgbKeyListWithMultiTreeItemSelect(CArray<T_DMGB_K, T_DMGB_K>& aKey);
	int GetSelectedThrgKeyListWithMultiTreeItemSelect(CArray<T_THRG_K, T_THRG_K>& aKey);
	int GetSelectedThriKeyListWithMultiTreeItemSelect(CArray<T_THRI_K, T_THRI_K>& aKey);
	int GetSelectedThrsKeyListWithMultiTreeItemSelect(CArray<T_THRS_K, T_THRS_K>& aKey);
	int GetSelectedThefKeyListWithMultiTreeItemSelect(CArray<T_THEF_K, T_THEF_K>& aKey);
	int GetSelectedThsfKeyListWithMultiTreeItemSelect(CArray<T_THSF_K, T_THSF_K>& aKey);
	int GetSelectedPoefKeyListWithMultiTreeItemSelect(CArray<T_POEF_K, T_POEF_K>& aKey);
	int GetSelectedPorgKeyListWithMultiTreeItemSelect(CArray<T_PORG_K, T_PORG_K>& aKey);
	int GetSelectedPoreKeyListWithMultiTreeItemSelect(CArray<T_PORE_K, T_PORE_K>& aKey);
	int GetSelectedPoriKeyListWithMultiTreeItemSelect(CArray<T_PORI_K, T_PORI_K>& aKey);
	int GetSelectedMlfcKeyListWithMultiTreeItemSelect(CArray<T_MLFC_K, T_MLFC_K> &aKey);
	int GetSelectedRlfcKeyListWithMultiTreeItemSelect(CArray<T_RLFC_K, T_RLFC_K> &aKey);
	int GetSelectedPoslKeyListWithMultiTreeItemSelect(CArray<T_POSL_K, T_POSL_K> &aKey);
	int GetSelectedPospKeyListWithMultiTreeItemSelect(CArray<T_POSP_K, T_POSP_K> &aKey);
	int GetSelectedMlfcDataListWithMultiTreeItemSelect(CArray<T_MLFC_D, T_MLFC_D> &aKey);
	int GetSelectedRlfcDataListWithMultiTreeItemSelect(CArray<T_RLFC_D, T_RLFC_D> &aKey);
	int GetSelectedCnldKeyListWithMultiTreeItemSelect(CArray<T_CNLD_K, T_CNLD_K> &aKey);
	int GetSelectedBmldKeyListWithMultiTreeItemSelect(CArray<T_BMLD_K, T_BMLD_K> &aKey, int nBmldType);
	int GetSelectedPresKeyListWithMultiTreeItemSelect(CArray<T_PRES_K, T_PRES_K> &aKey);
	int GetSelectedNtmpKeyListWithMultiTreeItemSelect(CArray<T_NTMP_K, T_NTMP_K> &aKey);
	int GetSelectedEtmpKeyListWithMultiTreeItemSelect(CArray<T_ETMP_K, T_ETMP_K> &aKey);
	int GetSelectedSdspKeyListWithMultiTreeItemSelect(CArray<T_SDSP_K, T_SDSP_K> &aKey);
	int GetSelectedRfstKeyListWithMultiTreeItemSelect(CArray<T_SECT_K, T_SECT_K>& aKey);
	int GetSelectedTdasKeyListWithMultiTreeItemSelect(CArray<T_TDAS_K, T_TDAS_K>& aKey);
		//////////////////////////////////////////////////////////////////////////////////////////

	int GetSelectedCnldType_Elements(T_STLD_K stldK,int pos,CArray<T_CNLD_KEY, T_CNLD_KEY&>& aCnldKey,T_CNLD_D& cnld);
	T_STLD_K FindCnldType(HTREEITEM ht);
	//////////////////////////////////////////////////////////////////////////////////////////
	// ETC
	int GetLdlnSortedKeyList(CDBDoc* pDoc, CArray<T_LDLN_K, T_LDLN_K>& raLdlnK);

	int GetNodeElemKeyListByPhgt(CDBDoc* pDoc, CArray<T_PHGT_K, T_PHGT_K>& raPhgtK,
															 CArray<T_NODE_K, T_NODE_K>& aNodeK, CArray<T_ELEM_K, T_ELEM_K>& raElemK);

	int GetNodeElemKeyListByPhge(CDBDoc* pDoc, CArray<T_PHGE_K, T_PHGE_K>& raPhgeK,
															 CArray<T_NODE_K, T_NODE_K>& aNodeK, CArray<T_ELEM_K, T_ELEM_K>& raElemK);

	BOOL GetNodeElemKeyListByIehg(CDBDoc* pDoc, CArray<T_IEHG_K, T_IEHG_K>& raPhgeK,
															 CArray<T_NODE_K, T_NODE_K>& aNodeK, CArray<T_ELEM_K, T_ELEM_K>& raElemK);


	int GetNodeElemKeyListByBngr(CString strSel, CArray<T_BNGR_K, T_BNGR_K>& aBngrK,
															 CArray<T_NODE_K, T_NODE_K>& aNodeK, CArray<T_ELEM_K, T_ELEM_K>& aElemK);
	int GetConsKeyListByBngr(CArray<T_BNGR_K, T_BNGR_K>& aBngrK, CArray<T_CONS_K, T_CONS_K>& aConsK);
	int GetNsprKeyListByBngr(CArray<T_BNGR_K, T_BNGR_K>& aBngrK, CArray<T_NSPR_K, T_NSPR_K>& aNsprK);
	int GetGsprKeyListByBngr(CArray<T_BNGR_K, T_BNGR_K>& aBngrK, CArray<T_GSPR_K, T_GSPR_K>& aGsprK);
	int GetSspsKeyListByBngr(CArray<T_BNGR_K, T_BNGR_K>& aBngrK, CArray<T_SSPS_K, T_SSPS_K>& aSspsK);
	int GetElnkKeyListByBngr(CArray<T_BNGR_K, T_BNGR_K>& aBngrK, CArray<T_ELNK_K, T_ELNK_K>& aElnkK);
	int GetNlnkKeyListByBngr(CArray<T_BNGR_K, T_BNGR_K>& aBngrK, CArray<T_NLNK_K, T_NLNK_K>& aNlnkK);
	//////////////////////////////////////////////////////////////////////////////////////////

	void SetDimensionDisplayText(BOOL bShow);
	void SetTracingAndLineGridDisplayText(bool bShow);


	//////////////////////////////////////////////////////////////////////////////////////////
	// Stack Overflow 문제 때문에 분할
	BOOL InsertTreeItemPogd(CDBDoc* pDoc, HTREEITEM hTI);
	BOOL InsertTreeItemPOMembAssign(CDBDoc* pDoc, HTREEITEM hTI);
	BOOL InsertTreeItemPolc(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_POLC_K, T_POLC_K>& aPolcK);
	BOOL InsertTreeItemPofc(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_POFC_K, T_POFC_K>& aPofcK);
	BOOL InsertTreeItemPhgt(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_PHGT_K, T_PHGT_K>& aPhgtK);
	BOOL InsertTreeItemPhge(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_PHGE_K, T_PHGE_K>& aPhgeK);
	BOOL InsertTreeItemPord(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_PORD_K, T_PORD_K>& aPordK);
	BOOL InsertTreeItemPO_Smart_Graph(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_POEF_K, T_POEF_K>& aPoefK, CArray<T_PORG_K, T_PORG_K>& aPorgK, CArray<T_PORE_K, T_PORE_K>& aPoreK, CArray<T_PORI_K, T_PORI_K>& aPoriK);

	BOOL GetPushoverhingetypeString(CDBDoc* pDoc, T_PHGT_K PhgtK, CString& strName);
	void GetElementByKind(CArray<UINT,UINT>& aKey, int nElemType);

	virtual BOOL IsNeedColorPalette(HTREEITEM hTreeItem);
	virtual COLORREF GetColorPalette(HTREEITEM hTreeItem);
	virtual void OnChangeColorPalette(HTREEITEM hTreeItem, COLORREF clr);

protected:
	COLORREF m_BkColor;
	//CString m_strBkColorNameOfDispOpt;
	CString m_strBkColorName;
	CString m_strBkTitleColorName;
	int m_nRPSCNum;

	CDC*       m_pMemDC;
	CRect      m_rcMemDC;
	CRect      m_rectEraseOld;    // 스크롤에 의해서 보고 있는 영역이 변경되었는지 여부 판단용

public:
	void SetBkGroundColor(COLORREF BkColor);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeWorkCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	//////////////////////////////////////////////////////////////////////////////////////////
	// DB Update
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	//////////////////////////////////////////////////////////////////////////////////////////

protected:
	int GetNonDissipativeListBySpfc(CDBDoc* pDoc, T_SPFC_K SpfcK, CArray<double, double>& aNonDissipative);

	// Generated message map functions
protected:
	virtual BOOL GetCustomColor(HTREEITEM hItem);
	void ToggleStyle(DWORD lStyleMask );
	void DrawBitmapNormally(CDC* pDC);
	void DrawStripeBackground(CDC* pDC, CRect& rect);

	//{{AFX_MSG(CTreeWorkCtrl)
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnWorktreeActive();
	afx_msg void OnWorktreeDelete();
	afx_msg void OnWorktreeInactive();
	afx_msg void OnWorktreeSelect();
	afx_msg void OnWorktreeUnselect();
	afx_msg void OnWorktreeUnselectall();
	afx_msg void OnWorktreeActiveall();
	afx_msg void OnWorktreeAssign();
	afx_msg void OnWorktreeProperties();
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnWorktreeAddmatl();
	afx_msg void OnWorktreeAddsect();
	afx_msg void OnWorktreeAddthik();
	afx_msg void OnWorktreeSelectplus();
	afx_msg void OnWorktreeDisplay();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnWorktreeDisappear();
	afx_msg void OnWorktreeDeleteUnusMatl();
	afx_msg void OnWorktreeDeleteUnusSect();
	afx_msg void OnWorktreeDeleteUnusThik();
	afx_msg void OnWorktreeActivePlus();
	afx_msg void OnWorktreeTables();
	afx_msg void OnWorktreeEnable();
	afx_msg void OnWorktreeDisable();
	afx_msg void OnWorktreePSCRebar();
	afx_msg void OnWorktreeExpandAll();
	afx_msg void OnWorktreeExpand1LEVEL();
	afx_msg void OnWorktreeExpand2LEVEL();
	afx_msg void OnWorktreePhgePropTable();
	afx_msg void OnWorktreePhgeAssignTable();
	afx_msg void OnWorktreePhgtDefine();
	afx_msg void OnWorktreeAddmado();
	afx_msg void OnWorktreeAddsbdo();
	afx_msg void OnWorktreeDisplayRelatedData();
	afx_msg void OnWorktreeDisappearRelatedData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEWORKCTRL_H__CE53B22D_F5CE_4C32_8EAD_1EB5F74EB88C__INCLUDED_)
