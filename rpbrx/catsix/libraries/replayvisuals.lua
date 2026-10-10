local ReplayVisuals = {}
local Properties = {
    GuiObject = {"Name", "Position", "Size", "AnchorPoint", "Rotation", "Visible", "ZIndex", "LayoutOrder", "BackgroundColor3", "BackgroundTransparency", "BorderSizePixel", "BorderColor3", "ClipsDescendants", "AutomaticSize", "SizeConstraint"},
    ScreenGui = {"Name", "IgnoreGuiInset", "DisplayOrder", "ZIndexBehavior", "Enabled", "ResetOnSpawn"},
    TextLabel = {"Text", "FontFace", "TextSize", "TextColor3", "TextTransparency", "TextStrokeColor3", "TextStrokeTransparency", "TextXAlignment", "TextYAlignment", "TextWrapped", "TextScaled", "RichText", "TextTruncate", "LineHeight"},
    ImageLabel = {"Image", "ImageColor3", "ImageTransparency", "ImageRectOffset", "ImageRectSize", "ScaleType", "SliceCenter", "SliceScale", "TileSize", "ResampleMode"},
    UICorner = {"CornerRadius"},
    UIStroke = {"Color", "Thickness", "Transparency", "ApplyStrokeMode", "LineJoinMode", "Enabled"},
    UIListLayout = {"FillDirection", "HorizontalAlignment", "VerticalAlignment", "SortOrder", "Padding", "HorizontalFlex", "VerticalFlex", "ItemLineAlignment", "Wraps"},
    UIGridLayout = {"CellPadding", "CellSize", "FillDirection", "FillDirectionMaxCells", "HorizontalAlignment", "VerticalAlignment", "SortOrder", "StartCorner"},
    UIPadding = {"PaddingTop", "PaddingBottom", "PaddingLeft", "PaddingRight"},
    UIScale = {"Scale"},
    UIAspectRatioConstraint = {"AspectRatio", "AspectType", "DominantAxis"},
    UISizeConstraint = {"MinSize", "MaxSize"},
    UITextSizeConstraint = {"MinTextSize", "MaxTextSize"},
    UIGradient = {"Color", "Transparency", "Rotation", "Offset", "Enabled"},
    Decal = {"Name", "Texture", "Color3", "Transparency", "Face", "ZIndex"},
    Texture = {"StudsPerTileU", "StudsPerTileV", "OffsetStudsU", "OffsetStudsV"},
    SpecialMesh = {"MeshType", "MeshId", "TextureId", "Scale", "Offset", "VertexColor"},
    SurfaceAppearance = {"AlphaMode", "ColorMap", "MetalnessMap", "NormalMap", "RoughnessMap"}
}

local function Encode(Value)
    local Kind: string = typeof(Value)
    if Kind == "string" or Kind == "number" or Kind == "boolean" then return Value end
    if Kind == "Color3" then return {Type = Kind, Value = {Value.R, Value.G, Value.B}} end
    if Kind == "Vector2" then return {Type = Kind, Value = {Value.X, Value.Y}} end
    if Kind == "Vector3" then return {Type = Kind, Value = {Value.X, Value.Y, Value.Z}} end
    if Kind == "UDim" then return {Type = Kind, Value = {Value.Scale, Value.Offset}} end
    if Kind == "UDim2" then return {Type = Kind, Value = {Value.X.Scale, Value.X.Offset, Value.Y.Scale, Value.Y.Offset}} end
    if Kind == "Rect" then return {Type = Kind, Value = {Value.Min.X, Value.Min.Y, Value.Max.X, Value.Max.Y}} end
    if Kind == "EnumItem" then return {Type = Kind, Enum = tostring(Value.EnumType):gsub("Enum%.", ""), Value = Value.Name} end
    if Kind == "Font" then return {Type = Kind, Value = {Value.Family, Value.Weight.Name, Value.Style.Name}} end
    if Kind == "ColorSequence" or Kind == "NumberSequence" then
        local Points = {}
        for _, v: any in Value.Keypoints do table.insert(Points, {v.Time, Encode(v.Value), Kind == "NumberSequence" and v.Envelope or 0}) end
        return {Type = Kind, Value = Points}
    end
    return nil
