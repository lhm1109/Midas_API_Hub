from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Fiber Section Elastic Modulus Retention Rate - TH Result Table (POST /post/TABLE)

class FiberSectionElasticModulusRetentionRateTHResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class FiberSectionElasticModulusRetentionRateTHResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class FiberSectionElasticModulusRetentionRateTHResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["FIBR_ELASTREMAIN"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: FiberSectionElasticModulusRetentionRateTHResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: FiberSectionElasticModulusRetentionRateTHResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    th_load_case_names: list[str] | None = Field(None, alias="TH_LOAD_CASE_NAMES")
    components: list[Literal["番号/要素", "番号/断面", "番号/セルNo.", "番号/荷重", "セル位置/y", "セル位置/z", "弾性剛性残存率/K", "セル面積/△A"]] | None = Field(None, alias="COMPONENTS")

class FiberSectionElasticModulusRetentionRateTHResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: FiberSectionElasticModulusRetentionRateTHResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-fiber-section-elastic-modulus-retention-rate-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["FiberSectionElasticModulusRetentionRateTHResultTableRequest"]
