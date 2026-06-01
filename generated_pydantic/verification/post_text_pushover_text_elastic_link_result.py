from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Pushover Text - Elastic Link Result (POST /post/TEXT)

class PushoverTextElasticLinkResultRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class PushoverTextElasticLinkResultRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class PushoverTextElasticLinkResultRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PushoverTextElasticLinkResultRequestArgumentSTEP(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    from_: int | None = Field(None, alias="FROM")
    to: str | None = Field(None, alias="TO")
    steps: int | None = Field(None, alias="STEPS")
    keys: list[int] | None = Field(None, alias="KEYS")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class PushoverTextElasticLinkResultRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["PO_ELINKDEFORM", "PO_ELINKFORCE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: PushoverTextElasticLinkResultRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: PushoverTextElasticLinkResultRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    text_type: str | None = Field(None, alias="TEXT_TYPE")
    node_elems: PushoverTextElasticLinkResultRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    po_case_name: list[str] | None = Field(None, alias="PO_CASE_NAME")
    step: PushoverTextElasticLinkResultRequestArgumentSTEP | None = Field(None, alias="STEP")
    parts: list[str] | None = Field(None, alias="PARTS")
    components: list[Literal["Key", "Node1", "Node2", "Load", "Step", "DX", "DY", "DZ", "RX", "RY", "RZ", "Part", "FX", "FY", "FZ", "MX", "MY", "MZ"]] | None = Field(None, alias="COMPONENTS")

class PushoverTextElasticLinkResultRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: PushoverTextElasticLinkResultRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-text-pushover-text-elastic-link-result"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["PushoverTextElasticLinkResultRequest"]
