from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Lack of Fit Force (Plate) - Analysis Result Table (POST /post/TABLE)

class LackOfFitForcePlateAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class LackOfFitForcePlateAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class LackOfFitForcePlateAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["LACK_OF_FIT_FORCE_PLATE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: LackOfFitForcePlateAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: LackOfFitForcePlateAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Elem", "Node", "LOFForce/Axial", "LOFForce/Shear-y", "LOFForce/Shear-z", "LOFForce/Torsion", "LOFForce/Moment-y", "LOFForce/Moment-z", "Displacement/DX", "Displacement/DY", "Displacement/DZ", "Displacement/RX", "Displacement/RY", "Displacement/RZ"]] | None = Field(None, alias="COMPONENTS")

class LackOfFitForcePlateAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: LackOfFitForcePlateAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-lack-of-fit-force-plate-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["LackOfFitForcePlateAnalysisResultTableRequest"]
