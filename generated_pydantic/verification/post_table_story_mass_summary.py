from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Story Mass Summary (POST /POST/TABLE)

class StoryMassSummaryRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class StoryMassSummaryRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class StoryMassSummaryRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["STORY_MASS", "STORY_MASS_X"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: StoryMassSummaryRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: StoryMassSummaryRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Story", "Level", "X-DIR", "Y-DIR", "RotationalMass", "X-Coord", "Y-Coord", "NodalMass", "LoadToMass", "DiaphragmMass", "StructureMass", "Sum"]] | None = Field(None, alias="COMPONENTS")

class StoryMassSummaryRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: StoryMassSummaryRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-story-mass-summary"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["StoryMassSummaryRequest"]
