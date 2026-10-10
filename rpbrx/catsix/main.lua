local License = ... or {}
if shared.vapedeveloper ~= nil then
    shared.VapeDeveloper = shared.vapedeveloper
end
local Directory: string = shared.VapeDeveloper and "catprivate" or "catsix"
if not game:IsLoaded() then
    game.Loaded:Wait()
end
if shared.vapeloading and coroutine.status(shared.vapeloading) ~= "dead" then
    return
end
shared.vapeloading = coroutine.running()
if shared.vape then
    shared.vape:Uninject()
end
License.Key = License.Key or "_key"
local Executor: string = identifyexecutor ~= nil and ({identifyexecutor()})[1] or ""
local IsMadium: boolean = Executor == "Madium"
local CanHook: boolean = not table.find({"Madium", "Solara", "Xeno"}, Executor)

for _, Folder: string in {Directory, `{Directory}/games`, `{Directory}/profiles`, `{Directory}/assets`, `{Directory}/libraries`, `{Directory}/guis`} do
    if not isfolder(Folder) then
        makefolder(Folder)
    end
end

if not shared.VapeDeveloper and isfolder("catrewrite") and isfolder("catrewrite/profiles") then
    for _, v: string in listfiles("catrewrite/profiles") do
        if not v:find("commit.txt") then
            local OldPath: string = v
            v = v:gsub("catrewrite", Directory)
            writefile(v, readfile(OldPath))
        end
    end
    delfolder("catrewrite/profiles")
end

getgenv().matchDodgeActive = isfile(`{Directory}/profiles/matchdodge.txt`) and readfile(`{Directory}/profiles/matchdodge.txt`) == "true"
getgenv().regionLockActive = isfile(`{Directory}/profiles/regionlock.txt`) and readfile(`{Directory}/profiles/regionlock.txt`) == "true"
getgenv().regionLockAllowed = not getgenv().regionLockActive
if (getgenv().matchDodgeActive or getgenv().regionLockActive) and not getgenv().runv2 and hookmetamethod and CanHook then
    getgenv().runv2 = true
    getgenv().matchDodgeHold = {Held = true, Started = os.clock()}
    getgenv().matchDodgeHold.Release = function()
        if not getgenv().matchDodgeHold.Held then
            return
        end
        getgenv().matchDodgeHold.Held = false
        getgenv().dodged = false
        if getgenv().matchDodgeHold.Remote then
            getgenv().matchDodgeHold.Remote:FireServer()
            getgenv().matchDodgeHold.Remote = nil
        end
    end
    local Old
    Old = hookmetamethod(game, "__namecall", newcclosure(function(self, ...)
        if getgenv().matchDodgeHold.Held and not checkcaller() and getnamecallmethod() == "FireServer" and tostring(self) == "PlayerReady" then
            getgenv().matchDodgeHold.Remote = self
            getgenv().dodged = true
            return
        end
        return Old(self, ...)
    end))
    task.delay(45, function()
        if not getgenv().matchDodgeHold.Claimed then
            getgenv().matchDodgeHold.Release()
        end
    end)
end

local vape, RunPremium
local loadstring = function(...)
    local Chunk, Error = loadstring(...)
    if Error and vape then
        vape:CreateNotification("Vape", `Failed to load : {Error}`, 30, "alert")
    end
    return Chunk
end
local queue_on_teleport = queue_on_teleport or function() end
local isfile = isfile or function(FilePath: string)
    local Success, Contents = pcall(function()
        return readfile(FilePath)
    end)
    return Success and Contents ~= nil and Contents ~= ""
end
local cloneref = cloneref or function(Reference: Instance)
    return Reference
end
local Players: Players = cloneref(game:GetService("Players"))
local HttpService: HttpService = cloneref(game:GetService("HttpService"))

if IsMadium and game.GameId == 2619619496 and not Players.LocalPlayer:GetAttribute("PlayerConnected") then
    local LogService: LogService = cloneref(game:GetService("LogService"))
    local SetupLine: string = "[SETUP] Set up Flamework Client"
    local SetupDone: boolean = false
    local SetupConnection = LogService.MessageOut:Connect(function(Message: string)
        if string.find(Message, SetupLine, 1, true) then
            SetupDone = true
        end
    end)

    for _, Entry: any in LogService:GetLogHistory() do
        if string.find(Entry.message, SetupLine, 1, true) then
            SetupDone = true
            break
        end
    end

    local WaitStarted: number = os.clock()
    repeat
        task.wait(0.25)
    until SetupDone or os.clock() - WaitStarted > 20
    SetupConnection:Disconnect()
