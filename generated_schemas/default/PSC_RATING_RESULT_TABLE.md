# PSC Rating Result Table

PSC rating result table schema authoring reference. The tables are split into Base 15 types and Virtual Beam 15 types. Virtual Beam types use the same filters and columns as the matching Base type, with `_VBEM` appended to `TABLE_TYPE`.

## Column Sets

### `SVC_SUM_DGN`

Used by `PSC_RATING_SVC_SUM_DGN`, `PSC_RATING_SVC_SUM_LEG`, `PSC_RATING_SVC_SUM_DGN_VBEM`, `PSC_RATING_SVC_SUM_LEG_VBEM`.

`Index`, `RatingCase`, `Component`, `MinimumRatingFactor`, `Location`, `RelativeLocation`, `Capacity`, `Demand`, `Point`, `DC/Factor`, `DC/Stress`, `DW/Factor`, `DW/Stress`, `Temperature/Factor`, `Temperature/Stress`, `T.Gradient/Factor`, `T.Gradient/Stress`, `Permanent/Factor`, `Permanent/Stress`, `Secondary/Factor`, `Secondary/Stress`, `UserDefined/Factor`, `UserDefined/Stress`, `Pri.LL/Factor`, `Pri.LL/Stress`, `Adj.LL/Factor`, `Adj.LL/Stress`

### `SVC_SUM_PER`

Used by `PSC_RATING_SVC_SUM_PER`, `PSC_RATING_SVC_SUM_PER_VBEM`.

`Index`, `RatingCase`, `Tendon/Rebar`, `Min.RatingRatio`, `Location`, `RelativeLocation`, `AllowableStress`, `AppliedStress`, `Crack/Uncrack`, `MDemand`, `Mcr`, `Eff.Stress`, `DeadStress`, `LiveStress`

### `STR_SUM_FLEX`

Used by `PSC_RATING_STR_SUM_FLEX`, `PSC_RATING_STR_SUM_FLEX_VBEM`.

`Index`, `RatingCase`, `RatingLevel`, `Posi./Nega.`, `Min.RatingFactor`, `Location`, `RelativeLocation`, `LRFDFactor`, `Capacity`, `Demand`, `DC/Factor`, `DC/Force`, `DW/Factor`, `DW/Force`, `Temperature/Factor`, `Temperature/Force`, `T.Gradient/Factor`, `T.Gradient/Force`, `Permanent/Factor`, `Permanent/Force`, `Secondary/Factor`, `Secondary/Force`, `UserDefined/Factor`, `UserDefined/Force`, `Pri.LL/Factor`, `Pri.LL/Force`, `Adj.LL/Factor`, `Adj.LL/Force`

### `STR_SUM_SHEAR`

Used by `PSC_RATING_STR_SUM_SHEAR`, `PSC_RATING_STR_SUM_SHEAR_VBEM`.

`Index`, `RatingCase`, `RatingLevel`, `Min.RatingFactor`, `Location`, `RelativeLocation`, `LRFDFactor`, `Capacity`, `Demand`, `DC/Factor`, `DC/Force`, `DW/Factor`, `DW/Force`, `Temperature/Factor`, `Temperature/Force`, `T.Gradient/Factor`, `T.Gradient/Force`, `Permanent/Factor`, `Permanent/Force`, `Secondary/Factor`, `Secondary/Force`, `UserDefined/Factor`, `UserDefined/Force`, `Pri.LL/Factor`, `Pri.LL/Force`, `Adj.LL/Factor`, `Adj.LL/Force`

### `STRESS_FACTOR`

Used by `PSC_RATING_STRESS_DGN`, `PSC_RATING_STRESS_LEG`, `PSC_RATING_STRESS_DGN_VBEM`, `PSC_RATING_STRESS_LEG_VBEM`.

`Index`, `Group`, `Elem.`, `Part`, `Girder/Slab`, `RelativeLocation`, `Comp./Tens.`, `RatingCase`, `RatingFactor`, `Check`

### `STRESS_PER`

Used by `PSC_RATING_STRESS_PER`, `PSC_RATING_STRESS_PER_VBEM`.

