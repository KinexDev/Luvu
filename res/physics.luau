local sphere: Texture = Texture.Load("res/sphere.png")
local outline: Texture = Texture.Load("res/sphereOutline.png")
local size: number = 20
-- this is O(n2)
local ballCount: number = 75
local bounce: number = 0.5

local rigidBodyList: { Rigidbody } = {}

type Rigidbody = {
    Radius: number,
    Position: Vec2,
    Velocity: Vec2,
    Color: Color
}

function NewRigidbody(Radius: number, Position: Vec2, Color: Color): Rigidbody
    return { Radius = Radius, Position = Position, Color = Color, Velocity = Vec2.New(math.random(300) / 100, math.random(300) / 100) }
end

for i = 1, ballCount do
    rigidBodyList[#rigidBodyList + 1] = NewRigidbody(math.random(50, 150) / 100, 
        Vec2.New(math.random(1, size - 2), math.random(1, size - 4)), Color.New(math.random(50, 80) / 100, math.random(80) / 100, math.random(25, 80) / 100, 1))
end

function _update(dt: number)
    local scale: number = (Graphics.GetHeight()) / size
    local mousePosition: Vec2 = Input.GetMousePosition()
    local scaledMousePosition: Vec2 = Vec2.New(mousePosition.x / scale, mousePosition.y / scale)

    for i = 1, #rigidBodyList do
        local x: Rigidbody = rigidBodyList[i]
        
        if Input.GetMouseButton(1) then 
            local dir: Vec2 = (x.Position - scaledMousePosition):Normalize()
            x.Velocity -= dir * 55 * dt
        else 
            x.Velocity += Vec2.New(0, 9.8) * dt
        end
        
        if x.Position.x > size - (x.Radius / 2) then
            x.Velocity.x = -x.Velocity.x * bounce
            x.Position.x = size - (x.Radius / 2) - 0.05
        end

        if x.Position.x < 0 then
            x.Velocity.x = -x.Velocity.x * bounce
            x.Position.x = 0.05
        end
    
        if x.Position.y > size - (x.Radius / 2) then
            x.Velocity.y = -x.Velocity.y * bounce
            x.Position.y = size - (x.Radius / 2) - 0.05
        end

        if x.Position.y < 0 then
            x.Velocity.y = -x.Velocity.y * bounce
            x.Position.y = 0.05
        end

        for j = 1, #rigidBodyList do
            if i == j then
                continue
            end
            local y: Rigidbody = rigidBodyList[j]

            if Vec2.Dist(x.Position, y.Position) < (y.Radius + x.Radius) / 2 then
                local dir: Vec2 = (x.Position - y.Position):Normalize()
                
                local overlap: number = (y.Radius + x.Radius) / 2 - Vec2.Dist(x.Position, y.Position)
                x.Position = x.Position + dir * overlap / 2
                y.Position = y.Position - dir * overlap / 2
                local velocityAlongNormal: number = Vec2.Dot(x.Velocity - y.Velocity, dir)

                if velocityAlongNormal < 0 then
                    local impulse: number = velocityAlongNormal / (x.Radius + y.Radius)
                    x.Velocity = x.Velocity - dir * impulse * bounce
                    y.Velocity = y.Velocity + dir * impulse * bounce
                end
            end
        end

        if Input.GetMouseButton(0) then
            if Vec2.Dist(x.Position, scaledMousePosition) < (x.Radius + 2) / 2 then
                local dir: Vec2 = (x.Position - scaledMousePosition):Normalize() * 2
                x.Velocity += dir * 1 * x.Radius
            end 
        end

        x.Position += x.Velocity * dt
    end
    Graphics.SetWindowTitle("Physics (" .. math.round(1 / dt) .. " fps)")
end

local color: Color = Color.New(1, 1, 1, 0.1)
local white: Color = Color.New(1, 1, 1, 1)

function _draw(dt: number)
    local scale: number = (Graphics.GetHeight()) / size
    for x = 0, size do
        for y = 0, size do
            if (x + y) % 2 == 0 then
                Graphics.DrawRectangle(Vec2.New((scale / 2) + scale * x, (scale / 2) + scale * y), 0, Vec2.New(scale, scale), color) 
            end
        end
    end

    if Input.GetMouseButton(0) then
        local mousePosition: Vec2 = Input.GetMousePosition()
        Graphics.DrawImage(sphere, mousePosition, 0, Vec2.New(scale, scale) * 2, Color.New(1, 0.3, 0.3, 0.5))
    end
    
    if Input.GetMouseButton(1) then
        local mousePosition: Vec2 = Input.GetMousePosition()
        Graphics.DrawImage(sphere, mousePosition, 0, Vec2.New(scale, scale) * 5, Color.New(0.3, 0.3, 1, 0.5))
    end

    for i = 1, #rigidBodyList do
        local x: Rigidbody = rigidBodyList[i]
        Graphics.DrawImage(sphere, Vec2.New((scale / 2) + scale * x.Position.x, (scale / 2) + scale * x.Position.y), 0, Vec2.New(scale, scale) * x.Radius, x.Color)
        Graphics.DrawImage(outline, Vec2.New((scale / 2) + scale * x.Position.x, (scale / 2) + scale * x.Position.y), 0, Vec2.New(scale, scale) * x.Radius, white)
    end
end
