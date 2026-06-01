from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Effective Span Length - Analysis Result Table (POST /post/TABLE)

class EffectiveSpanLengthAnalysisResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class EffectiveSpanLengthAnalysisResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class EffectiveSpanLengthAnalysisResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["EFFECTIVE_LENGTH_TRUSS", "EFFECTIVE_LENGTH_BEAM", "EFFECTIVE_LENGTH_PLATE"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: EffectiveSpanLengthAnalysisResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: EffectiveSpanLengthAnalysisResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    components: list[Literal["Element", "Lane", "Max", "Min", "Parts", "My_max", "My_min", "Mz_max", "Mz_min", "Fx_max", "Fx_min", "ImpactFactor", "Mxx_max", "Mxx_min", "Myy_max", "Myy_min", "Fxx_max", "Fxx_min", "Fyy_max", "Fyy_min"]] | None = Field(None, alias="COMPONENTS")

class EffectiveSpanLengthAnalysisResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: EffectiveSpanLengthAnalysisResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-effective-span-length-analysis-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["EffectiveSpanLengthAnalysisResultTableRequest"]