`Index`, `Group`, `Elem.`, `Part`, `Girder/Slab`, `RelativeLocation`, `RatingCase`, `RatingFactor`, `Check`

### `FLEXURE`

Used by `PSC_RATING_FLEXURE`, `PSC_RATING_FLEXURE_VBEM`.

`Index`, `Group`, `Elem.`, `Part`, `RelativeLocation`, `Positive/Negative`, `RatingCase`, `RatingLevel`, `LRFDResistanceFactor`, `SystemFactor`, `ConditionFactor`, `RatingFactor`, `Check`

### `SHEAR`

Used by `PSC_RATING_SHEAR`, `PSC_RATING_SHEAR_VBEM`.

`Index`, `Group`, `Elem.`, `Part`, `RelativeLocation`, `RatingCase`, `RatingLevel`, `LRFDResistanceFactor`, `SystemFactor`, `ConditionFactor`, `RatingFactor`, `Check`

### `STRESS_DETAIL`

Used by `PSC_RATING_STRESS_DETAIL_DGN`, `PSC_RATING_STRESS_DETAIL_LEG`, `PSC_RATING_STRESS_DETAIL_PER`, and matching `_VBEM` table types.

`Index`, `Group`, `Elem.`, `Part`, `Girder/Slab`, `RelativeLocation`, `RatingCase`, `RatingFactor/C`, `RatingFactor/T`, `Capacity/CStress`, `Capacity/TStress`, `DC/Factor`, `DC/LT`, `DC/RT`, `DC/RB`, `DC/LB`, `DW/Factor`, `DW/LT`, `DW/RT`, `DW/RB`, `DW/LB`, `Temperature/Factor`, `Temperature/LT`, `Temperature/RT`, `Temperature/RB`, `Temperature/LB`, `T.Gradient/Factor`, `T.Gradient/LT`, `T.Gradient/RT`, `T.Gradient/RB`, `T.Gradient/LB`, `Permanent/Factor`, `Permanent/LT`, `Permanent/RT`, `Permanent/RB`, `Permanent/LB`, `Secondary/Factor`, `Secondary/LT`, `Secondary/RT`, `Secondary/RB`, `Secondary/LB`, `UserDefined/Factor`, `UserDefined/LT`, `UserDefined/RT`, `UserDefined/RB`, `UserDefined/LB`, `Pri.LL/Factor`, `Pri.LL/LT`, `Pri.LL/RT`, `Pri.LL/RB`, `Pri.LL/LB`, `Adj.LL/Factor`, `Adj.LL/LT`, `Adj.LL/RT`, `Adj.LL/RB`, `Adj.LL/LB`

### `FLEXURE_DETAIL`

Used by `PSC_RATING_FLEXURE_DETAIL`, `PSC_RATING_FLEXURE_DETAIL_VBEM`.

`Index`, `Group`, `Elem.`, `Part`, `RelativeLocation`, `RatingCase`, `RatingLevel`, `RatingFactor`, `Mn`, `Areas/Rebar`, `Areas/Tendon`, `Rein.Requirement/Min`, `Rein.Requirement/Max`, `DC/Factor`, `DC/Force`, `DW/Factor`, `DW/Force`, `Temperature/Factor`, `Temperature/Force`, `T.Gradient/Factor`, `T.Gradient/Force`, `Permanent/Factor`, `Permanent/Force`, `Secondary/Factor`, `Secondary/Force`, `UserDefined/Factor`, `UserDefined/Force`, `Pri.LL/Factor`, `Pri.LL/Force`, `Adj.LL/Factor`, `Adj.LL/Force`

### `SHEAR_DETAIL`

Used by `PSC_RATING_SHEAR_DETAIL`, `PSC_RATING_SHEAR_DETAIL_VBEM`.

`Index`, `Group`, `Elem.`, `Part`, `RelativeLocation`, `RatingCase`, `RatingLevel`, `RatingFactor`, `Vn`, `DC/Factor`, `DC/Force`, `DW/Factor`, `DW/Force`, `Temperature/Factor`, `Temperature/Force`, `T.Gradient/Factor`, `T.Gradient/Force`, `Permanent/Factor`, `Permanent/Force`, `Secondary/Factor`, `Secondary/Force`, `UserDefined/Factor`, `UserDefined/Force`, `Pri.LL/Factor`, `Pri.LL/Force`, `Adj.LL/Factor`, `Adj.LL/Force`

