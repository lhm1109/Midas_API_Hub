from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Fiber Section Average Compression Strain - TH Result Table (POST /post/TABLE)

class FiberSectionAverageCompressionStrainTHResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class FiberSectionAverageCompressionStrainTHResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class FiberSectionAverageCompressionStrainTHResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class FiberSectionAverageCompressionStrainTHResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["FIBR_MEANCOMPCONTRACT"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: FiberSectionAverageCompressionStrainTHResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: FiberSectionAverageCompressionStrainTHResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: FiberSectionAverageCompressionStrainTHResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    th_load_case_names: list[str] | None = Field(None, alias="TH_LOAD_CASE_NAMES")
    sect_position: str | None = Field(None, alias="SECT_POSITION")
    output_step: str | None = Field(None, alias="OUTPUT_STEP")
    components: list[Literal["各ステップにおける外側セルの平均圧縮ひずみ/ステップ", "各ステップにおける外側セルの平均圧縮ひずみ/要素", "各ステップにおける外側セルの平均圧縮ひずみ/断面位置", "各ステップにおける外側セルの平均圧縮ひずみ/材料", "各ステップにおける外側セルの平均圧縮ひずみ/荷重", "各ステップにおける外側セルの平均圧縮ひずみ/Ea"]] | None = Field(None, alias="COMPONENTS")

class FiberSectionAverageCompressionStrainTHResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: FiberSectionAverageCompressionStrainTHResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-fiber-section-average-compression-strain-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["FiberSectionAverageCompressionStrainTHResultTableRequest"]
