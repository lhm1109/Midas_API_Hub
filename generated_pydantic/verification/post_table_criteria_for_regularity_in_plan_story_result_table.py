from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Criteria for Regularity in Plan - Story Result Table (POST /POST/TABLE)

class CriteriaForRegularityInPlanStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class CriteriaForRegularityInPlanStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class CriteriaForRegularityInPlanStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["CRITERIA_FOR_REGULARITY_IN_PLAN"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: CriteriaForRegularityInPlanStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: CriteriaForRegularityInPlanStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Story", "Level", "TranslationalMass/X-DIR", "TranslationalMass/Y-DIR", "RotationalMass", "Rx/X", "Rx/Y", "r²/Is²/X", "r²/Is²/Y", "Check/X", "Check/Y"]] | None = Field(None, alias="COMPONENTS")

class CriteriaForRegularityInPlanStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: CriteriaForRegularityInPlanStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-criteria-for-regularity-in-plan-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["CriteriaForRegularityInPlanStoryResultTableRequest"]
