from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Tendon Weight - Analysis Result Table (POST /post/TABLE)

class TendonWeightAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class TendonWeightAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class TendonWeightAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["TNDN_WEIGHT_PROFILE", "TNDN_WEIGHT_PROPERTY", "TNDN_WEIGHT_GROUP"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: TendonWeightAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: TendonWeightAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["TendonName", "TendonNum", "Area", "Length", "Weight/Length", "Weight", "TotalWeight", "TendonProperty", "TotalLength", "TendonGroup"]] | None = Field(None, alias="COMPONENTS")

class TendonWeightAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: TendonWeightAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-tendon-weight-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["TendonWeightAnalysisResultTableRequest"]
