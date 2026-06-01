from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Initial Element Force - Analysis Result Table (POST /post/TABLE)

class InitialElementForceAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class InitialElementForceAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class InitialElementForceAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["INITIAL_ELEM_FORCE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: InitialElementForceAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: InitialElementForceAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Type", "ID", "MemberForce-i/Axial", "MemberForce-i/Shear(y)", "MemberForce-i/Shear(z)", "MemberForce-i/Torsion", "MemberForce-i/Moment(y)", "MemberForce-i/Moment(z)", "MemberForce-j/Axial", "MemberForce-j/Shear(y)", "MemberForce-j/Shear(z)", "MemberForce-j/Torsion", "MemberForce-j/Moment(y)", "MemberForce-j/Moment(z)"]] | None = Field(None, alias="COMPONENTS")

class InitialElementForceAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: InitialElementForceAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-initial-element-force-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["InitialElementForceAnalysisResultTableRequest"]
