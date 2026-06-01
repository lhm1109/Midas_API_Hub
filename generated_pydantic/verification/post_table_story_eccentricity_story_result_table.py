from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Story Eccentricity - Story Result Table (POST /POST/TABLE)

class StoryEccentricityStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class StoryEccentricityStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class StoryEccentricityStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["STORY_ECNTRICITY"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: StoryEccentricityStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: StoryEccentricityStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Story", "Level", "WeightCenter/X", "WeightCenter/Y", "StiffnessCenter/X", "StiffnessCenter/Y", "Ecc.Dist./X", "Ecc.Dist./Y", "TorsionalStiffness", "El.Radius/X", "El.Radius/Y", "Ecc.Ratio/X", "Ecc.Ratio/Y"]] | None = Field(None, alias="COMPONENTS")

class StoryEccentricityStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: StoryEccentricityStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-story-eccentricity-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["StoryEccentricityStoryResultTableRequest"]
