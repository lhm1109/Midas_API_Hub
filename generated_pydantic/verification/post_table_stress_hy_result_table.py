from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Stress - HY Result Table (POST /post/TABLE)

class StressHYResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class StressHYResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class StressHYResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class StressHYResultTableRequestArgumentNODEFLAG(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    center: bool | None = Field(None, alias="CENTER")
    nodes: bool | None = Field(None, alias="NODES")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class StressHYResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["HEAT_HYDR_STRESS_L", "HEAT_HYDR_STRESS_G"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: StressHYResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: StressHYResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: StressHYResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    node_flag: StressHYResultTableRequestArgumentNODEFLAG | None = Field(None, alias="NODE_FLAG")
    stage_step: list[str] | None = Field(None, alias="STAGE_STEP")
    components: list[Literal["Elem", "Stage", "Step", "Time", "Node", "Sig-xx", "Sig-yy", "Sig-zz", "Sig-xy", "Sig-yz", "Sig-xz", "Sig-P1", "Sig-P2", "Sig-P3", "Max-Shear", "Sig-EFF", "Sig-OCT", "Sig-XX", "Sig-YY", "Sig-ZZ", "Sig-XY", "Sig-YZ", "Sig-XZ"]] | None = Field(None, alias="COMPONENTS")

class StressHYResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: StressHYResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-stress-hy-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["StressHYResultTableRequest"]
