from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Inelastic Hinge General Link Summary - TH Result Table (POST /post/TABLE)

class InelasticHingeGeneralLinkSummaryTHResultTableRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class InelasticHingeGeneralLinkSummaryTHResultTableRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class InelasticHingeGeneralLinkSummaryTHResultTableRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["IEHG_GL_LINK_SUM_DX", "IEHG_GL_LINK_SUM_DY", "IEHG_GL_LINK_SUM_DZ", "IEHG_GL_LINK_SUM_RX", "IEHG_GL_LINK_SUM_RY", "IEHG_GL_LINK_SUM_RZ"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: InelasticHingeGeneralLinkSummaryTHResultTableRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: InelasticHingeGeneralLinkSummaryTHResultTableRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    th_load_case_names: list[str] | None = Field(None, alias="TH_LOAD_CASE_NAMES")
    components: list[Literal["GeneralLink/No", "GeneralLink/Prop.", "GeneralLink/Node1", "GeneralLink/Node2", "InelasticHingeProp.", "Load", "Time/Step", "Deform", "Force", "max(D/D1)", "max(D/D2)", "Status", "Performance", "P1", "P2", "P3", "D1", "D2", "D3"]] | None = Field(None, alias="COMPONENTS")

class InelasticHingeGeneralLinkSummaryTHResultTableRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: InelasticHingeGeneralLinkSummaryTHResultTableRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-inelastic-hinge-general-link-summary-th-result-table"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["InelasticHingeGeneralLinkSummaryTHResultTableRequest"]
