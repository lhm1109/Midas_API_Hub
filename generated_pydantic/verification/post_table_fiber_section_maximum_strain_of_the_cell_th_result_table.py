from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Fiber Section Maximum Strain of The Cell - TH Result Table (POST /post/TABLE)

class FiberSectionMaximumStrainOfTheCellTHResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class FiberSectionMaximumStrainOfTheCellTHResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class FiberSectionMaximumStrainOfTheCellTHResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class FiberSectionMaximumStrainOfTheCellTHResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["FIBR_MAXCONTRACT"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: FiberSectionMaximumStrainOfTheCellTHResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: FiberSectionMaximumStrainOfTheCellTHResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: FiberSectionMaximumStrainOfTheCellTHResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    th_load_case_names: list[str] | None = Field(None, alias="TH_LOAD_CASE_NAMES")
    sect_position: str | None = Field(None, alias="SECT_POSITION")
    fiber_cell_minmax: bool | None = Field(None, alias="FIBER_CELL_MINMAX")
    components: list[Literal["要素", "断面位置", "材料", "荷重", "セルNo.", "最小/epsilon", "最小/時間/ステップ", "最大/epsilon", "最大/時間/ステップ"]] | None = Field(None, alias="COMPONENTS")

class FiberSectionMaximumStrainOfTheCellTHResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: FiberSectionMaximumStrainOfTheCellTHResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-fiber-section-maximum-strain-of-the-cell-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["FiberSectionMaximumStrainOfTheCellTHResultTableRequest"]
