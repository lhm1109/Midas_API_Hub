from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Story Shear Force(R.S.Analysis) - Story Result Table (POST /POST/TABLE)

class StoryShearForceRSAnalysisStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class StoryShearForceRSAnalysisStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class StoryShearForceRSAnalysisStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["STORY_SHEAR_FOR_RS"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: StoryShearForceRSAnalysisStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: StoryShearForceRSAnalysisStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    components: list[Literal["Story", "Level", "Spectrum", "InertiaForce/X", "InertiaForce/Y", "ShearForce/SpringReactions/X", "ShearForce/SpringReactions/Y", "ShearForce/WithoutSpring/X", "ShearForce/WithoutSpring/Y", "ShearForce/WithSpring/X", "ShearForce/WithSpring/Y", "Eccentricity", "StoryForce", "EccentricMoment"]] | None = Field(None, alias="COMPONENTS")

class StoryShearForceRSAnalysisStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: StoryShearForceRSAnalysisStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-story-shear-force-r-s-analysis-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["StoryShearForceRSAnalysisStoryResultTableRequest"]
