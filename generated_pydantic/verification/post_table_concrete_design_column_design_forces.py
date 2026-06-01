from __future__ import annotations

from typing import Any, Literal
from pydantic import BaseModel, ConfigDict, Field


# Generated from Concrete Design - Column Design Forces (POST /POST/TABLE)

class ConcreteDesignColumnDesignForcesRequestArgumentUNIT(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    force: str | None = Field(None, alias="FORCE")
    dist: str | None = Field(None, alias="DIST")
    heat: str | None = Field(None, alias="HEAT")
    temp: str | None = Field(None, alias="TEMP")

class ConcreteDesignColumnDesignForcesRequestArgumentSTYLES(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    format: Literal["Default", "Fixed", "Scientific", "General"] | None = Field(None, alias="FORMAT")
    place: int | None = Field(None, alias="PLACE")

class ConcreteDesignColumnDesignForcesRequestArgumentNODEELEMS(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    keys: list[int] | None = Field(None, alias="KEYS")
    to: str | None = Field(None, alias="TO")
    structure_group_name: str | None = Field(None, alias="STRUCTURE_GROUP_NAME")

class ConcreteDesignColumnDesignForcesRequestArgument(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    table_name: str = Field("", alias="TABLE_NAME")
    table_type: Literal["string", "COLUMNDESIGNFORCES"] = Field(..., alias="TABLE_TYPE")
    export_path: str | None = Field(None, alias="EXPORT_PATH")
    unit: ConcreteDesignColumnDesignForcesRequestArgumentUNIT | None = Field(None, alias="UNIT")
    styles: ConcreteDesignColumnDesignForcesRequestArgumentSTYLES | None = Field(None, alias="STYLES")
    node_elems: ConcreteDesignColumnDesignForcesRequestArgumentNODEELEMS | None = Field(None, alias="NODE_ELEMS")
    parts: list[str] | None = Field(None, alias="PARTS")
    components: list[Literal["Memb", "Part", "LComName", "Type", "Fx", "Fy", "Fz", "Mx", "My", "Mz"]] | None = Field(None, alias="COMPONENTS")

class ConcreteDesignColumnDesignForcesRequest(BaseModel):
    model_config = ConfigDict(populate_by_name=True, extra='forbid')
    argument: ConcreteDesignColumnDesignForcesRequestArgument = Field(..., alias="Argument")

ENDPOINT_ID = "post-table-concrete-design-column-design-forces"
DEFAULT_METHOD = "POST"

from generated_pydantic.client import endpoint as _endpoint

_command = _endpoint(ENDPOINT_ID, method=DEFAULT_METHOD)
build_payload = _command.build_payload
run = _command.run

__all__ = ["ConcreteDesignColumnDesignForcesRequest"]
