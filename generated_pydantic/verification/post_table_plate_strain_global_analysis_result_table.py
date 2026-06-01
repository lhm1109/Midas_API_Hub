from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Plate Strain (Global) - Analysis Result Table (POST /post/TABLE)

class PlateStrainGlobalAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class PlateStrainGlobalAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class PlateStrainGlobalAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PlateStrainGlobalAnalysisResultTableRequestArgumentNODEFLAG(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    center: bool | None = Field(None, alias="CENTER")
    nodes: bool | None = Field(None, alias="NODES")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PlateStrainGlobalAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["PLATESTRAING", "PLATESTRAINPG", "PLATESTRAINTG"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: PlateStrainGlobalAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: PlateStrainGlobalAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: PlateStrainGlobalAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    node_flag: PlateStrainGlobalAnalysisResultTableRequestArgumentNODEFLAG | None = Field(None, alias="NODE_FLAG")
    opt_cs: bool | None = Field(None, alias="OPT_CS")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    components: list[Literal["Elem", "Load", "Step", "Node", "Part", "Strain-XX", "Strain-YY", "Strain-ZZ", "Strain-XY", "Strain-YZ", "Strain-XZ", "Strain-Max", "Strain-Min", "Angle", "Max-Shear", "Stage", "Strain-xx", "Strain-yy", "Strain-xy", "Comp.Damage", "Tens.Damage", "Damage"]] | None = Field(None, alias="COMPONENTS")

class PlateStrainGlobalAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: PlateStrainGlobalAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-plate-strain-global-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["PlateStrainGlobalAnalysisResultTableRequest"]
