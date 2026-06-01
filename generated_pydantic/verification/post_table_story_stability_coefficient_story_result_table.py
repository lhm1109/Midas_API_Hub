from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Story Stability Coefficient - Story Result Table (POST /POST/TABLE)

class StoryStabilityCoefficientStoryResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class StoryStabilityCoefficientStoryResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class StoryStabilityCoefficientStoryResultTableRequestArgumentADDITIONALSETSTABILITYCOEFFICIENTPARAMSLCOMSItem(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    name: str | None = Field(None, alias="NAME")
    factor: int | None = Field(None, alias="FACTOR")

class StoryStabilityCoefficientStoryResultTableRequestArgumentADDITIONALSETSTABILITYCOEFFICIENTPARAMSBETA(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    fix_user_check: str | None = Field(None, alias="FIX_USER_CHECK")
    name_from: str | None = Field(None, alias="NAME_FROM")
    name_to: str | None = Field(None, alias="NAME_TO")
    value: int | None = Field(None, alias="VALUE")

class StoryStabilityCoefficientStoryResultTableRequestArgumentADDITIONALSETSTABILITYCOEFFICIENTPARAMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    deflection_ampl_factor_value: int | None = Field(None, alias="DEFLECTION_AMPL_FACTOR_VALUE")
    importance_factor_value: int | None = Field(None, alias="IMPORTANCE_FACTOR_VALUE")
    scale_factor_value: int | None = Field(None, alias="SCALE_FACTOR_VALUE")
    lcoms: list[StoryStabilityCoefficientStoryResultTableRequestArgumentADDITIONALSETSTABILITYCOEFFICIENTPARAMSLCOMSItem] | None = Field(None, alias="LCOMS")
    beta: StoryStabilityCoefficientStoryResultTableRequestArgumentADDITIONALSETSTABILITYCOEFFICIENTPARAMSBETA | None = Field(None, alias="BETA")

class StoryStabilityCoefficientStoryResultTableRequestArgumentADDITIONALSETCALCULATIONMETHOD(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    story_drift_method: str | None = Field(None, alias="STORY_DRIFT_METHOD")

class StoryStabilityCoefficientStoryResultTableRequestArgumentADDITIONAL(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    set_stability_coefficient_params: StoryStabilityCoefficientStoryResultTableRequestArgumentADDITIONALSETSTABILITYCOEFFICIENTPARAMS | None = Field(None, alias="SET_STABILITY_COEFFICIENT_PARAMS")
    set_calculation_method: StoryStabilityCoefficientStoryResultTableRequestArgumentADDITIONALSETCALCULATIONMETHOD | None = Field(None, alias="SET_CALCULATION_METHOD")
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class StoryStabilityCoefficientStoryResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["STORY_STABILITY_COEFFICIENT_X", "STORY_STABILITY_COEFFICIENT_Y"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: StoryStabilityCoefficientStoryResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: StoryStabilityCoefficientStoryResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    load_case_names: list[str] | None = Field(None, alias="LOAD_CASE_NAMES")
    additional: StoryStabilityCoefficientStoryResultTableRequestArgumentADDITIONAL | None = Field(None, alias="ADDITIONAL")
    components: list[Literal["LoadCase", "Story", "StoryHeight", "VerticalLoad", "StoryShearForce", "ModifiedStoryDrift", "Beta", "StabilityCoefficient", "AllowableLimit", "Remark", "P-DeltaIncrementalFactor"]] | None = Field(None, alias="COMPONENTS")

class StoryStabilityCoefficientStoryResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: StoryStabilityCoefficientStoryResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-story-stability-coefficient-story-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["StoryStabilityCoefficientStoryResultTableRequest"]
