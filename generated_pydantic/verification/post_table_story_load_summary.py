from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Story Load Summary (POST /POST/TABLE)

class StoryLoadSummaryRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class StoryLoadSummaryRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class StoryLoadSummaryRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["STORY_LOAD_SUMMARY_X", "STORY_LOAD_SUMMARY_Y", "STORY_LOAD_SUMMARY_Z"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: StoryLoadSummaryRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: StoryLoadSummaryRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Load", "Story", "Level", "Concent", "Beam", "Floor", "Pressure", "SelfWeight", "Sum"]] | None = Field(None, alias="COMPONENTS")

class StoryLoadSummaryRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: StoryLoadSummaryRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-story-load-summary"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["StoryLoadSummaryRequest"]
