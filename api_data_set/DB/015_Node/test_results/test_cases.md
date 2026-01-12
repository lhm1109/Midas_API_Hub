# Node API Test Cases

## Test Suite for Node API

### Endpoint
`POST/GET/PUT/DELETE /api/db/NODE`

### Test Cases

#### 1. POST - Create Single Node
**Request:**
```json
{
  "Assign": {
    "1": {
      "X": 0,
      "Y": 0,
      "Z": 0
    }
  }
}
```
**Expected:** Success, node created at origin

#### 2. POST - Create Multiple Nodes
**Request:**
```json
{
  "Assign": {
    "1": { "X": -1, "Y": -1, "Z": -1 },
    "2": { "X": -2, "Y": -2, "Z": -2 },
    "3": { "X": -3, "Y": -3, "Z": -3 }
  }
}
```
**Expected:** Success, 3 nodes created

#### 3. POST - Create Node with Default Values
**Request:**
```json
{
  "Assign": {
    "10": {}
  }
}
```
**Expected:** Success, node created at (0,0,0) with defaults

#### 4. GET - Retrieve All Nodes
**Request:** GET without parameters
**Expected:** Returns all nodes

#### 5. GET - Retrieve Specific Node
**Request:** GET with nodeId=1
**Expected:** Returns node #1 data

#### 6. PUT - Update Node Position
**Request:**
```json
{
  "Assign": {
    "1": {
      "X": 100,
      "Y": 200,
      "Z": 300
    }
  }
}
```
**Expected:** Node #1 updated to new position

#### 7. DELETE - Remove Node
**Request:**
```json
{
  "nodeIds": ["1"]
}
```
**Expected:** Node #1 deleted

#### 8. Error Test - Invalid Type
**Request:**
```json
{
  "Assign": {
    "1": {
      "X": "invalid",
      "Y": 0,
      "Z": 0
    }
  }
}
```
**Expected:** Error - X must be number

#### 9. Boundary Test - Large Coordinates
**Request:**
```json
{
  "Assign": {
    "999": {
      "X": 999999.999,
      "Y": -999999.999,
      "Z": 999999.999
    }
  }
}
```
**Expected:** Success or reasonable error handling

#### 10. Boundary Test - Zero Coordinates
**Request:**
```json
{
  "Assign": {
    "5": {
      "X": 0,
      "Y": 0,
      "Z": 0
    }
  }
}
```
**Expected:** Success, node at origin