end

local function DownloadFile(FilePath: string, Reader)
    if not isfile(FilePath) then
        local Success, Response = pcall(function()
            return game:HttpGet(`https://api.catvape.dev/download/src/{select(1, FilePath:gsub(`^{Directory}/`, ""))}`, true)
        end)
        if not Success or Response == "404: Not Found" then
            error(Response)
        end
        if FilePath:find(".lua") then
            Response = `--This watermark is used to delete the file if its cached, remove it to make the file persist after vape updates.\n{Response}`
        end
        writefile(FilePath, Response)
    end
    return (Reader or readfile)(FilePath)
end

local function FinishLoading()
    vape.Init = nil
    vape:Load()
    shared.vapeloading = nil

    local TeleportedServers: boolean?
    vape:Clean(Players.LocalPlayer.OnTeleport:Connect(function()
        if (not TeleportedServers) and (not shared.VapeIndependent) then
            TeleportedServers = true
            local TeleportScript: string = [[
				shared.vapereload = true
				if shared.VapeDeveloper then
					loadstring(readfile('catprivate/main.lua'), 'main')(_scriptconfig)
				else
					loadstring(game:HttpGet('https://api.catvape.dev/script?key=_key', true), 'init')(_scriptconfig)
				end
			]]
            local TeleportConfig: string = HttpService:JSONEncode(License)
            TeleportConfig = TeleportConfig:gsub('":true', "=true"):gsub('{"', "{")
            TeleportConfig = TeleportConfig:gsub(',"', ","):gsub('":', "=")
            TeleportConfig = TeleportConfig:gsub("%[", "{"):gsub("%]", "}")
            TeleportScript = TeleportScript:gsub("_key", tostring(License.Key or "_key"))
            TeleportScript = TeleportScript:gsub("_scriptconfig", TeleportConfig)
            if shared.VapeDeveloper then
                TeleportScript = `shared.VapeDeveloper = true\n{TeleportScript}`
            end
            if shared.VapeCustomProfile then
                TeleportScript = `shared.VapeCustomProfile = "{shared.VapeCustomProfile}"\n{TeleportScript}`
            end
            vape:Save()
            queue_on_teleport(TeleportScript)
        end
    end))

    if not shared.vapereload then
        vape:CreateNotification("Finished Loading", (getgenv().catuser and `Authenticated as {getgenv().catuser} with {getgenv().catrole}, ` or "") .. (vape.VapeButton and "Press the button in the top right" or `Press {table.concat(vape.GUIBind and vape.GUIBind.Keys or {"RightShift"}, " + "):upper()}`) .. " to open GUI", 5)
        task.delay(0.05, function()
            if shared.updated then
                vape:CreateNotification("Cat", `Script has updated from {(shared.updated or ""):sub(1, 8)} to {(readfile(`{Directory}/profiles/commit.txt`) or ""):sub(1, 8)}`, 10, "info")
            end
        end)
    end
end

if not isfile(`{Directory}/profiles/gui.txt`) then
    writefile(`{Directory}/profiles/gui.txt`, "new")
end
local Gui: string = "new"

if not isfolder(`{Directory}/assets/{Gui}`) then
    makefolder(`{Directory}/assets/{Gui}`)
end
vape, RunPremium = loadstring(DownloadFile(`{Directory}/guis/{Gui}.lua`), "gui")(License)
shared.vape = vape
shared.vapesmooth = false
_G.vape = vape
getgenv().used_init = true

if hookmetamethod and not getgenv().run and CanHook then
    getgenv().run = true
    local Old
    Old = hookmetamethod(game, "__namecall", newcclosure(function(self, Remote, ...)
        if not checkcaller() and getnamecallmethod() == "FireServer" then
            if typeof(Remote) == "Instance" and Remote.Name == "TabFreezeAnticheat_ClientToServerReport" and shared.vapeloading then
                return
            end
        end
        return Old(self, Remote, ...)
    end))
end

if shared.maincat then
    redirect()
    Players.LocalPlayer:Kick("Your script is outdated, get new one at discord.gg/catvape")
    return
end

if not shared.VapeIndependent then
    loadstring(DownloadFile(`{Directory}/games/universal.lua`), "universal")(License)
    if isfile(`{Directory}/games/{game.PlaceId}.lua`) then
        loadstring(readfile(`{Directory}/games/{game.PlaceId}.lua`), tostring(game.PlaceId))(License)
    end
    loadstring(DownloadFile(`{Directory}/libraries/main.lua`), "main")(License)
    FinishLoading()
else
    vape.Init = FinishLoading
    return vape
end