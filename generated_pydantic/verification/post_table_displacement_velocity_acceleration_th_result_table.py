from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Displacement/Velocity/Acceleration - TH Result Table (POST /post/TABLE)

class DisplacementVelocityAccelerationTHResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class DisplacementVelocityAccelerationTHResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class DisplacementVelocityAccelerationTHResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class DisplacementVelocityAccelerationTHResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["THISDISPLACEMENT", "THISVELOCITY", "THISABSOLUTEACCEL", "THISRELATIVEACCEL"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: DisplacementVelocityAccelerationTHResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: DisplacementVelocityAccelerationTHResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: DisplacementVelocityAccelerationTHResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    components: list[Literal["Node", "Load", "DX/DX", "DX/Time/Step", "DY/DY", "DY/Time/Step", "DZ/DZ", "DZ/Time/Step", "RX/RX", "RX/Time/Step", "RY/RY", "RY/Time/Step", "RZ/RZ", "RZ/Time/Step"]] | None = Field(None, alias="COMPONENTS")

class DisplacementVelocityAccelerationTHResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: DisplacementVelocityAccelerationTHResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-displacement-velocity-acceleration-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["DisplacementVelocityAccelerationTHResultTableRequest"]
