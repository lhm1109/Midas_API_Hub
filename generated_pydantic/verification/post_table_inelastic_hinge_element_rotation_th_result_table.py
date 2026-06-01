from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Inelastic Hinge Element Rotation - TH Result Table (POST /post/TABLE)

class InelasticHingeElementRotationTHResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class InelasticHingeElementRotationTHResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class InelasticHingeElementRotationTHResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["IEHG_ELEM_ROT_BEAM", "IEHG_ELEM_ROT_WALL"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: InelasticHingeElementRotationTHResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: InelasticHingeElementRotationTHResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    th_load_case_names: list[str] | None = Field(None, alias="TH_LOAD_CASE_NAMES")
    components: list[Literal["Elem", "Load", "Part", "Ry/Rotation", "Ry/Time", "Rz/Rotation", "Rz/Time", "Story", "WallID"]] | None = Field(None, alias="COMPONENTS")

class InelasticHingeElementRotationTHResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: InelasticHingeElementRotationTHResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-inelastic-hinge-element-rotation-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["InelasticHingeElementRotationTHResultTableRequest"]
