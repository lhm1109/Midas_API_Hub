from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Tendon Arrangement - Analysis Result Table (POST /post/TABLE)

class TendonArrangementAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class TendonArrangementAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class TendonArrangementAnalysisResultTableRequestArgumentADDITIONALSETTENDONPARAMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    tendon_group: str | None = Field(None, alias="TENDON_GROUP")
    stage: str | None = Field(None, alias="STAGE")

class TendonArrangementAnalysisResultTableRequestArgumentADDITIONAL(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    set_tendon_params: TendonArrangementAnalysisResultTableRequestArgumentADDITIONALSETTENDONPARAMS | None = Field(None, alias="SET_TENDON_PARAMS")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class TendonArrangementAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["TNDN_ARRANGEMENT"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: TendonArrangementAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: TendonArrangementAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    additional: TendonArrangementAnalysisResultTableRequestArgumentADDITIONAL | None = Field(None, alias="ADDITIONAL")
    components: list[Literal["Elem", "Part", "TendonNumber", "Yp", "Zp", "AverageSintheta", "AverageCostheta", "AverageStress", "AverageForce"]] | None = Field(None, alias="COMPONENTS")

class TendonArrangementAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: TendonArrangementAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-tendon-arrangement-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["TendonArrangementAnalysisResultTableRequest"]
