// Dgn_DLLDataConverter.h: interface for the CDgn_PSCManagerBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGN_DLLDATACONVERTER_H__)
#define __DGN_DLLDATACONVERTER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DB_ST_DN_RESULT_PSC.h"


struct _FYSG_PSC_LRFD_BASE;
struct _FYPC_AASHTO;
struct _FYCM_PSC_LRFD_BASE;
struct _FPMT_PSC_LRFD_BASE;
struct _BDCW_PSC_LRFD_BASE;
struct _TBAR_PSC_LRFD_BASE;
struct _BDCY_PSC_LRFD_BASE;
struct _CUMS_AASHTO_BASE;
struct _CRMS_AASHTO_BASE;

namespace Dgn_DLLDataConverter  
{
	//////////////////////////////////////////////////////////////////////////
	BOOL ConvertFysg_CivilToEngineAASHTO(const T_FYSG_BASE& InData, _FYSG_PSC_LRFD_BASE& rData);
	BOOL ConvertFypc_CivilToEngineAASHTO(const T_FYPC_AASHTO& InData,      _FYPC_AASHTO& rData);
	BOOL ConvertFycm_CivilToEngineAASHTO(const T_FYCM_AASHTO_BASE& InData, _FYCM_PSC_LRFD_BASE& rData);
	BOOL ConvertFssg_CivilToEngineAASHTO(const T_FPMT_JTG_BASE& InData,    _FPMT_PSC_LRFD_BASE& rData);
	BOOL ConvertFpms_CivilToEngineAASHTO(const T_FPMS_JTG_BASE& InData,    _FPMT_PSC_LRFD_BASE& rData);
	BOOL ConvertBdcw_CivilToEngineAASHTO(const T_BDCW_AASHTO_BASE& InData, _BDCW_PSC_LRFD_BASE& rData);
	BOOL ConvertTbar_CivilToEngineAASHTO(const T_TBAR_AASHTO_BASE& InData, _TBAR_PSC_LRFD_BASE& rData);
	BOOL ConvertBdcy_CivilToEngineAASHTO(const T_BDCY_AASHTO_BASE& InData, _BDCY_PSC_LRFD_BASE& rData);
	BOOL ConvertCums_CivilToEngineAASHTO(const T_CUMS_AASHTO_BASE& InData, _CUMS_AASHTO_BASE& rData);
	BOOL ConvertCrmt_CivilToEngineAASHTO(const T_CRMT_AASHTO_BASE& InData, _CRMS_AASHTO_BASE& rData);

	//////////////////////////////////////////////////////////////////////////
	BOOL ConvertFysg_EngineToCVL(const _FYSG_PSC_LRFD_BASE& InData, T_FYSG_BASE& rData);
	BOOL ConvertFypc_EngineToCVL(const _FYPC_AASHTO& InData, T_FYPC_AASHTO& rData);
	BOOL ConvertFycm_EngineToCVL(const _FYCM_PSC_LRFD_BASE& InData, T_FYCM_AASHTO_BASE& rData);
	BOOL ConvertFssg_EngineToCVL(const _FPMT_PSC_LRFD_BASE& InData, T_FPMT_JTG_BASE& rData);
	BOOL ConvertFpms_EngineToCVL(const _FPMT_PSC_LRFD_BASE& InData, T_FPMS_JTG_BASE& rData);
	BOOL ConvertBdcw_EngineToCVL(const _BDCW_PSC_LRFD_BASE& InData, T_BDCW_AASHTO_BASE& rData);
	BOOL ConvertTbar_EngineToCVL(const _TBAR_PSC_LRFD_BASE& InData, T_TBAR_AASHTO_BASE& rData);
	BOOL ConvertBdcy_EngineToCVL(const _BDCY_PSC_LRFD_BASE& InData, T_BDCY_AASHTO_BASE& rData);
	BOOL ConvertCums_EngineToCVL(const _CUMS_AASHTO_BASE& InData, T_CUMS_AASHTO_BASE& rData);
	BOOL ConvertCrmt_EngineToCVL(const _CRMS_AASHTO_BASE& InData, T_CRMT_AASHTO_BASE& rData);
	
	//////////////////////////////////////////////////////////////////////////  
	BOOL ConvertFycm_ToEngineAASHTO(const T_FYCM_D& InData, T_FYCM_AASHTO& rData);
	BOOL ConvertFycm_ToCivilData(const T_FYCM_AASHTO& InData, T_FYCM_D& rData);
}

#endif // !defined(__DGN_DLLDATACONVERTER_H__)
