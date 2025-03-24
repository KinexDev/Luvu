local fov: number = 500
local width: number = 2
local moveSpeed: number = 5
local near: number = 0.01
local far: number = 1000
local sensitivity: number = 30

Graphics.SetWindowTitle("3D Example")
Graphics.SetWindowResizable(true)

type Vec3 = {
    x: number,
    y: number,
    z: number
}

local Vec3 = {
    New = function(x: number, y: number, z: number): Vec3
        return { x = x, y = y, z = z }
    end,
    
    Add = function(a: Vec3, b: Vec3): Vec3
        return { x = a.x + b.x, y = a.y + b.y, z = a.z + b.z }
    end,
        
    Mul = function(a: Vec3, b: Vec3): Vec3
        return { x = a.x * b.x, y = a.y * b.y, z = a.z * b.z }
    end,
    
    Scale = function(a: Vec3, b: number): Vec3
        return { x = a.x * b, y = a.y * b, z = a.z * b }    
    end,

    Distance = function(a: Vec3, b: Vec3): number
        return math.sqrt((b.x - a.x)^2 + (b.y - a.y)^2 + (b.z - a.z)^2)
    end,
    
    Negate = function(a: Vec3): Vec3
        return { x = -a.x, y = -a.y, z = -a.z }
    end,
    
    Length = function(a: Vec3): number
        return math.sqrt(a.x^2 + a.y^2 + a.z^2)
    end
}

function Vec3.Rotate(a: Vec3, rotation: Vec3): Vec3
    local radY: number = math.rad(rotation.y)
    local cosY: number = math.cos(radY)
    local sinY: number = math.sin(radY)
    local rotatedY: Vec3 = Vec3.New(
        a.x * cosY + a.z * sinY,
        a.y,
        -a.x * sinY + a.z * cosY
    )

    local radX: number = math.rad(rotation.x)
    local cosX: number = math.cos(radX)
    local sinX: number = math.sin(radX)
    local rotatedX: Vec3 = Vec3.New(
        rotatedY.x,
        rotatedY.y * cosX - rotatedY.z * sinX,
        rotatedY.y * sinX + rotatedY.z * cosX
    )

    local radZ: number = math.rad(rotation.z)
    local cosZ: number = math.cos(radZ)
    local sinZ: number = math.sin(radZ)
    return Vec3.New(
        rotatedX.x * cosZ - rotatedX.y * sinZ,
        rotatedX.x * sinZ + rotatedX.y * cosZ,
        rotatedX.z
    )
end

function Vec3.Normalise(a: Vec3): Vec3
    local length: number = Vec3.Length(a)
    
    if length == 0 then
        return a
    end

    return Vec3.Scale(a, 1 / length)
end

type Mesh = {
    vertices: { Vec3 },
    indicies: { {number} }
}

local Mesh = {
    New = function(vertices: { Vec3 }, indicies: { { number } }): Mesh
        return { vertices = vertices, indicies = indicies }
    end
}

type Camera = {
    position: Vec3,
    rotation: Vec3
}

type GameObject = {
    type: number,
    mesh: Mesh,
    position: Vec3,
    scale: Vec3,
    color: Color,
    logic: (go: GameObject, dt: number) -> ()
}

type Scene = {
    gameObjects: { GameObject },
    index: number
}

local GoType = {
    Regular = 0 :: number,
    End = 1 :: number,
    Death = 2 :: number,
    Bounce = 3 :: number,
    Player = 4 :: number
}

local SceneManager = {
    scenes = {} :: { Scene },
    currentScene = { gameObjects = {} } :: Scene
}

