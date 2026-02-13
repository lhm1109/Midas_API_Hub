// DgnToolMPhi_SeismicDraw.h: interface for the CDgnToolMPhi_SeismicDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNTOOLMPHI_SEISMICDRAW_H__60DEE9D1_A686_46E4_A0B8_BEEF52DB0B31__INCLUDED_)
#define AFX_DGNTOOLMPHI_SEISMICDRAW_H__60DEE9D1_A686_46E4_A0B8_BEEF52DB0B31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnSeismicDesignStruct.h"

class CDgnToolMPhi_SeismicDraw  
{
public:
	CDgnToolMPhi_SeismicDraw();
	virtual ~CDgnToolMPhi_SeismicDraw();

public:
	BOOL MakeLateralCapaFig(_DGN_RESERVE_LATERAL_CAPA_RES& ResData);
	BOOL DeleteLateralCapaFig(_DGN_RESERVE_LATERAL_CAPA_RES& ResData);

};

#endif // !defined(AFX_DGNTOOLMPHI_SEISMICDRAW_H__60DEE9D1_A686_46E4_A0B8_BEEF52DB0B31__INCLUDED_)