end

local function Decode(Value)
    if typeof(Value) ~= "table" then return Value end
    local Kind, Values = Value.Type, Value.Value
    if Kind == "Color3" then return Color3.new(table.unpack(Values)) end
    if Kind == "Vector2" then return Vector2.new(table.unpack(Values)) end
    if Kind == "Vector3" then return Vector3.new(table.unpack(Values)) end
    if Kind == "UDim" then return UDim.new(table.unpack(Values)) end
    if Kind == "UDim2" then return UDim2.new(table.unpack(Values)) end
    if Kind == "Rect" then return Rect.new(table.unpack(Values)) end
    if Kind == "EnumItem" then return Enum[Value.Enum][Values] end
    if Kind == "Font" then return Font.new(Values[1], Enum.FontWeight[Values[2]], Enum.FontStyle[Values[3]]) end
    if Kind == "ColorSequence" or Kind == "NumberSequence" then
        local Points = {}
        for _, v: any in Values do
            table.insert(Points, Kind == "ColorSequence" and ColorSequenceKeypoint.new(v[1], Decode(v[2])) or NumberSequenceKeypoint.new(v[1], v[2], v[3] or 0))
        end
        return Kind == "ColorSequence" and ColorSequence.new(Points) or NumberSequence.new(Points)
    end
    return nil
end

function ReplayVisuals.Capture(Object: Instance?)
    if not Object then return nil end
    local Supported: boolean = Object:IsA("GuiObject") or Properties[Object.ClassName] ~= nil
    if not Supported then return nil end
    local Data = {Class = Object.ClassName, Props = {}, Children = {}}
    for Class: string, List: {string} in Properties do
        local Matches: boolean = Object:IsA(Class) or Class == "TextLabel" and (Object:IsA("TextButton") or Object:IsA("TextBox")) or Class == "ImageLabel" and Object:IsA("ImageButton")
        if Matches then
            for _, Name: string in List do
                local Success, Value = pcall(function() return Object[Name] end)
                if Success then Data.Props[Name] = Encode(Value) end
            end
        end
    end
    for _, v: Instance in Object:GetChildren() do
        local Child = ReplayVisuals.Capture(v)
        if Child then table.insert(Data.Children, Child) end
    end
    return Data
end

function ReplayVisuals.Restore(Data, Parent: Instance?)
    if typeof(Data) ~= "table" or typeof(Data.Class) ~= "string" then return nil end
    local Success, Object = pcall(Instance.new, Data.Class)
    if not Success then return nil end
    if not (Object:IsA("GuiObject") or Properties[Object.ClassName]) then
        Object:Destroy()
        return nil
    end
    for Name: string, Value: any in Data.Props or {} do
        pcall(function() Object[Name] = Decode(Value) end)
    end
    for _, Child: any in Data.Children or {} do ReplayVisuals.Restore(Child, Object) end
    Object.Parent = Parent
    return Object
end

function ReplayVisuals.CaptureAppearance(Part: BasePart)
    local Children = {}
    for _, v: Instance in Part:GetChildren() do
        if v:IsA("Decal") or v:IsA("SpecialMesh") or v:IsA("SurfaceAppearance") then
            local Data = ReplayVisuals.Capture(v)
            if Data then table.insert(Children, Data) end
        end
    end
    return Children
end

function ReplayVisuals.ApplyAppearance(Part: BasePart, Appearance)
    if typeof(Appearance) ~= "table" then return end
    for _, v: Instance in Part:GetChildren() do
        if v:IsA("Decal") or v:IsA("SpecialMesh") or v:IsA("SurfaceAppearance") then v:Destroy() end
    end
    for _, Data: any in Appearance do ReplayVisuals.Restore(Data, Part) end
end

return ReplayVisuals