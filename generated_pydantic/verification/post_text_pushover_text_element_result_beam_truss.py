from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Pushover Text - Element Result(Beam, Truss) (POST /post/TEXT)

class PushoverTextElementResultBeamTrussRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class PushoverTextElementResultBeamTrussRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class PushoverTextElementResultBeamTrussRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PushoverTextElementResultBeamTrussRequestArgumentSTEP(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    from_: int | None = Field(None, alias="FROM")
    to: str | None = Field(None, alias="TO")
    steps: int | None = Field(None, alias="STEPS")
    keys: list[int] | None = Field(None, alias="KEYS")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PushoverTextElementResultBeamTrussRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["PO_BEAMFORCE", "PO_TRUSSFORCE", "PO_BEAMSTRESS", "PO_TRUSSSTRESS"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: PushoverTextElementResultBeamTrussRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: PushoverTextElementResultBeamTrussRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    text_type: str | None = Field(None, alias="TEXT_TYPE")
    node_elems: PushoverTextElementResultBeamTrussRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    parts: list[str] | None = Field(None, alias="PARTS")
    po_case_name: list[str] | None = Field(None, alias="PO_CASE_NAME")
    step: PushoverTextElementResultBeamTrussRequestArgumentSTEP | None = Field(None, alias="STEP")
    components: list[Literal["Elem", "Load", "Step", "Part", "Axial", "Shear-y", "Shear-z", "Torsion", "Moment-y", "Moment-z", "Force-I", "Force-J", "Stress-I", "Stress-J", "Time/Step", "Bending(+y)", "Bending(-y)", "Bending(+z)", "Bending(-z)"]] | None = Field(None, alias="COMPONENTS")

class PushoverTextElementResultBeamTrussRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: PushoverTextElementResultBeamTrussRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-text-pushover-text-element-result-beam-truss"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["PushoverTextElementResultBeamTrussRequest"]
