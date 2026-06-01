from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Resultant Forces - Analysis Result Table (POST /post/TABLE)

class ResultantForcesAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class ResultantForcesAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class ResultantForcesAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class ResultantForcesAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["RESULTANT_FORCES", "RESULTANT_FORCESVBM"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: ResultantForcesAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: ResultantForcesAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: ResultantForcesAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    parts: list[Literal["PartI", "PartJ"]] | None = Field(None, alias="PARTS")
    opt_cs: bool | None = Field(None, alias="OPT_CS")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    item_to_display: list[Literal["Axial", "Shear-y", "Shear-z", "Torsion", "Moment-y", "Moment-z"]] | None = Field(None, alias="ITEM_TO_DISPLAY")
    components: list[Literal["VirtualBeam", "Load", "Part", "Axial", "Shear-Y", "Shear-Z", "Torsion", "Moment-Y", "Moment-Z", "Stage", "Step", "Component"]] | None = Field(None, alias="COMPONENTS")

class ResultantForcesAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: ResultantForcesAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-resultant-forces-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["ResultantForcesAnalysisResultTableRequest"]