## Base Table Types

| No. | Name | `TABLE_TYPE` | Filters | Columns | Example |
| --- | --- | --- | --- | --- | --- |
| 1 | Service Limit State Summary (Design Live) | `PSC_RATING_SVC_SUM_DGN` | `RATING_CASE` | `SVC_SUM_DGN` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_SVC_SUM_DGN","RATING_CASE":["drc1"]}}` |
| 2 | Service Limit State Summary (Legal) | `PSC_RATING_SVC_SUM_LEG` | `RATING_CASE` | `SVC_SUM_DGN` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_SVC_SUM_LEG","RATING_CASE":["drc1"]}}` |
| 3 | Service Limit State Summary (Permit) | `PSC_RATING_SVC_SUM_PER` | `RATING_CASE` | `SVC_SUM_PER` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_SVC_SUM_PER","RATING_CASE":["drc1"]}}` |
| 4 | Strength Limit State Summary (Flexural) | `PSC_RATING_STR_SUM_FLEX` | `RATING_CASE` | `STR_SUM_FLEX` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STR_SUM_FLEX","RATING_CASE":["drc1"]}}` |
| 5 | Strength Limit State Summary (Shear) | `PSC_RATING_STR_SUM_SHEAR` | `RATING_CASE` | `STR_SUM_SHEAR` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STR_SUM_SHEAR","RATING_CASE":["drc1"]}}` |
| 6 | Stress Rating Factor (Design Live) | `PSC_RATING_STRESS_DGN` | `NODE_ELEMS`, `PARTS`, `COMP_TENS`, `GIRDER_SLAB` | `STRESS_FACTOR` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STRESS_DGN","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"COMP_TENS":["Tension"],"GIRDER_SLAB":"Girder"}}` |
| 7 | Stress Rating Factor (Legal) | `PSC_RATING_STRESS_LEG` | `NODE_ELEMS`, `PARTS`, `COMP_TENS`, `GIRDER_SLAB` | `STRESS_FACTOR` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STRESS_LEG","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"COMP_TENS":["Compression"],"GIRDER_SLAB":"Girder"}}` |
| 8 | Stress Rating Factor (Permit) | `PSC_RATING_STRESS_PER` | `NODE_ELEMS`, `PARTS`, `GIRDER_SLAB` | `STRESS_PER` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STRESS_PER","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"GIRDER_SLAB":"Girder"}}` |
| 9 | Flexural Strength Rating Factor | `PSC_RATING_FLEXURE` | `NODE_ELEMS`, `PARTS`, `POS_NEG`, `RATING_CASE` | `FLEXURE` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_FLEXURE","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"POS_NEG":["Positive"],"RATING_CASE":["drc1"]}}` |
| 10 | Shear Strength Rating Factor | `PSC_RATING_SHEAR` | `NODE_ELEMS`, `PARTS`, `RATING_CASE` | `SHEAR` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_SHEAR","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"RATING_CASE":["drc1"]}}` |
| 11 | Stress Rating Detail (Design Live) | `PSC_RATING_STRESS_DETAIL_DGN` | `NODE_ELEMS`, `PARTS`, `RATING_CASE`, `GIRDER_SLAB` | `STRESS_DETAIL` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STRESS_DETAIL_DGN","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"RATING_CASE":["drc1"],"GIRDER_SLAB":"Girder"}}` |
| 12 | Stress Rating Detail (Legal) | `PSC_RATING_STRESS_DETAIL_LEG` | `NODE_ELEMS`, `PARTS`, `RATING_CASE`, `GIRDER_SLAB` | `STRESS_DETAIL` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STRESS_DETAIL_LEG","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"RATING_CASE":["drc1"],"GIRDER_SLAB":"Girder"}}` |
| 13 | Stress Rating Detail (Permit) | `PSC_RATING_STRESS_DETAIL_PER` | `NODE_ELEMS`, `PARTS`, `RATING_CASE`, `GIRDER_SLAB` | `STRESS_DETAIL` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STRESS_DETAIL_PER","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"RATING_CASE":["drc1"],"GIRDER_SLAB":"Girder"}}` |
| 14 | Flexural Capacity Demand Ratio | `PSC_RATING_FLEXURE_DETAIL` | `NODE_ELEMS`, `PARTS`, `RATING_CASE` | `FLEXURE_DETAIL` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_FLEXURE_DETAIL","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"RATING_CASE":["drc1"]}}` |
| 15 | Shear Strength Data | `PSC_RATING_SHEAR_DETAIL` | `NODE_ELEMS`, `PARTS`, `RATING_CASE` | `SHEAR_DETAIL` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_SHEAR_DETAIL","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"RATING_CASE":["drc1"]}}` |

