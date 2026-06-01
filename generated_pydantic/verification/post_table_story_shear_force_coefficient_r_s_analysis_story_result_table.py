from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Story Shear Force Coefficient(R.S.Analysis) - Story Result Table (POST /POST/TABLE)

class StoryShearForceCoefficientRSAnalysisStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class StoryShearForceCoefficientRSAnalysisStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class StoryShearForceCoefficientRSAnalysisStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["STORY_SHEAR_FORCE_COEFFICIENT"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: StoryShearForceCoefficientRSAnalysisStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: StoryShearForceCoefficientRSAnalysisStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    components: list[Literal["Story", "Spectrum", "ShearForce/X", "ShearForce/Y", "WeightSum/X", "WeightSum/Y", "StoryShearForceCoefficient/X", "StoryShearForceCoefficient/Y"]] | None = Field(None, alias="COMPONENTS")

class StoryShearForceCoefficientRSAnalysisStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: StoryShearForceCoefficientRSAnalysisStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-story-shear-force-coefficient-r-s-analysis-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["StoryShearForceCoefficientRSAnalysisStoryResultTableRequest"]
