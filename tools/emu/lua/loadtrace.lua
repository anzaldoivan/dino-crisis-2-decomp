-- tools/emu/lua/loadtrace.lua: loader-route trace (T5). Load after lua/vsync.lua (uses DC2_VSYNC); needs -debugger.
-- Non-pausing Exec breakpoints on the loader entry points (docs/memory-map.md#loader-routes); each hit appends
--   <vsync> <tag> <pc> <reg>=<value> ...
-- to <repo>/.run/emu/loadtrace.log (or $DC2_LOADTRACE). Invokers return nothing, so the breakpoint stays armed
-- and the emulator never pauses (the default invoker is the one that pauses).
local src = debug.getinfo(1, 'S').source:gsub('^@', '')
local root = src:gsub('tools/emu/lua/loadtrace%.lua$', '')
local path = os.getenv('DC2_LOADTRACE') or (root .. '.run/emu/loadtrace.log')
DC2_LOADTRACE_FILE = io.open(path, 'a')

local function log(tag, pc, regs)
    local r = PCSX.getRegisters().GPR.n
    local parts = { tostring(DC2_VSYNC or -1), tag, string.format('0x%08x', pc) }
    for _, name in ipairs(regs) do parts[#parts + 1] = string.format('%s=0x%08x', name, r[name]) end
    DC2_LOADTRACE_FILE:write(table.concat(parts, ' '), '\n')
    DC2_LOADTRACE_FILE:flush()
end

-- pc, tag, logged argument registers
local points = {
    { 0x8001ced4, 'exec', { 'a0' } },         -- overlay exec: a0 = entry
    { 0x8001ecbc, 'raw', { 'a0', 'a1' } },    -- raw read: a0 = index, a1 = dest
    { 0x8001ec10, 'arc', { 'a0' } },          -- archive request: a0 = index
    { 0x8004262c, 'r2', { 'a0' } },           -- R2 module load: a0 = k
}
DC2_LOADTRACE_BPS = {}
for _, p in ipairs(points) do
    local pc, tag, regs = p[1], p[2], p[3]
    DC2_LOADTRACE_BPS[#DC2_LOADTRACE_BPS + 1] = PCSX.addBreakpoint(pc, 'Exec', 4, 'dc2-' .. tag, function()
        log(tag, pc, regs)
    end)
end
