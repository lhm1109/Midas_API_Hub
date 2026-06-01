from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Analysis Status (POST /OPE/ANALSTATUS)

class AnalysisStatusRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    pass

class AnalysisStatusRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: AnalysisStatusRequestArgument = Field(..., alias="Argument")

class AnalysisStatusResponseANALSTATUSItem(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    stage: str = Field(..., alias="STAGE")
    status: Literal["Waiting", "Analyzing", "Done"] = Field(..., alias="STATUS")
    progress: str = Field(..., alias="PROGRESS")

class AnalysisStatusResponse(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    anal_status: list[AnalysisStatusResponseANALSTATUSItem] = Field(..., alias="ANAL_STATUS")

ENDPOINT_ID = "ope/analstatus/analysis-status"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["AnalysisStatusRequest", "AnalysisStatusResponse"]
