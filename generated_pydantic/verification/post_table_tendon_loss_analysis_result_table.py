from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Tendon Loss - Analysis Result Table (POST /post/TABLE)

class TendonLossAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class TendonLossAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class TendonLossAnalysisResultTableRequestArgumentADDITIONALSETTENDONPARAMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    tendon_group: str | None = Field(None, alias="TENDON_GROUP")
    stage: str | None = Field(None, alias="STAGE")

class TendonLossAnalysisResultTableRequestArgumentADDITIONAL(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    set_tendon_params: TendonLossAnalysisResultTableRequestArgumentADDITIONALSETTENDONPARAMS | None = Field(None, alias="SET_TENDON_PARAMS")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class TendonLossAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["TNDN_LOSS_STRESS", "TNDN_LOSS_FORCE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: TendonLossAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: TendonLossAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    additional: TendonLossAnalysisResultTableRequestArgumentADDITIONAL | None = Field(None, alias="ADDITIONAL")
    components: list[Literal["Elem", "Part", "Stress(AfterImmediateLoss):A", "ElasticDeform.Loss:B", "Ratio/A", "Creep/ShrinkageLoss", "RelaxationLoss", "Stress(AfterAllLoss)/Stress(AfterImmediateLoss)", "EffectiveNum.", "Force(AfterImmediateLoss):A", "Force(AfterAllLoss)/Force(AfterImmediateLoss)"]] | None = Field(None, alias="COMPONENTS")

class TendonLossAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: TendonLossAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-tendon-loss-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["TendonLossAnalysisResultTableRequest"]
