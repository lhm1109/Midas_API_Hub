#pragma once

class CDBDoc;
struct T_INELASTICDYN_HINGE_INFO;
class CHingeResultTool
{
public:
	CHingeResultTool(void);
	virtual ~CHingeResultTool(void);

public:	
	double GetPOHingeThetamaxAbsMax(T_ELEM_K ElemK, int nDOF, T_POLC_K PolcK, int nStep, int nAddStepIdx);
	double GetTHHingeThetamaxAbsMax(T_ELEM_K ElemK, int nDOF, T_THIS_K ThisK, int nHingeTimeInex);

	bool GetPOHingeThetamaxByPos(T_ELEM_K ElemK, int nDOF, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT double raThetamax[2]);
	bool GetTHHingeThetamaxByPos(T_ELEM_K ElemK, int nDOF, T_THIS_K ThisK, int nHingeTimeInex, OUT double raThetamax[2]);

private:
	bool GetPOInelstHngeInfo(T_ELEM_K ElemK, int nDOF, OUT T_INELASTICDYN_HINGE_INFO_BASE& rHngeInfoBase, OUT std::vector<int>& rvSeqId);
	bool GetTHInelstHngeInfo(T_ELEM_K ElemK, int nDOF, OUT T_INELASTICDYN_HINGE_INFO_BASE& rHngeInfoBase, OUT std::vector<int>& rvSeqId);

	bool GetInelstHngeInfoBase(const T_INELASTICDYN_HINGE_INFO& HngeInfo, int nDOF, OUT T_INELASTICDYN_HINGE_INFO_BASE& rHngeInfoBase);
	
	bool GetPOHingeSeqId(const T_INELASTICDYN_HINGE_INFO_BASE& HngeInfoBase, OUT std::vector<int>& rvSeqId);
	
	bool GetTHHingeSeqIdElemBeam(const T_INELASTICDYN_HINGE_INFO_BASE& HngeInfoBase, OUT std::vector<int>& rvSeqId);
	bool GetTHHingeSeqIdElemWall(const T_INELASTICDYN_HINGE_INFO_BASE& HngeInfoBase, OUT std::vector<int>& rvSeqId);

	bool GetTHHingeResult(T_THIS_K ThisK, int nHingeTimeInex, int nSeqId, int nHgsmHingeType, int nDOF, OUT T_NLHG_D& rNlhgD);

private:
	CDBDoc* m_pDoc;
};

