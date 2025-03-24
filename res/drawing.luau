-- this is really unoptimized.
local previousPositions: { Dot } = {}

type Dot = {
    stop: boolean,
    color: Color,
    pos: Vec2,
    radius: number
}

function NewDot(pos: Vec2, color: Color, stop: boolean, radius: number): Dot
    return { stop = stop, pos = pos, color = color, radius = radius }
end

Graphics.SetWindowTitle("Drawing")

local heldDown = false
local color = Color.New(1, 1, 1, 1)

local updateTime = 0
local updateFrequency = 1 / 50

local keyColors : {[number]: Color} = {
    [Key.Key1] = Color.New(1, 1, 1, 1),
    [Key.Key2] = Color.New(1, 0, 0, 1),
    [Key.Key3] = Color.New(0, 1, 0, 1),
    [Key.Key4] = Color.New(0, 0, 1, 1),
    [Key.Key5] = Color.New(0, 0, 0, 1)
}

function _update(dt: number)
    if Input.GetKey(Key.O) then
        table.clear(previousPositions)
    end
    
    for key, col in pairs(keyColors) do
        if Input.GetKey(key) then
            color = col
            break
        end
    end

    if Input.GetMouseButton(0) then
        if updateTime > updateFrequency then
            local dot: Dot = NewDot(Input.GetMousePosition(), color, not heldDown, 3)
            previousPositions[#previousPositions + 1] = dot
            heldDown = true
            updateTime = 0
        end
    else
        heldDown = false
    end
    
    updateTime += dt
end

function _draw()
    Graphics.DrawRectangle(Vec2.New(15, 15), 0, Vec2.New(15, 15), color)

    for i = 1, #previousPositions do
        local dot: Dot = previousPositions[i]

        if i + 1 <= #previousPositions then
            local dot2: Dot = previousPositions[i + 1]
            
            if not dot2.stop then
                Graphics.DrawLine(dot.pos, dot2.pos, dot.radius, dot.color)
                continue
            end
        end
        
        Graphics.DrawRectangle(dot.pos, 0, Vec2.New(dot.radius, dot.radius), dot.color)
    end
end
