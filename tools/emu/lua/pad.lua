-- tools/emu/lua/pad.lua: scripted pad-1 input (T5). Run via emu.sh DC2_LUA after lua/vsync.lua (uses DC2_VSYNC).
-- DC2_PAD="<vsync>:<BUTTON>[:<hold>] ..." space-separated, ascending vsyncs; BUTTON is a PCSX.CONSTS.PAD.BUTTON name
-- (START, CROSS, CIRCLE, UP, DOWN, ...), pressed at <vsync> and held <hold> vsyncs (default 6).
-- API: PCSX.SIO0.slots[1].pads[1].setOverride(btn) / .clearOverride(btn). Each press/release appends
-- "<vsync> <BUTTON> down|up" to <repo>/.run/emu/pad.log (or $DC2_PADLOG).
local root = debug.getinfo(1, 'S').source:gsub('^@', ''):gsub('tools/emu/lua/pad%.lua$', '')
local plog = io.open(os.getenv('DC2_PADLOG') or (root .. '.run/emu/pad.log'), 'a')
local function note(s) plog:write(tostring(DC2_VSYNC), ' ', s, '\n'); plog:flush() end
local pad = PCSX.SIO0.slots[1].pads[1]
local B = PCSX.CONSTS.PAD.BUTTON
local seq = {}
for v, name, hold in (os.getenv('DC2_PAD') or ''):gmatch('(%d+):(%u+):?(%d*)') do
    assert(B[name], 'pad.lua: unknown button ' .. name)
    seq[#seq + 1] = { tonumber(v), name, tonumber(v) + (tonumber(hold) or 6) }
end
local i, held = 1, nil
DC2_PAD_LISTENER = PCSX.Events.createEventListener('GPU::Vsync', function()
    local v = DC2_VSYNC or 0
    if held and v >= held[3] then
        pad.clearOverride(B[held[2]]); note(held[2] .. ' up'); held = nil
    end
    if not held and seq[i] and v >= seq[i][1] then
        held = seq[i]; i = i + 1
        pad.setOverride(B[held[2]]); note(held[2] .. ' down')
    end
end)
