from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Story Mode Shape - Story Result Table (POST /POST/TABLE)

class StoryModeShapeStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class StoryModeShapeStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class StoryModeShapeStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["STORY_MODE_SHAPE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: StoryModeShapeStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: StoryModeShapeStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    modes: list[str] | None = Field(None, alias="MODES")
    components: list[Literal["Story", "Level", "X", "Y", "Mode", "UX", "UY", "UZ", "RX", "RY", "RZ"]] | None = Field(None, alias="COMPONENTS")

class StoryModeShapeStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: StoryModeShapeStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-story-mode-shape-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["StoryModeShapeStoryResultTableRequest"]
