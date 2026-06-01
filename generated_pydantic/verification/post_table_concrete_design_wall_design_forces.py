from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Concrete Design - Wall Design Forces (POST /POST/TABLE)

class ConcreteDesignWallDesignForcesRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class ConcreteDesignWallDesignForcesRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class ConcreteDesignWallDesignForcesRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class ConcreteDesignWallDesignForcesRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["string", "WALLDESIGNFORCES"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: ConcreteDesignWallDesignForcesRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: ConcreteDesignWallDesignForcesRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: ConcreteDesignWallDesignForcesRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    parts: list[str] | None = Field(None, alias="PARTS")
    story_names: list[str] | None = Field(None, alias="STORY_NAMES")
    components: list[Literal["WID", "Story", "Part", "LComName", "Type", "Fx", "Fy", "Fz", "Mx", "My", "Mz"]] | None = Field(None, alias="COMPONENTS")

class ConcreteDesignWallDesignForcesRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: ConcreteDesignWallDesignForcesRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-concrete-design-wall-design-forces"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["ConcreteDesignWallDesignForcesRequest"]
