#pragma once
#include "DB_ST_DT_ELEM.h"

struct T_SECTSHAPE_OTHERINFO
{
	int m_nNode;
	double	m_NodePos[D_ELEM_MAXNOD][3];
	double	m_FrameLocalVector[3][3];
	double	m_dBetaAngle;
	bool    m_bGetOtherInfo;   // NodePos , FrameLocalVector등을 받아갈지 여부 
	bool	m_bApplyBetaAngle; // Beta Angle 적용한 것을 받아갈지 여부 
	

	T_SECTSHAPE_OTHERINFO()
	{
		m_nNode = 0;
		m_dBetaAngle = 0.0;
		m_bGetOtherInfo = false;
		m_bApplyBetaAngle = true;

		memset(m_NodePos, 0, sizeof(double)*D_ELEM_MAXNOD * 3);
		memset(m_FrameLocalVector, 0, sizeof(double) * 9);
		m_FrameLocalVector[0][0] = 1.0;
		m_FrameLocalVector[1][1] = 1.0;
		m_FrameLocalVector[2][2] = 1.0;
	}
};