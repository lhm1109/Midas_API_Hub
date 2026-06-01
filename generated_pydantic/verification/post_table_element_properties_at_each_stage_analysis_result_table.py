from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Element Properties at Each Stage - Analysis Result Table (POST /post/TABLE)

class ElementPropertiesAtEachStageAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class ElementPropertiesAtEachStageAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class ElementPropertiesAtEachStageAnalysisResultTableRequestArgumentADDITIONALSETSTAGE(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    stage: str | None = Field(None, alias="STAGE")

class ElementPropertiesAtEachStageAnalysisResultTableRequestArgumentADDITIONAL(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    set_stage: ElementPropertiesAtEachStageAnalysisResultTableRequestArgumentADDITIONALSETSTAGE | None = Field(None, alias="SET_STAGE")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class ElementPropertiesAtEachStageAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["ELEM_PROP_EACH_STAGE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: ElementPropertiesAtEachStageAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: ElementPropertiesAtEachStageAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    additional: ElementPropertiesAtEachStageAnalysisResultTableRequestArgumentADDITIONAL | None = Field(None, alias="ADDITIONAL")
    components: list[Literal["Elem", "StartAge", "EndAge", "StartElasticity", "EndElasticity", "CumulativeShrinkage", "CreepCoeff."]] | None = Field(None, alias="COMPONENTS")

class ElementPropertiesAtEachStageAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: ElementPropertiesAtEachStageAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-element-properties-at-each-stage-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["ElementPropertiesAtEachStageAnalysisResultTableRequest"]
