local fov: number = 200
local width: number = 1
local near: number = 0.03
local far: number = 1000
local angle: number = 0

Graphics.SetWindowTitle("3d example")

type Vec3 = {
    x: number,
    y: number,
    z: number
}

local Vec3 = {
    New = function(x: number, y: number, z: number): Vec3
        return { x = x, y = y, z = z }
    end,
    
    Add = function(v1: Vec3, v2: Vec3): Vec3
        return { x = v1.x + v2.x, y = v1.y + v2.y, z = v1.z + v2.z }
    end,
}
function Vec3.Rotate(v: Vec3, rotation: Vec3): Vec3
    local radY: number = math.rad(rotation.y)
    local cosY: number = math.cos(radY)
    local sinY: number = math.sin(radY)
    local rotatedY: Vec3 = Vec3.New(
        v.x * cosY + v.z * sinY,
        v.y,
        -v.x * sinY + v.z * cosY
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
    position: Vec3
}

local camera: Camera = { position = Vec3.New(0, 0, -5) }

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


function project(p: Vec3): Vec2
    local factor: number = fov / (math.clamp(p.z, -far, -near))
    return Vec2.New(p.x * factor, p.y * factor)
end

-- i only got near working
function isValid(p: Vec3): boolean
    return p.z < 0
end

function _update(dt: number)
    angle += 90 * dt
    angle %= 360
end

function _draw(dt: number)
    local center: Vec2 = Vec2.New(Graphics.GetWidth() / 2, Graphics.GetHeight() / 2)

    local transformedVertexs: { Vec2 } = {}
    local validVertexes: { boolean } = {}
    for k, v in ipairs(cube.vertices) do
        local vertex: Vec3 = Vec3.Rotate(v, Vec3.New(angle, angle, angle))
        vertex = Vec3.Add(vertex, camera.position)
        transformedVertexs[k] = project(vertex) + center
        validVertexes[k] = isValid(vertex)
    end

    for _, v in ipairs(cube.indicies) do
        if not validVertexes[v[1]] and not validVertexes[v[2]] then
            continue
        end
        Graphics.DrawLine(transformedVertexs[v[1]], transformedVertexs[v[2]], width, Color.New(1, 1, 1, 1))
    end
end