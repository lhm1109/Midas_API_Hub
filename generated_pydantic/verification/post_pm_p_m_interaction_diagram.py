from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from P-M Interaction Diagram (POST /post/PM)

class PMInteractionDiagramRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class PMInteractionDiagramRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class PMInteractionDiagramRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: PMInteractionDiagramRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: PMInteractionDiagramRequestArgumentSTYLES | None = Field(None, alias="STYLES")

class PMInteractionDiagramRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: PMInteractionDiagramRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-pm-p-m-interaction-diagram"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["PMInteractionDiagramRequest"]
