-- tools/emu/lua/vsync.lua: loaded by tools/emu/emu.sh via -dofile.
-- Counts GPU vsyncs since boot; GET /api/v1/lua/vsync returns the count as text.
-- PCSX.WebServer does not exist yet at -dofile time, so the handler is registered lazily
-- (first vsync, or the DC2_BREAK hit, whichever comes first).
DC2_VSYNC = 0
local function dc2_route()
    if not DC2_VSYNC_ROUTE and PCSX.WebServer and PCSX.WebServer.Handlers then
        PCSX.WebServer.Handlers.vsync = function(request) return tostring(DC2_VSYNC) end
        DC2_VSYNC_ROUTE = true
    end
end
DC2_VSYNC_LISTENER = PCSX.Events.createEventListener('GPU::Vsync', function()
    DC2_VSYNC = DC2_VSYNC + 1
    dc2_route()
end)
-- DC2_BREAK=<addr> (set by emu.sh callers, needs -debugger): Exec breakpoint that pauses the emulator.
if os.getenv('DC2_BREAK') then
    DC2_BREAK_BP = PCSX.addBreakpoint(tonumber(os.getenv('DC2_BREAK')), 'Exec', 4, 'dc2-break', function()
        dc2_route()
        PCSX.pauseEmulator()
    end)
end
