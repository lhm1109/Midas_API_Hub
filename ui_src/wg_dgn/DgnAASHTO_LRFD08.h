// DgnAASHTO_LRFD08.h: interface for the CDgnAASHTO_LRFD08 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNAASHTO_LRFD08_H__INCLUDED_)
#define AFX_DGNAASHTO_LRFD08_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnPscCommon.h"

//#include "..\wg_db\DB_ST_DN.h"

//typedef BOOL (PDGNCALC)(void*, void*, void*, size_t, size_t, BOOL&);

struct _BDCY_AASHTO;
struct _CUMS_AASHTO;
struct _CRMS_AASHTO;
struct _BDCW_AASHTO;

class CDgnAASHTO_LRFD08 : public CDgnPscCommon
{
public:
	CDgnAASHTO_LRFD08();
	virtual ~CDgnAASHTO_LRFD08();

	void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl);
	BOOL InitialData();

	BOOL Get_FysgData(ElemPairK ElemK, T_FYSG_AASHTO& rData);
	BOOL Get_FysgData(ElemPairK ElemK, T_FYSG_D& rData);
	BOOL Get_FypcData(T_TDNA_K TdnaK, T_FYPC_AASHTO& rData);
	BOOL Get_FycmData(ElemPairK ElemK, T_FYCM_AASHTO& rData);
	BOOL Get_FssgData(ElemPairK ElemK, T_FPMT_JTG& rData);
	BOOL Get_FpmsData(ElemPairK ElemK, T_FPMS_JTG& rData);
	BOOL Get_TbarData(ElemPairK ElemK, T_TBAR_AASHTO& rData);
	BOOL Get_BdcyData(ElemPairK ElemK, T_BDCY_AASHTO& rData, _BDCY_AASHTO* pRes, int nPrtUnit=0);
	BOOL Get_CumsData(ElemPairK ElemK, T_CUMS_AASHTO& rData, _CUMS_AASHTO* pRes, int nPrtUnit=0);
	BOOL Get_CrmtData(ElemPairK ElemK, T_CRMT_AASHTO& rData, _CRMS_AASHTO* pRes, int nPrtUnit=0);
	BOOL Get_BdcwData(ElemPairK ElemK, T_BDCW_AASHTO& rData, _BDCW_AASHTO* pRes, int nPrtUnit=0);
	//For Composite
	BOOL Get_FysgData(int nGirder, ElemPairK ElemK, T_FYSG_D& rData);
	BOOL Get_FycmData(int nGirder, ElemPairK ElemK, T_FYCM_AASHTO& rData);


protected:

	CDgn_PSCManager* m_pPscManager;
	

};

#endif // !defined(AFX_DGNAASHTO_LRFD08_H__INCLUDED_)