function SceneManager.AddScene(sceneCreator : (index: number) -> Scene) 
    local scene: Scene = sceneCreator(#SceneManager.scenes + 1)
    SceneManager.scenes[#SceneManager.scenes + 1] = scene
end

function SceneManager.LoadScene(id: number)
    if SceneManager.scenes[id] then
        SceneManager.currentScene = SceneManager.scenes[id]
    else
        quit()
    end
end

function SceneManager.Update(dt: number)
    for gok, go in ipairs(SceneManager.currentScene.gameObjects) do
        go.logic(go, dt)
    end
end

local GameObject = {
    New = function(goTable: { GameObject }, type: number, mesh: Mesh, position: Vec3, scale: Vec3, color: Color, goLogic: (go: GameObject, dt: number) -> ()): GameObject
        local go: GameObject = { type = type, mesh = mesh, position = position, scale = scale, color = color, logic = goLogic }
        table.insert(goTable, go)
        return go
    end,
    NewNoLogic = function(goTable: { GameObject }, type: number, mesh: Mesh, position: Vec3, scale: Vec3, color: Color): GameObject
        local go: GameObject = { type = type, mesh = mesh, position = position, scale = scale, color = color, logic = function() end }
        table.insert(goTable, go)
        return go
    end
}

local camera: Camera = { position = Vec3.New(0, 0, -5), rotation = Vec3.New(0, 0, 0) }
local lastMousePosition: Vec2 = Input.GetMousePosition()


local playerY: number = 0
local canJump: boolean = false
local spacePressed: boolean = false

function checkAABBCollision(go1: GameObject, go2: GameObject): boolean
    local min1 = Vec3.New(go1.position.x - go1.scale.x / 2, go1.position.y - go1.scale.y / 2, go1.position.z - go1.scale.z / 2)
    local max1 = Vec3.New(go1.position.x + go1.scale.x / 2, go1.position.y + go1.scale.y / 2, go1.position.z + go1.scale.z / 2)

    local min2 = Vec3.New(go2.position.x - go2.scale.x / 2, go2.position.y - go2.scale.y / 2, go2.position.z - go2.scale.z / 2)
    local max2 = Vec3.New(go2.position.x + go2.scale.x / 2, go2.position.y + go2.scale.y / 2, go2.position.z + go2.scale.z / 2)

    return not (max1.x < min2.x or max1.y < min2.y or max1.z < min2.z or min1.x > max2.x or min1.y > max2.y or min1.z > max2.z)
end

function resolveAABBCollision(go1: GameObject, go2: GameObject, dt: number): boolean
    local colliding: boolean = checkAABBCollision(go1, go2)

    if colliding then
        local overlapX: number = math.min(go1.position.x + go1.scale.x / 2, go2.position.x + go2.scale.x / 2) - math.max(go1.position.x - go1.scale.x / 2, go2.position.x - go2.scale.x / 2)
        local overlapY: number = math.min(go1.position.y + go1.scale.y / 2, go2.position.y + go2.scale.y / 2) - math.max(go1.position.y - go1.scale.y / 2, go2.position.y - go2.scale.y / 2)
        local overlapZ: number = math.min(go1.position.z + go1.scale.z / 2, go2.position.z + go2.scale.z / 2) - math.max(go1.position.z - go1.scale.z / 2, go2.position.z - go2.scale.z / 2)

        if overlapX < overlapY and overlapX < overlapZ then
            if go1.position.x < go2.position.x then
                go1.position.x = go1.position.x - overlapX
            else
                go1.position.x = go1.position.x + overlapX
            end
            canJump = false
        elseif overlapY < overlapX and overlapY < overlapZ then
            if go1.position.y < go2.position.y then
                go1.position.y = go1.position.y - overlapY
            else
                go1.position.y = go1.position.y + overlapY
            end
            playerY = -2
            canJump = true
        else
            if go1.position.z < go2.position.z then
                go1.position.z = go1.position.z - overlapZ
            else
                go1.position.z = go1.position.z + overlapZ
            end
            canJump = false
        end
    end

    return colliding
end

function playerLogic(go: GameObject, dt: number)
    playerY -= 9.81 * dt
    math.clamp(playerY, -10, 0)
    go.position.y += playerY * dt

    local yOnly: Vec3 = Vec3.New(0, camera.rotation.y, 0)
    local forwardDirection = Vec3.Rotate(Vec3.New(0, 0, -1), yOnly)
    local rightDirection = Vec3.Rotate(Vec3.New(1, 0, 0), yOnly)
    local velocity: Vec3 = Vec3.New(0, 0, 0)

    if Input.GetKey(Key.A) then
        velocity = Vec3.Add(velocity, rightDirection)
    end
    if Input.GetKey(Key.D) then
        velocity = Vec3.Add(velocity, Vec3.Negate(rightDirection))
    end
    if Input.GetKey(Key.W) then
        velocity = Vec3.Add(velocity, forwardDirection)
    end  
    if Input.GetKey(Key.S) then
        velocity = Vec3.Add(velocity, Vec3.Negate(forwardDirection))
    end

    velocity = Vec3.Scale(Vec3.Normalise(velocity), moveSpeed * dt)
    go.position = Vec3.Add(go.position, velocity)

    for _, otherGo in ipairs(SceneManager.currentScene.gameObjects) do
        if otherGo ~= go then
            if resolveAABBCollision(go, otherGo, dt) then
                if Input.GetKey(Key.Space) and canJump then
                    if not spacePressed then
                        playerY = 5
                    end
                    spacePressed = true
                else 
                    spacePressed = false
                end
                
                if otherGo.type == GoType.End then
                    playerY = 0
                    SceneManager.LoadScene(SceneManager.currentScene.index + 1)                    
                end

                if otherGo.type == GoType.Death then
                    go.position.x = 0
                    go.position.y = 0
                    go.position.z = 5     
                    playerY = 0 
                end

                if otherGo.type == GoType.Bounce then
                    playerY = 10
                end
            end
        end
    end

    if go.position.y < -4 then
        go.position.x = 0
        go.position.y = 0
        go.position.z = 5
        playerY = 0
    end

    camera.position = Vec3.Add(go.position, Vec3.New(0, 0.5, 0))
end

local cube: Mesh = Mesh.New({
    Vec3.New(-1, -1, -1),
    Vec3.New(1, -1, -1),
    Vec3.New(1, 1, -1),
    Vec3.New(-1, 1, -1),
    Vec3.New(1, -1, 1),
    Vec3.New(1, 1, 1),
    Vec3.New(-1, 1, 1),
    Vec3.New(-1, -1, 1)
}, {
    {1, 2}, {2, 3}, {3, 4}, {4, 1},
    {5, 6}, {6, 7}, {7, 8}, {8, 5},
    {1, 8}, {4, 7}, {2, 5}, {3, 6}
})

SceneManager.AddScene(function(index)  
    local gameObjects: { GameObject } = {}
    GameObject.NewNoLogic(gameObjects, GoType.End, cube, Vec3.New(-4, -3, -9), Vec3.New(2, 1, 2), Color.New(0, 1, 0, 1))
    GameObject.NewNoLogic(gameObjects, GoType.Regular, cube, Vec3.New(0, -2, 5), Vec3.New(2, 1, 2), Color.New(1, 1, 1, 1))
    GameObject.NewNoLogic(gameObjects, GoType.Regular, cube, Vec3.New(0, -1, 0), Vec3.New(1, 1, 1), Color.New(1, 1, 1, 1))
    GameObject.NewNoLogic(gameObjects, GoType.Regular, cube, Vec3.New(0, 0, -4), Vec3.New(1, 1, 1), Color.New(1, 1, 1, 1))
    GameObject.NewNoLogic(gameObjects, GoType.Regular, cube, Vec3.New(-4, 0, -5.5), Vec3.New(1, 1, 1), Color.New(1, 1, 1, 1))    
    GameObject.New(gameObjects, GoType.Player, cube, Vec3.New(0, 0, 5), Vec3.New(1, 2, 1), Color.New(0, 0, 1, 1), playerLogic)    

    local scene: Scene = { gameObjects = gameObjects, index = index}
    return scene
end)

SceneManager.AddScene(function(index)  
    local gameObjects: { GameObject } = {}
    GameObject.NewNoLogic(gameObjects, GoType.End, cube, Vec3.New(0, 6.5, 3), Vec3.New(2, 1, 2), Color.New(0, 1, 0, 1))
    GameObject.NewNoLogic(gameObjects, GoType.Regular, cube, Vec3.New(0, -2, 5), Vec3.New(2, 1, 2), Color.New(1, 1, 1, 1))
    GameObject.NewNoLogic(gameObjects, GoType.Regular, cube, Vec3.New(0, 1.5, -5), Vec3.New(3, 1, 3), Color.New(1, 1, 1, 1))
    GameObject.NewNoLogic(gameObjects, GoType.Regular, cube, Vec3.New(0, 7, -4), Vec3.New(3, 1, 4), Color.New(1, 1, 1, 1))
    GameObject.NewNoLogic(gameObjects, GoType.Bounce, cube, Vec3.New(0, -2, 0), Vec3.New(1, 1, 1), Color.New(1, 1, 0, 1))    
    GameObject.NewNoLogic(gameObjects, GoType.Bounce, cube, Vec3.New(0, 3, -8), Vec3.New(1, 1, 1), Color.New(1, 1, 0, 1))    
    GameObject.New(gameObjects, GoType.Player, cube, Vec3.New(0, 0, 5), Vec3.New(1, 1, 1), Color.New(0, 0, 1, 1), playerLogic)    

    local scene: Scene = { gameObjects = gameObjects, index = index}
    return scene
end)

SceneManager.LoadScene(1)

function project(p: Vec3): Vec2
    local factor: number = fov / (math.clamp(p.z, near, far))
    return Vec2.New(p.x * factor, p.y * factor)
end

function isValid(p: Vec3): boolean
    return p.z > 0
end

function _update(dt: number)
    SceneManager.Update(dt)

    Graphics.SetWindowTitle("3D Example (".. math.round(1 / dt) .. " fps)")

    local delta: Vec2 = (Input.GetMousePosition() - lastMousePosition) * dt * sensitivity
    camera.rotation.x = math.clamp(camera.rotation.x - delta.y, -90, 90)
    camera.rotation.y += delta.x
    lastMousePosition = Input.GetMousePosition()
end

function _draw(dt: number)
    local center: Vec2 = Vec2.New(Graphics.GetWidth() / 2, Graphics.GetHeight() / 2)

    local transformedVertexs: { Vec2 } = {}
    local validVertexes: { boolean } = {}
    
    for gok, go in ipairs(SceneManager.currentScene.gameObjects) do
        if go.type == GoType.Player then
            continue
        end

        for k, v in ipairs(go.mesh.vertices) do
            local vertex: Vec3 = Vec3.Mul(v, Vec3.Scale(go.scale, 0.5))
            vertex = Vec3.Add(vertex, Vec3.Negate(go.position))
            vertex = Vec3.Add(vertex, camera.position)
            vertex = Vec3.Rotate(vertex, Vec3.Negate(camera.rotation))
            transformedVertexs[k] = project(vertex) + center
            validVertexes[k] = isValid(vertex)
        end

        for _, v in ipairs(go.mesh.indicies) do
            if not validVertexes[v[1]] and not validVertexes[v[2]] then
                continue
            end
            Graphics.DrawLine(transformedVertexs[v[1]], transformedVertexs[v[2]], width, go.color)
        end
    end
end