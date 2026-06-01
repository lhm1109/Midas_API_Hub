from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Pushover Text - Displacement Result (POST /post/TEXT)

class PushoverTextDisplacementResultRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class PushoverTextDisplacementResultRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class PushoverTextDisplacementResultRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PushoverTextDisplacementResultRequestArgumentSTEP(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    from_: int | None = Field(None, alias="FROM")
    to: str | None = Field(None, alias="TO")
    steps: int | None = Field(None, alias="STEPS")
    keys: list[int] | None = Field(None, alias="KEYS")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PushoverTextDisplacementResultRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["PO_DISP"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: PushoverTextDisplacementResultRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: PushoverTextDisplacementResultRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    text_type: str | None = Field(None, alias="TEXT_TYPE")
    node_elems: PushoverTextDisplacementResultRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    po_case_name: list[str] | None = Field(None, alias="PO_CASE_NAME")
    step: PushoverTextDisplacementResultRequestArgumentSTEP | None = Field(None, alias="STEP")
    ref_pt: str | None = Field(None, alias="REF_PT")
    components: list[Literal["Node", "Load", "Step", "Dx", "Dy", "Dz", "Rx", "Ry", "Rz"]] | None = Field(None, alias="COMPONENTS")

class PushoverTextDisplacementResultRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: PushoverTextDisplacementResultRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-text-pushover-text-displacement-result"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["PushoverTextDisplacementResultRequest"]
