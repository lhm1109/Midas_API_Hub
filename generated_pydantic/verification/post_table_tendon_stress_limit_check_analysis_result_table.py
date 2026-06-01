from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Tendon Stress Limit Check - Analysis Result Table (POST /post/TABLE)

class TendonStressLimitCheckAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class TendonStressLimitCheckAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class TendonStressLimitCheckAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["TNDN_STRS_LIMIT_CHECK"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: TendonStressLimitCheckAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: TendonStressLimitCheckAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Tendon", "TendonStress/f_p1", "TendonStress/f_p2", "TendonStress/f_pe", "TendonStressLimit/Atanch.", "TendonStressLimit/Awayfromanch.", "TendonStressLimit/Atservice"]] | None = Field(None, alias="COMPONENTS")

class TendonStressLimitCheckAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: TendonStressLimitCheckAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-tendon-stress-limit-check-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["TendonStressLimitCheckAnalysisResultTableRequest"]