## Virtual Beam Table Types

Virtual Beam table types use the same filters and columns as the matching Base table type. They are available for AASHTO models in Virtual Beam analysis mode.

| No. | Name | `TABLE_TYPE` | Filters | Columns | Example |
| --- | --- | --- | --- | --- | --- |
| 1 | Service Limit State Summary (Design Live) | `PSC_RATING_SVC_SUM_DGN_VBEM` | `RATING_CASE` | `SVC_SUM_DGN` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_SVC_SUM_DGN_VBEM","RATING_CASE":["drc1"]}}` |
| 2 | Service Limit State Summary (Legal) | `PSC_RATING_SVC_SUM_LEG_VBEM` | `RATING_CASE` | `SVC_SUM_DGN` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_SVC_SUM_LEG_VBEM","RATING_CASE":["drc1"]}}` |
| 3 | Service Limit State Summary (Permit) | `PSC_RATING_SVC_SUM_PER_VBEM` | `RATING_CASE` | `SVC_SUM_PER` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_SVC_SUM_PER_VBEM","RATING_CASE":["drc1"]}}` |
| 4 | Strength Limit State Summary (Flexural) | `PSC_RATING_STR_SUM_FLEX_VBEM` | `RATING_CASE` | `STR_SUM_FLEX` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STR_SUM_FLEX_VBEM","RATING_CASE":["drc1"]}}` |
| 5 | Strength Limit State Summary (Shear) | `PSC_RATING_STR_SUM_SHEAR_VBEM` | `RATING_CASE` | `STR_SUM_SHEAR` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STR_SUM_SHEAR_VBEM","RATING_CASE":["drc1"]}}` |
| 6 | Stress Rating Factor (Design Live) | `PSC_RATING_STRESS_DGN_VBEM` | `NODE_ELEMS`, `PARTS`, `COMP_TENS`, `GIRDER_SLAB` | `STRESS_FACTOR` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STRESS_DGN_VBEM","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"COMP_TENS":["Tension"],"GIRDER_SLAB":"Girder"}}` |
| 7 | Stress Rating Factor (Legal) | `PSC_RATING_STRESS_LEG_VBEM` | `NODE_ELEMS`, `PARTS`, `COMP_TENS`, `GIRDER_SLAB` | `STRESS_FACTOR` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STRESS_LEG_VBEM","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"COMP_TENS":["Compression"],"GIRDER_SLAB":"Girder"}}` |
| 8 | Stress Rating Factor (Permit) | `PSC_RATING_STRESS_PER_VBEM` | `NODE_ELEMS`, `PARTS`, `GIRDER_SLAB` | `STRESS_PER` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STRESS_PER_VBEM","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"GIRDER_SLAB":"Girder"}}` |
| 9 | Flexural Strength Rating Factor | `PSC_RATING_FLEXURE_VBEM` | `NODE_ELEMS`, `PARTS`, `POS_NEG`, `RATING_CASE` | `FLEXURE` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_FLEXURE_VBEM","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"POS_NEG":["Positive"],"RATING_CASE":["drc1"]}}` |
| 10 | Shear Strength Rating Factor | `PSC_RATING_SHEAR_VBEM` | `NODE_ELEMS`, `PARTS`, `RATING_CASE` | `SHEAR` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_SHEAR_VBEM","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"RATING_CASE":["drc1"]}}` |
| 11 | Stress Rating Detail (Design Live) | `PSC_RATING_STRESS_DETAIL_DGN_VBEM` | `NODE_ELEMS`, `PARTS`, `RATING_CASE`, `GIRDER_SLAB` | `STRESS_DETAIL` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STRESS_DETAIL_DGN_VBEM","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"RATING_CASE":["drc1"],"GIRDER_SLAB":"Girder"}}` |
| 12 | Stress Rating Detail (Legal) | `PSC_RATING_STRESS_DETAIL_LEG_VBEM` | `NODE_ELEMS`, `PARTS`, `RATING_CASE`, `GIRDER_SLAB` | `STRESS_DETAIL` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STRESS_DETAIL_LEG_VBEM","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"RATING_CASE":["drc1"],"GIRDER_SLAB":"Girder"}}` |
| 13 | Stress Rating Detail (Permit) | `PSC_RATING_STRESS_DETAIL_PER_VBEM` | `NODE_ELEMS`, `PARTS`, `RATING_CASE`, `GIRDER_SLAB` | `STRESS_DETAIL` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_STRESS_DETAIL_PER_VBEM","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"RATING_CASE":["drc1"],"GIRDER_SLAB":"Girder"}}` |
| 14 | Flexural Capacity Demand Ratio | `PSC_RATING_FLEXURE_DETAIL_VBEM` | `NODE_ELEMS`, `PARTS`, `RATING_CASE` | `FLEXURE_DETAIL` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_FLEXURE_DETAIL_VBEM","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"RATING_CASE":["drc1"]}}` |
| 15 | Shear Strength Data | `PSC_RATING_SHEAR_DETAIL_VBEM` | `NODE_ELEMS`, `PARTS`, `RATING_CASE` | `SHEAR_DETAIL` | `{"Argument":{"TABLE_TYPE":"PSC_RATING_SHEAR_DETAIL_VBEM","NODE_ELEMS":{"KEYS":[37]},"PARTS":["Part J"],"RATING_CASE":["drc1"]}}` |

## Filter Reference

| Filter | Type | Allowed Values | Examples | Applied `TABLE_TYPE` |
| --- | --- | --- | --- | --- |
| `NODE_ELEMS` | object | `{"KEYS":[int,...]}` or `{"TO":"starttoend"}` range | `"NODE_ELEMS":{"KEYS":[1,2,3]}`<br>`"NODE_ELEMS":{"TO":"1to5"}` | All except summary tables (`SVC_SUM_*`, `STR_SUM_*`) |
| `PARTS` | array of string | `"Part I"`, `"Part J"` | `"PARTS":["Part I"]`<br>`"PARTS":["Part I","Part J"]` | All tables using `NODE_ELEMS` |
| `COMP_TENS` | array of string | `"Compression"`, `"Tension"` | `"COMP_TENS":["Compression"]`<br>`"COMP_TENS":["Tension"]` | `PSC_RATING_STRESS_DGN`, `PSC_RATING_STRESS_LEG`, and matching `_VBEM` types |
| `POS_NEG` | array of string | `"Positive"`, `"Negative"` | `"POS_NEG":["Positive"]`<br>`"POS_NEG":["Positive","Negative"]` | `PSC_RATING_FLEXURE` and `PSC_RATING_FLEXURE_VBEM` |
| `RATING_CASE` | array of string | Model Rating Case label. Substring matching is supported. | `"RATING_CASE":["drc1"]`<br>`"RATING_CASE":["drc2_LEGAL"]` | `PSC_RATING_SVC_SUM_*`, `PSC_RATING_STR_SUM_*`, `PSC_RATING_FLEXURE`, `PSC_RATING_SHEAR`, `PSC_RATING_*_DETAIL_*`, and matching `_VBEM` types |
| `GIRDER_SLAB` | string | `"Girder"`, `"Slab"` | `"GIRDER_SLAB":"Girder"`<br>`"GIRDER_SLAB":"Slab"` | `PSC_RATING_STRESS_DGN`, `PSC_RATING_STRESS_LEG`, `PSC_RATING_STRESS_PER`, `PSC_RATING_STRESS_DETAIL_*`, and matching `_VBEM` types |
