from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Vibration Mode Shape - Analysis Result Table (POST /post/TABLE)

class VibrationModeShapeAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class VibrationModeShapeAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class VibrationModeShapeAnalysisResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class VibrationModeShapeAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["EIGENVALUEMODE", "PARTICIPATIONVECTORMODE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: VibrationModeShapeAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: VibrationModeShapeAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: VibrationModeShapeAnalysisResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    modes: list[str] | None = Field(None, alias="MODES")
    components: list[Literal["Node", "Mode", "UX", "UY", "UZ", "RX", "RY", "RZ", "ModeNo", "Frequency(rad/sec)", "Frequency(cycle/sec)", "Period(sec)", "Tolerance", "TRAN-XMASS(%)", "TRAN-XSUM(%)", "TRAN-YMASS(%)", "TRAN-YSUM(%)", "TRAN-ZMASS(%)", "TRAN-ZSUM(%)", "ROTN-XMASS(%)", "ROTN-XSUM(%)", "ROTN-YMASS(%)", "ROTN-YSUM(%)", "ROTN-ZMASS(%)", "ROTN-ZSUM(%)", "TRAN-XMASS", "TRAN-XSUM", "TRAN-YMASS", "TRAN-YSUM", "TRAN-ZMASS", "TRAN-ZSUM", "ROTN-XMASS", "ROTN-XSUM", "ROTN-YMASS", "ROTN-YSUM", "ROTN-ZMASS", "ROTN-ZSUM", "TRAN-XValue", "TRAN-YValue", "TRAN-ZValue", "ROTN-XValue", "ROTN-YValue", "ROTN-ZValue"]] | None = Field(None, alias="COMPONENTS")

class VibrationModeShapeAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: VibrationModeShapeAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-vibration-mode-shape-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["VibrationModeShapeAnalysisResultTableRequest"]
