from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Fiber Section Event Step - TH Result Table (POST /post/TABLE)

class FiberSectionEventStepTHResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class FiberSectionEventStepTHResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class FiberSectionEventStepTHResultTableRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class FiberSectionEventStepTHResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["FIBR_EVENTSTEP"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: FiberSectionEventStepTHResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: FiberSectionEventStepTHResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: FiberSectionEventStepTHResultTableRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    th_load_case_names: list[str] | None = Field(None, alias="TH_LOAD_CASE_NAMES")
    sect_position: str | None = Field(None, alias="SECT_POSITION")
    components: list[Literal["要素", "断面位置", "荷重", "鉄骨/引張降伏/セルNo.", "鉄骨/引張降伏/epsilon", "鉄骨/引張降伏/時間/ステップ", "鉄骨/圧縮降伏/セルNo.", "鉄骨/圧縮降伏/epsilon", "鉄骨/圧縮降伏/時間/ステップ"]] | None = Field(None, alias="COMPONENTS")

class FiberSectionEventStepTHResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: FiberSectionEventStepTHResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-fiber-section-event-step-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["FiberSectionEventStepTHResultTableRequest"]
