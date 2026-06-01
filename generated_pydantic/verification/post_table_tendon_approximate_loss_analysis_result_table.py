from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Tendon Approximate Loss - Analysis Result Table (POST /post/TABLE)

class TendonApproximateLossAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class TendonApproximateLossAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class TendonApproximateLossAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["TNDN_STRS_LIMIT_CHECK", "TNDN_APPROX_LOSS_STRESS", "TNDN_APPROX_LOSS_FORCE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: TendonApproximateLossAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: TendonApproximateLossAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Elem", "Part", "ImmediateLoss", "CreepLoss", "ShrinkageLoss", "RelaxationLoss", "AllLoss", "Stress(ImmediateLoss)", "Stress(AllLoss)", "Stress(AllLoss)/Stress", "Force(ImmediateLoss)", "Force(AllLoss)", "Force(AllLoss)/Force"]] | None = Field(None, alias="COMPONENTS")

class TendonApproximateLossAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: TendonApproximateLossAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-tendon-approximate-loss-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["TendonApproximateLossAnalysisResultTableRequest"]
