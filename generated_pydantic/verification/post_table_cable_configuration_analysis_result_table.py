from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Cable Configuration - Analysis Result Table (POST /post/TABLE)

class CableConfigurationAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class CableConfigurationAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class CableConfigurationAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class CableConfigurationAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["CABLECONFIG"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: CableConfigurationAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: CableConfigurationAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: CableConfigurationAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    opt_cs: bool | None = Field(None, alias="OPT_CS")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    components: list[Literal["Elem", "NodeI", "NodeJ", "Load", "Step", "TotalLength", "Elongation", "UnstrainedLength", "Sag", "HorizontalDistance", "VerticalDistance", "Gradient", "SkewAngle/IEnd", "SkewAngle/JEnd", "Stage", "IEnd", "JEnd"]] | None = Field(None, alias="COMPONENTS")

class CableConfigurationAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: CableConfigurationAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-cable-configuration-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["CableConfigurationAnalysisResultTableRequest"]
