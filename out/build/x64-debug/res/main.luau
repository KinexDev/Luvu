local sphere: Texture = Texture.Load("sphere.png")
local size: number = 20
local ballCount: number = 10
Graphics.SetWindowTitle("Physics")
Graphics.SetWindowPos(Vec2.New((1920 / 2) - 300, (1080 / 2) - 300))

local rigidBodyList = {}

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
    rigidBodyList[#rigidBodyList + 1] = NewRigidbody(math.random(50, 100) / 100, 
        Vec2.New(math.random(1, size - 2), math.random(1, size - 4)), Color.New(math.random(50, 80) / 100, math.random(80) / 100, math.random(25, 80) / 100, 1))
end

function _update(dt: number)
    for i = 1, #rigidBodyList do
        local x: Rigidbody = rigidBodyList[i]
        x.Velocity += Vec2.New(0, 9.8) * dt
        
        if x.Position.x > size - 1 * (x.Radius * 1) then
            x.Velocity.x = -x.Velocity.x
            x.Position.x -= dt
        end

        if x.Position.x < 0 then
            x.Velocity.x = -x.Velocity.x
            x.Position.x += dt
        end
    
        if x.Position.y > size - 1 * (x.Radius * 1) then
            x.Velocity.y = -x.Velocity.y 
            x.Position.y -= dt
        end

        if x.Position.y < 0 then
            x.Velocity.y = -x.Velocity.y 
            x.Position.x += dt
        end

        for j = 1, #rigidBodyList do
            if i == j then
                continue
            end
            local y: Rigidbody = rigidBodyList[j]

            if Vec2.Dist(x.Position, y.Position) < (y.Radius + x.Radius) / 2 then   
                local dir: Vec2 = (x.Position - y.Position):Normalize()
                x.Velocity = Vec2.Lerp(x.Velocity:Normalize(), dir, y.Radius):Normalize() * x.Velocity:Length()

                dir = (y.Position - x.Position):Normalize()
                y.Velocity = Vec2.Lerp(y.Velocity:Normalize(), dir, x.Radius):Normalize() * y.Velocity:Length()
            end
        end
        x.Position += x.Velocity * dt
    end
end

function _draw(dt: number)
    local scale: number = (Graphics.GetHeight()) / size
    for x = 0, size do
        for y = 0, size do
            Graphics.DrawRectangle(Vec2.New((scale / 2) + scale * x, (scale / 2) + scale * y), 0, Vec2.New(scale, scale), if (x + y) % 2 == 0 then Color.New(0, 0, 1, 1) else Color.New(1, 1, 0, 1))
        end
    end

    for i = 1, #rigidBodyList do
        local x: Rigidbody = rigidBodyList[i]
        Graphics.DrawImage(sphere, Vec2.New((scale / 2) + scale * x.Position.x, (scale / 2) + scale * x.Position.y), 0, Vec2.New(scale, scale) * x.Radius, x.Color)
    end
end
