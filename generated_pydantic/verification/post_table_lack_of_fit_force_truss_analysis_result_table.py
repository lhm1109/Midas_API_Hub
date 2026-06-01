from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Lack of Fit Force (Truss) - Analysis Result Table (POST /post/TABLE)

class LackOfFitForceTrussAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class LackOfFitForceTrussAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class LackOfFitForceTrussAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["LACK_OF_FIT_FORCE_TRUSS"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: LackOfFitForceTrussAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: LackOfFitForceTrussAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Elem", "NodeI", "NodeJ", "Pretension", "LOFForce", "SUM", "LocalVector/V-X", "LocalVector/V-Y", "LocalVector/V-Z", "Angle", "Elasticity", "Area", "I-NodeDisp./DX", "I-NodeDisp./DY", "I-NodeDisp./DZ", "J-NodeDisp./DX", "J-NodeDisp./DY", "J-NodeDisp./DZ", "Deform"]] | None = Field(None, alias="COMPONENTS")

class LackOfFitForceTrussAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: LackOfFitForceTrussAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-lack-of-fit-force-truss-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["LackOfFitForceTrussAnalysisResultTableRequest"]
