from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Beam Force - TH Result Table (POST /post/TABLE)

class BeamForceTHResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class BeamForceTHResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class BeamForceTHResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class BeamForceTHResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["THISBEAMFORCE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: BeamForceTHResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: BeamForceTHResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: BeamForceTHResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    parts: list[str] | None = Field(None, alias="PARTS")
    components: list[Literal["Elem", "Load", "Part", "Axial/Force", "Axial/Time/Step", "Shear-y/Force", "Shear-y/Time/Step", "Shear-z/Force", "Shear-z/Time/Step", "Torsion/Force", "Torsion/Time/Step", "Moment-y/Force", "Moment-y/Time/Step", "Moment-z/Force", "Moment-z/Time/Step"]] | None = Field(None, alias="COMPONENTS")

class BeamForceTHResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: BeamForceTHResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-beam-force-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["BeamForceTHResultTableRequest"]
