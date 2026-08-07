## Cache

### `cache.invalidate(obj)`
Aliases: none
Invalidates a cached Instance reference so a fresh reference must be re-fetched.
```lua
local part = Instance.new("Part")
part.Parent = workspace
cache.invalidate(part)
local stillvalid = part.Parent == workspace
assert(stillvalid, "part should still be usable after invalidate")
print(stillvalid) -- output: true
```

### `cache.iscached(obj)`
Aliases: none
Returns whether an Instance is currently present in the engine's reference cache.
```lua
local part = Instance.new("Part")
local cached = cache.iscached(part)
assert(type(cached) == "boolean", "cache.iscached should return a boolean")
print(cached) -- output: true
```

### `cache.replace(obj, newObj)`
Aliases: none
Replaces all internal references to `obj` with `newObj`.
```lua
local part = Instance.new("Part")
local replacement = Instance.new("Part")
cache.replace(part, replacement)
local ok = pcall(function()
    return part.Parent
end)
assert(ok, "part reference should still be callable after replace")
print(ok) -- output: true
```

## Closures

### `islclosure(fn)`
Aliases: none
Returns `true` if `fn` is a Lua closure.
```lua
local function samplefunction() end
local result = islclosure(samplefunction)
assert(result == true, "samplefunction should be reported as a lua closure")
print(result) -- output: true
```

### `iscclosure(fn)`
Aliases: none
Returns `true` if `fn` is a C closure.
```lua
local result = iscclosure(print)
assert(result == true, "print should be reported as a c closure")
print(result) -- output: true
```

### `isexecutorclosure(fn)`
Aliases: `checkclosure`, `isourclosure`
Returns `true` if `fn` originates from the executor rather than the game or Lua VM.
```lua
local function samplefunction() end
local ours = isexecutorclosure(samplefunction)
local roblox = isexecutorclosure(print)
assert(ours == true, "samplefunction should be flagged as an executor closure")
assert(roblox == false, "print should not be flagged as an executor closure")
print(ours, roblox) -- output: true false
```

### `newcclosure(fn, name?)`
Aliases: none
Wraps a Lua function so it behaves like (and is detected as) a C closure, while still yielding correctly.
```lua
local wrapped = newcclosure(function()
    return "hello"
end)
assert(iscclosure(wrapped) == true, "wrapped function should report as a c closure")
local result = wrapped()
assert(result == "hello", "wrapped function should still return its original value")
print(result) -- output: hello
```

### `clonefunction(fn)`
Aliases: none
Returns a fresh copy of a function with the same behavior/upvalues/environment but a different identity.
```lua
local function samplefunction()
    return 1
end
local cloned = clonefunction(samplefunction)
assert(cloned() == samplefunction(), "cloned function should behave identically")
assert(cloned ~= samplefunction, "cloned function should be a distinct identity")
print(cloned()) -- output: 1
```

### `hookfunction(target, hook)`
Aliases: `hookfunc`, `replaceclosure`
Replaces `target`'s implementation with `hook` and returns the original function so it can still be called.
```lua
local function target()
    return "original"
end
local original = hookfunction(target, function()
    return "hooked"
end)
assert(target() == "hooked", "target should now run the hook")
assert(original() == "original", "original should still be callable")
print(target()) -- output: hooked
print(original()) -- output: original
```

### `restorefunction(fn)`
Aliases: `restorefunc`
Restores a previously-hooked function back to its original behavior.
```lua
restorefunction(target)
local result = target()
assert(result == "original", "target should be back to its original behavior")
print(result) -- output: original
```

### `getfunctionhash(fn)`
Aliases: none
Returns a hex-encoded hash uniquely identifying a function's bytecode/behavior.
```lua
local function samplefunction() end
local hash = getfunctionhash(samplefunction)
assert(type(hash) == "string" and #hash > 0, "hash should be a non empty string")
print(hash) -- output: a hex string such as 3f2a9c1b...
```

### `checkcaller()`
Aliases: none
Returns `true` if the calling code originates from the executor thread rather than the game.
```lua
local result = checkcaller()
assert(result == true, "running from the executor console should return true")
print(result) -- output: true
```

### `isfunctionhooked(fn)`
Aliases: none
Returns `true` if a function has been hooked via `hookfunction`.
```lua
local function target()
    return 1
end
hookfunction(target, function()
    return 2
end)
local result = isfunctionhooked(target)
assert(result == true, "target should be reported as hooked")
print(result) -- output: true
```

## crypt

### `crypt.encrypt(data, key, iv?, mode?)`
Aliases: none
Encrypts a string (default AES-CBC) with a base64 key/IV.
```lua
local key = crypt.generatekey()
local encrypted = crypt.encrypt("serpent", key)
assert(type(encrypted) == "string" and encrypted ~= "serpent", "encrypted output should differ from the plaintext")
print(encrypted) -- output: a base64 string that changes every run
```

### `crypt.decrypt(data, key, iv?, mode?)`
Aliases: none
Decrypts data produced by `crypt.encrypt`.
```lua
local decrypted = crypt.decrypt(encrypted, key)
assert(decrypted == "serpent", "decrypted output should match the original plaintext")
print(decrypted) -- output: serpent
```

### `crypt.generatekey()`
Aliases: none
Generates a random base64-encoded 32-byte key.
```lua
local key = crypt.generatekey()
assert(type(key) == "string" and #key > 0, "key should be a non empty string")
print(key) -- output: a base64 string, different every call
```

### `crypt.generatebytes(n)`
Aliases: none
Generates `n` random bytes, base64-encoded.
```lua
local bytes = crypt.generatebytes(16)
assert(type(bytes) == "string" and #bytes > 0, "bytes should be a non empty string")
print(bytes) -- output: a base64 string encoding 16 random bytes
```

### `crypt.base64encode(str)`
Aliases: `base64encode`, `base64_encode`, `crypt.base64.encode`, `base64.encode`
Base64-encodes a string.
```lua
local encoded = crypt.base64encode("serpent")
assert(encoded == "c2VycGVudA==", "encoded output should match the known base64 of serpent")
print(encoded) -- output: c2VycGVudA==
```

### `crypt.base64decode(str)`
Aliases: `base64decode`, `base64_decode`, `crypt.base64.decode`, `base64.decode`
Base64-decodes a string.
```lua
local decoded = crypt.base64decode("c2VycGVudA==")
assert(decoded == "serpent", "decoded output should match the original string")
print(decoded) -- output: serpent
```

### `crypt.hash(data, algo)`
Aliases: none
Hashes data with the given algorithm (md5, sha1, sha224/256/384/512, sha3-256/384/512, etc).
```lua
local hashed = crypt.hash("serpent", "sha256")
assert(type(hashed) == "string" and #hashed == 64, "sha256 hash should be a 64 character hex string")
print(hashed) -- output: a fixed 64 character hex string for the same input
```

### `crypt.random(n)`
Aliases: none
Returns `n` random bytes as a string (0-1024).
```lua
local random = crypt.random(32)
assert(#random == 32, "random should return exactly the requested byte length")
print(#random) -- output: 32
```

### `crypt.hmac(key, data, algo)`
Aliases: none
Computes an HMAC digest of `data` using `key` and the given hash algorithm.
```lua
local key = crypt.generatekey()
local mac = crypt.hmac(key, "serpent", "sha256")
assert(type(mac) == "string" and #mac > 0, "hmac should be a non empty string")
print(mac) -- output: a hex or base64 digest, changes with the key
```

## debug

### `debug.getconstant(fn, index)`
Returns the constant stored at `index` in a function's constant table.
```lua
local function sample()
    return "hello"
end
local constant = debug.getconstant(sample, 1)
assert(constant == "hello", "first constant should be the literal used in the function")
print(constant) -- output: hello
```

### `debug.getconstants(fn)`
Returns all constants used by a function, in order.
```lua
local constants = debug.getconstants(sample)
assert(type(constants) == "table" and constants[1] == "hello", "constants table should contain the literal")
print(constants[1]) -- output: hello
```

### `debug.getproto(fn, index, active?)`
Returns a nested function prototype; with `active=true` returns a callable closure.
```lua
local function outer()
    local function inner()
        return "nested"
    end
    return inner
end
local proto = debug.getproto(outer, 1, true)
assert(type(proto) == "function", "active proto should be a callable function")
print(proto()) -- output: nested
```

### `debug.getprotos(fn)`
Returns all nested function prototypes of `fn`.
```lua
local protos = debug.getprotos(outer)
assert(type(protos) == "table" and #protos > 0, "outer should have at least one nested prototype")
print(#protos) -- output: 1
```

### `debug.getupvalue(fn, index)`
Returns the upvalue at `index` for a function.
```lua
local counter = 0
local function increment()
    counter = counter + 1
end
local upvalue = debug.getupvalue(increment, 1)
assert(upvalue == 0, "upvalue should match the current value of counter")
print(upvalue) -- output: 0
```

### `debug.getupvalues(fn)`
Returns a table of all upvalues for a function.
```lua
local upvalues = debug.getupvalues(increment)
assert(type(upvalues) == "table" and upvalues[1] == 0, "upvalues table should contain counter")
print(upvalues[1]) -- output: 0
```

### `debug.getstack(thread, index?)`
Returns values from the Lua call stack.
```lua
local function sample()
    local x = 42
    return debug.getstack(coroutine.running(), 1)
end
local value = sample()
assert(value ~= nil, "getstack should return a value from the active frame")
print(value) -- output: 42
```

### `debug.setstack(thread, index, value)`
Overwrites a value on the Lua call stack.
```lua
local function sample()
    local x = 1
    debug.setstack(coroutine.running(), 1, 99)
    return x
end
local result = sample()
assert(result == 99, "x should reflect the overwritten stack value")
print(result) -- output: 99
```

### `debug.setconstant(fn, index, value)`
Overwrites a constant in a function's constant table.
```lua
local function sample()
    return "hello"
end
debug.setconstant(sample, 1, "changed")
local result = sample()
assert(result == "changed", "sample should now return the new constant")
print(result) -- output: changed
```

### `debug.setupvalue(fn, index, value)`
Overwrites an upvalue of a function.
```lua
local counter = 0
local function increment()
    counter = counter + 1
    return counter
end
debug.setupvalue(increment, 1, 100)
local result = increment()
assert(result == 101, "increment should continue from the overwritten upvalue")
print(result) -- output: 101
```

## Drawing

### `Drawing.new(type)`
Creates a Drawing object: `Line`, `Text`, `Image`, `Circle`, `Square`, `Quad`, or `Triangle`, each with type-appropriate properties (`Color`, `Transparency`, `Visible`, `ZIndex`, etc).
```lua
local line = Drawing.new("Line")
line.Visible = true
line.Color = Color3.new(1, 0, 0)
assert(isrenderobj(line) == true, "line should be a recognized drawing object")
print(isrenderobj(line)) -- output: true
```

### `Drawing.Fonts`
Table of built-in fonts: `UI=0`, `System=1`, `Plex=2`, `Monospace=3`.
```lua
local system = Drawing.Fonts.System
assert(system == 1, "Drawing.Fonts.System should be enum value 1")
print(system) -- output: 1
```

### `getrenderproperty(drawingObj, prop)`
Reads a property off a Drawing object.
```lua
local line = Drawing.new("Line")
line.Visible = true
local visible = getrenderproperty(line, "Visible")
assert(visible == true, "getrenderproperty should reflect the value just set")
print(visible) -- output: true
```

### `setrenderproperty(drawingObj, prop, value)`
Sets a property on a Drawing object.
```lua
setrenderproperty(line, "Visible", false)
local visible = getrenderproperty(line, "Visible")
assert(visible == false, "property should update after setrenderproperty")
print(visible) -- output: false
```

### `cleardrawcache()`
Destroys all active Drawing objects.
```lua
cleardrawcache()
local ok = pcall(function()
    return line.Visible
end)
assert(ok == false, "line should be destroyed after cleardrawcache")
print(ok) -- output: false
```

### `isrenderobj(obj)`
Returns `true` if `obj` is a Drawing object.
```lua
local line = Drawing.new("Line")
local part = Instance.new("Part")
assert(isrenderobj(line) == true, "line should be a drawing object")
assert(isrenderobj(part) == false, "part should not be a drawing object")
print(isrenderobj(line), isrenderobj(part)) -- output: true false
```

## Environment

### `getgenv()`
Returns the executor's persistent global environment table.
```lua
getgenv().serpenttest = true
local value = getgenv().serpenttest
assert(value == true, "value stored in getgenv should persist on the same table")
print(value) -- output: true
```

### `getrenv()`
Returns the Roblox (game) global environment table.
```lua
local renv = getrenv()
assert(type(renv) == "table", "getrenv should return a table")
print(type(renv)) -- output: table
```

### `getgc(includeTables?)`
Returns a table of items currently tracked by the garbage collector.
```lua
local gc = getgc(true)
assert(type(gc) == "table" and #gc > 0, "gc list should contain tracked objects")
print(#gc > 0) -- output: true
```

### `filtergc(type, filter, returnFirst?)`
Searches the GC list for a `function` or `table` matching constants/upvalues/key-values/metatable criteria.
```lua
local found = filtergc("function", { name = "print" }, true)
assert(found ~= nil, "filtergc should locate the print function")
print(type(found)) -- output: function
```

### `getreg()`
Returns the Lua registry table.
```lua
local registry = getreg()
assert(type(registry) == "table", "getreg should return a table")
print(type(registry)) -- output: table
```

## Filesystem

### `writefile(path, content)`
Writes a string to a file, creating nested folders as needed (restricted to the executor's workspace).
```lua
writefile("serpent/test.txt", "hello")
local exists = isfile("serpent/test.txt")
assert(exists == true, "file should exist after writefile")
print(exists) -- output: true
```

### `readfile(path)`
Reads a file's contents as a string.
```lua
local content = readfile("serpent/test.txt")
assert(content == "hello", "readfile should return exactly what was written")
print(content) -- output: hello
```

### `appendfile(path, content)`
Appends content to the end of an existing (or new) file.
```lua
appendfile("serpent/test.txt", " world")
local content = readfile("serpent/test.txt")
assert(content == "hello world", "appendfile should extend the existing content")
print(content) -- output: hello world
```

### `isfile(path)`
Returns `true` if a path exists and is a file.
```lua
local result = isfile("serpent/test.txt")
assert(result == true, "test.txt should be reported as a file")
print(result) -- output: true
```

### `delfile(path)`
Deletes a file.
```lua
delfile("serpent/test.txt")
local result = isfile("serpent/test.txt")
assert(result == false, "test.txt should no longer exist after delfile")
print(result) -- output: false
```

### `listfiles(path)`
Lists files/folders at a given path.
```lua
writefile("serpent/listtest.txt", "x")
local list = listfiles("serpent")
local found = false
for i, entry in list do
    if entry:find("listtest.txt") then
        found = true
    end
end
assert(found == true, "listfiles should include the file just created")
print(found) -- output: true
```

### `loadfile(path)`
Compiles a file into a callable function, similar to `loadstring`.
```lua
writefile("serpent/run.lua", "return 1 + 1")
local fn = loadfile("serpent/run.lua")
assert(type(fn) == "function", "loadfile should return a callable function")
local result = fn()
assert(result == 2, "compiled file should evaluate to 2")
print(result) -- output: 2
```

### `isfolder(path)`
Returns `true` if a path exists and is a folder.
```lua
makefolder("serpent")
local result = isfolder("serpent")
assert(result == true, "serpent should be reported as a folder")
print(result) -- output: true
```

### `makefolder(path)`
Creates a folder (and parents as needed).
```lua
makefolder("serpent/nested/folder")
local result = isfolder("serpent/nested/folder")
assert(result == true, "nested folder path should exist after makefolder")
print(result) -- output: true
```

### `delfolder(path)`
Deletes a folder and its contents.
```lua
delfolder("serpent/nested")
local result = isfolder("serpent/nested")
assert(result == false, "nested folder should no longer exist after delfolder")
print(result) -- output: false
```

### `getcustomasset(path)`
Returns an `rbxasset://` URI that lets a local file be used as a game asset (images, sounds, meshes, fonts, video).
```lua
writefile("serpent/image.txt", "placeholder")
local uri = getcustomasset("serpent/image.txt")
assert(type(uri) == "string" and uri:sub(1, 11) == "rbxasset://", "uri should be an rbxasset scheme")
print(uri) -- output: rbxasset://serpent/image.txt
```

## FileDialog

### `openfiledialog(options?)`
Opens a native "pick a file" dialog; returns a file object (`GetName`, `IsFile`, `IsFolder`, `Read`, `Write`, `Append`, `Delete`). Supports `title`, `defaultPath`, `extensionFilter`.
```lua
local file = openfiledialog({ title = "pick a script", extensionFilter = { "lua" } })
assert(file == nil or type(file) == "table", "result should be nil (cancelled) or a file object")
if file then
    print(file:GetName()) -- output: the file name the user picked
end
```

### `openfilesdialog(options?)`
Same as above but allows multi-select; returns an array of file objects.
```lua
local files = openfilesdialog({ title = "pick scripts" })
assert(files == nil or type(files) == "table", "result should be nil (cancelled) or a table of file objects")
if files then
    print(#files) -- output: the number of files picked
end
```

### `openfolderdialog(options?)`
Opens a "pick a folder" dialog; returns a folder object (`GetName`, `IsFile`, `IsFolder`, `List`, `MakeFolder`, `Delete`).
```lua
local folder = openfolderdialog({ title = "pick a folder" })
assert(folder == nil or type(folder) == "table", "result should be nil (cancelled) or a folder object")
if folder then
    print(folder:GetName()) -- output: the folder name the user picked
end
```

### `savefiledialog(content, options?)`
Opens a native "save as" dialog, writes `content` to the chosen path, and returns a file object. Supports `suggestedName`, `defaultExtension`, `extensionFilter`.
```lua
local saved = savefiledialog("hello", { suggestedName = "output", defaultExtension = "txt" })
assert(saved == nil or type(saved) == "table", "result should be nil (cancelled) or a file object")
if saved then
    print(saved:Read()) -- output: hello
end
```

## Input

### `isrbxactive()`
Aliases: `iswindowactive`, `isgameactive`
Returns whether the Roblox window currently has focus.
```lua
local result = isrbxactive()
assert(type(result) == "boolean", "isrbxactive should return a boolean")
print(result) -- output: true if the roblox window is focused
```

### `keypress(keycode)`
Aliases: none
Simulates a key-down input event.
```lua
local ok = pcall(function()
    keypress(0x57)
end)
assert(ok == true, "keypress should not error for a valid keycode")
print(ok) -- output: true
```

### `keyrelease(keycode)`
Aliases: none
Simulates a key-up input event.
```lua
local ok = pcall(function()
    keyrelease(0x57)
end)
assert(ok == true, "keyrelease should not error for a valid keycode")
print(ok) -- output: true
```

### `keyclick(keycode)`
Aliases: `keytap`
Simulates a full key press + release.
```lua
local ok = pcall(function()
    keyclick(0x57)
end)
assert(ok == true, "keyclick should not error for a valid keycode")
print(ok) -- output: true
```

### `mouse1click()` / `mouse1press()` / `mouse1release()`
Aliases: none
Simulate a left mouse click / press / release.
```lua
local ok = pcall(function()
    mouse1press()
    mouse1release()
    mouse1click()
end)
assert(ok == true, "left mouse simulation calls should not error")
print(ok) -- output: true
```

### `mouse2click()` / `mouse2press()` / `mouse2release()`
Aliases: none
Simulate a right mouse click / press / release.
```lua
local ok = pcall(function()
    mouse2press()
    mouse2release()
    mouse2click()
end)
assert(ok == true, "right mouse simulation calls should not error")
print(ok) -- output: true
```

### `mousemoveabs(x, y)`
Aliases: none
Moves the mouse to an absolute screen position.
```lua
local ok = pcall(function()
    mousemoveabs(500, 300)
end)
assert(ok == true, "mousemoveabs should not error for valid coordinates")
print(ok) -- output: true
```

### `mousemoverel(dx, dy)`
Aliases: none
Moves the mouse relative to its current position.
```lua
local ok = pcall(function()
    mousemoverel(10, 0)
end)
assert(ok == true, "mousemoverel should not error for a valid delta")
print(ok) -- output: true
```

### `mousescroll(delta)`
Aliases: none
Simulates a scroll-wheel event.
```lua
local ok = pcall(function()
    mousescroll(1)
end)
assert(ok == true, "mousescroll should not error for a valid delta")
print(ok) -- output: true
```

### `setclipboard(text)`
Aliases: `toclipboard`
Writes text to the system clipboard.
```lua
local ok = pcall(function()
    setclipboard("serpent")
end)
assert(ok == true, "setclipboard should not error for a string argument")
print(ok) -- output: true
```

### `setrbxclipboard(data)`
Aliases: none
Writes data to Roblox Studio's internal clipboard (for pasting Instances).
```lua
local part = Instance.new("Part")
local ok = pcall(function()
    setrbxclipboard(part)
end)
assert(ok == true, "setrbxclipboard should not error for an instance argument")
print(ok) -- output: true
```

## Instances

### `gethui()`
Aliases: `get_hidden_gui`
Returns a protected GUI container hidden from the game's own scripts, usually a clone of CoreGui.
```lua
local hidden = gethui()
assert(typeof(hidden) == "Instance", "gethui should return an instance")
print(typeof(hidden)) -- output: Instance
```

### `cloneref(instance)`
Aliases: none
Returns a duplicate reference to an Instance that is `~=` the original but points to the same underlying object (defeats `==` reference checks).
```lua
local part = Instance.new("Part")
local cloned = cloneref(part)
assert(cloned ~= part, "cloneref should return a distinct reference")
assert(compareinstances(part, cloned) == true, "cloned reference should point to the same underlying instance")
print(cloned ~= part) -- output: true
```

### `compareinstances(a, b)`
Aliases: none
Compares two Instance references (including cloned refs) for underlying equality.
```lua
local part = Instance.new("Part")
local cloned = cloneref(part)
local result = compareinstances(part, cloned)
assert(result == true, "compareinstances should treat clone and original as equal")
print(result) -- output: true
```

### `getinstances()`
Aliases: none
Returns every Instance loaded into the client, including those with a `nil` parent.
```lua
local instances = getinstances()
assert(type(instances) == "table" and #instances > 0, "getinstances should return a non empty table")
print(#instances > 0) -- output: true
```

### `getnilinstances()`
Aliases: none
Returns only Instances whose parent is `nil` (not currently in the DataModel tree).
```lua
local part = Instance.new("Part")
local nilinstances = getnilinstances()
local found = false
for i, instance in nilinstances do
    if instance == part then
        found = true
    end
end
assert(found == true, "unparented part should show up in getnilinstances")
print(found) -- output: true
```

### `getcallbackvalue(instance, property)`
Aliases: `getcallbackmember`
Returns the function assigned to a callback property (e.g. `BindableFunction.OnInvoke`, `RemoteFunction.OnServerInvoke`).
```lua
local bindable = Instance.new("BindableFunction")
bindable.OnInvoke = function()
    return "called"
end
local callback = getcallbackvalue(bindable, "OnInvoke")
assert(type(callback) == "function", "getcallbackvalue should return the assigned function")
print(callback()) -- output: called
```

### `fireclickdetector(detector, distance?, event?)`
Aliases: none
Fires a `ClickDetector`'s `MouseClick`/`RightMouseClick`/`MouseHoverEnter`/`MouseHoverLeave` event.
```lua
local detector = Instance.new("ClickDetector")
local fired = false
detector.MouseClick:Connect(function()
    fired = true
end)
fireclickdetector(detector, 10)
assert(fired == true, "MouseClick should fire from fireclickdetector")
print(fired) -- output: true
```

### `fireproximityprompt(prompt)`
Aliases: none
Fires a `ProximityPrompt` as if a player triggered it.
```lua
local prompt = Instance.new("ProximityPrompt")
local fired = false
prompt.Triggered:Connect(function()
    fired = true
end)
fireproximityprompt(prompt)
assert(fired == true, "Triggered should fire from fireproximityprompt")
print(fired) -- output: true
```

### `firetouchinterest(part1, part2, toggle)`
Aliases: none
Fires `Touched`/`TouchEnded` between two parts.
```lua
local part1 = Instance.new("Part")
local part2 = Instance.new("Part")
local touched = false
part1.Touched:Connect(function()
    touched = true
end)
firetouchinterest(part1, part2, 0)
firetouchinterest(part1, part2, 1)
assert(touched == true, "Touched should fire from firetouchinterest")
print(touched) -- output: true
```

### `saveinstance(root, options?)`
Aliases: none
Serializes an Instance tree to disk (place/rbxm-style dump).
```lua
saveinstance(workspace, { path = "serpent/dump.rbxm" })
local result = isfile("serpent/dump.rbxm")
assert(result == true, "saveinstance should write a file at the given path")
print(result) -- output: true
```

## Metatable

### `getrawmetatable(obj)`
Returns the real metatable of a table/userdata, bypassing a protected `__metatable` field.
```lua
local obj = setmetatable({}, { __metatable = "locked" })
local mt = getrawmetatable(obj)
assert(type(mt) == "table", "getrawmetatable should return the real metatable, not the locked value")
print(type(mt)) -- output: table
```

### `setrawmetatable(obj, mt)`
Forcibly sets the metatable of a table/userdata, bypassing `__metatable` protection.
```lua
local obj = setmetatable({}, { __metatable = "locked" })
local newmt = { __index = function() return "patched" end }
setrawmetatable(obj, newmt)
local result = obj.anything
assert(result == "patched", "obj should use the newly set metatable")
print(result) -- output: patched
```

### `hookmetamethod(obj, name, hook)`
Hooks a metamethod (e.g. `__index`, `__namecall`) on `obj`'s metatable and returns the original.
```lua
local hookedvalue = nil
local original = hookmetamethod(game, "__index", function(self, key)
    hookedvalue = key
    return original(self, key)
end)
local workspace = game.Workspace
assert(hookedvalue == "Workspace", "hooked __index should observe the key being indexed")
print(hookedvalue) -- output: Workspace
```

### `getnamecallmethod()`
Returns the method name currently being invoked inside an active `__namecall` call.
```lua
local capturedmethod = nil
local original = hookmetamethod(game, "__namecall", function(self, ...)
    capturedmethod = getnamecallmethod()
    return original(self, ...)
end)
game:GetService("Workspace")
assert(capturedmethod == "GetService", "captured method should match the namecall used")
print(capturedmethod) -- output: GetService
```

### `setreadonly(table, bool)`
Toggles a table's read-only (frozen) state.
```lua
local t = {}
setreadonly(t, true)
local ok = pcall(function()
    t.x = 1
end)
assert(ok == false, "writing to a readonly table should fail")
print(ok) -- output: false
```

### `makereadonly(table)`
Freezes a table so writes error.
```lua
local t = {}
makereadonly(t)
local result = isreadonly(t)
assert(result == true, "table should be frozen after makereadonly")
print(result) -- output: true
```

### `isreadonly(table)`
Returns whether a table is currently frozen.
```lua
local t = {}
makereadonly(t)
local result = isreadonly(t)
assert(result == true, "isreadonly should reflect the frozen state")
print(result) -- output: true
```

## Miscellaneous

### `identifyexecutor()`
Aliases: `getexecutorname`
Returns the executor's name and version as two strings.
```lua
local name, version = identifyexecutor()
assert(type(name) == "string" and type(version) == "string", "identifyexecutor should return two strings")
print(name, version) -- output: serpent 1.0.0
```

### `setfpscap(fps)`
Aliases: none
Sets the client's frame-rate cap (`0` = unlimited).
```lua
setfpscap(120)
local result = getfpscap()
assert(result == 120, "fps cap should match the value just set")
print(result) -- output: 120
```

### `getfpscap()`
Aliases: none
Returns the current FPS cap.
```lua
local result = getfpscap()
assert(type(result) == "number", "getfpscap should return a number")
print(result) -- output: 120
```

### `request(options)`
Aliases: `http.request`, `http_request`
Sends an HTTP request bypassing CORS, returning `{StatusCode, Body, ...}`.
```lua
local response = request({ Url = "https://example.com", Method = "GET" })
assert(response.StatusCode == 200, "request to example.com should return a 200 status")
print(response.StatusCode) -- output: 200
```

### `lz4compress(data)`
Aliases: `crypt.lz4compress`
Compresses a string using LZ4.
```lua
local compressed = lz4compress("serpent")
assert(type(compressed) == "string", "lz4compress should return a string")
print(#compressed) -- output: a length smaller than or close to the original for short strings
```

### `lz4decompress(data, size)`
Aliases: `crypt.lz4decompress`
Decompresses LZ4-compressed data given the original size.
```lua
local compressed = lz4compress("serpent")
local decompressed = lz4decompress(compressed, 7)
assert(decompressed == "serpent", "decompressed data should match the original string")
print(decompressed) -- output: serpent
```

### `queueonteleport(code)`
Aliases: `queue_on_teleport`
Queues a script to run automatically after the next teleport.
```lua
local ok = pcall(function()
    queueonteleport("print('ran after teleport')")
end)
assert(ok == true, "queueonteleport should accept a source string without erroring")
print(ok) -- output: true
```

### `clearqueueonteleport()`
Aliases: `clearteleportqueue`, `clear_teleport_queue`
Clears any script queued via `queueonteleport`.
```lua
queueonteleport("print('should not run')")
local ok = pcall(function()
    clearqueueonteleport()
end)
assert(ok == true, "clearqueueonteleport should not error")
print(ok) -- output: true
```

## External Console

### `messagebox(text, caption, flags)`
Shows a native OS message box.
```lua
local ok = pcall(function()
    messagebox("hello", "serpent", 0)
end)
assert(ok == true, "messagebox should not error for basic arguments")
print(ok) -- output: true
```

### `rconsoleprint(text)`
Prints to the executor's external console window.
```lua
local ok = pcall(function()
    rconsoleprint("serpent test\n")
end)
assert(ok == true, "rconsoleprint should not error")
print(ok) -- output: true
```

### `rconsoleinfo(text)` / `rconsolewarn(text)` / `rconsoleerr(text)`
Print info/warning/error-level messages to the external console.
```lua
local ok = pcall(function()
    rconsoleinfo("info\n")
    rconsolewarn("warn\n")
    rconsoleerr("error\n")
end)
assert(ok == true, "console log helpers should not error")
print(ok) -- output: true
```

### `rconsoleinput()`
Reads a line of input from the external console.
```lua
local line = rconsoleinput()
assert(type(line) == "string", "rconsoleinput should return a string")
print(line) -- output: whatever the user types into the console
```

### `rconsoleclear()`
Clears the external console.
```lua
local ok = pcall(function()
    rconsoleclear()
end)
assert(ok == true, "rconsoleclear should not error")
print(ok) -- output: true
```

### `rconsolename(name)`
Sets the external console window's title.
```lua
local ok = pcall(function()
    rconsolename("serpent console")
end)
assert(ok == true, "rconsolename should not error")
print(ok) -- output: true
```

### `rconsolehide()` / `rconsoleshow()`
Hides / shows the external console window.
```lua
local ok = pcall(function()
    rconsolehide()
    rconsoleshow()
end)
assert(ok == true, "console visibility toggles should not error")
print(ok) -- output: true
```

## Reflection

### `sethiddenproperty(instance, name, value)`
Aliases: none
Writes to an Instance property that isn't normally scriptable (e.g. `Part.shape`).
```lua
local part = Instance.new("Part")
local ok = sethiddenproperty(part, "shape", 0)
assert(ok == true, "sethiddenproperty should report success")
print(ok) -- output: true
```

### `gethiddenproperty(instance, name)`
Aliases: none
Reads a normally-hidden Instance property; returns value plus whether it was actually hidden.
```lua
local part = Instance.new("Part")
local value, washidden = gethiddenproperty(part, "shape")
assert(washidden == true, "shape should be reported as a hidden property")
print(value, washidden) -- output: 0 true
```

### `setthreadidentity(n)`
Aliases: `setidentity`, `setthreadcontext`
Sets the current thread's Roblox security "identity" level.
```lua
setthreadidentity(8)
local result = getthreadidentity()
assert(result == 8, "identity should match the value just set")
print(result) -- output: 8
```

### `getthreadidentity()`
Aliases: `getidentity`, `getthreadcontext`
Returns the current thread's identity level.
```lua
local result = getthreadidentity()
assert(type(result) == "number", "getthreadidentity should return a number")
print(result) -- output: 8
```

### `setscriptable(instance, property, bool)`
Aliases: none
Toggles whether a property is directly accessible from Lua.
```lua
local part = Instance.new("Part")
setscriptable(part, "shape", true)
local result = isscriptable(part, "shape")
assert(result == true, "shape should now be scriptable")
print(result) -- output: true
```

### `isscriptable(instance, property)`
Aliases: none
Returns whether a property is currently scriptable.
```lua
local part = Instance.new("Part")
local result = isscriptable(part, "Size")
assert(result == true, "Size should be scriptable by default")
print(result) -- output: true
```

### `getrendersteppedlist()`
Aliases: none
Returns the list of functions currently bound via `RunService:BindToRenderStep`.
```lua
game:GetService("RunService"):BindToRenderStep("serpenttest", 1, function() end)
local list = getrendersteppedlist()
assert(type(list) == "table" and list["serpenttest"] ~= nil, "bound step should appear in the list")
print(list["serpenttest"] ~= nil) -- output: true
```

## Scripts

### `getscripts()`
Aliases: none
Returns every LocalScript/ModuleScript/client-context Script currently loaded.
```lua
local scripts = getscripts()
assert(type(scripts) == "table", "getscripts should return a table")
print(#scripts > 0) -- output: true
```

### `getrunningscripts()`
Aliases: none
Returns only scripts that are enabled and actively executing.
```lua
local running = getrunningscripts()
assert(type(running) == "table", "getrunningscripts should return a table")
print(#running > 0) -- output: true
```

### `getcallingscript()`
Aliases: none
Returns the Script/LocalScript instance that called into the current context (or `nil` from executor code).
```lua
local caller = getcallingscript()
assert(caller == nil, "executor code has no calling script")
print(caller) -- output: nil
```

### `getloadedmodules()`
Aliases: none
Returns every ModuleScript that has been `require`'d.
```lua
local modules = getloadedmodules()
assert(type(modules) == "table", "getloadedmodules should return a table")
print(#modules >= 0) -- output: true
```

### `getsenv(script)`
Aliases: none
Returns the sandboxed global environment table for a running script.
```lua
local target = getrunningscripts()[1]
local env = getsenv(target)
assert(type(env) == "table", "getsenv should return a table")
print(type(env)) -- output: table
```

### `getscripthash(script)`
Aliases: none
Returns a hash string uniquely identifying a script's bytecode.
```lua
local target = getrunningscripts()[1]
local hash = getscripthash(target)
assert(type(hash) == "string" and #hash > 0, "getscripthash should return a non empty string")
print(hash) -- output: a hex string unique to that script's bytecode
```

### `loadstring(source, chunkname?)`
Aliases: none
Compiles a source string into a callable function.
```lua
local fn = loadstring("return 1 + 1")
assert(type(fn) == "function", "loadstring should return a callable function")
local result = fn()
assert(result == 2, "compiled chunk should evaluate to 2")
print(result) -- output: 2
```

### `require(moduleScript)`
Aliases: none
Loads and caches a ModuleScript's return value, sharing state across the game/executor.
```lua
local module = Instance.new("ModuleScript")
module.Source = "return 42"
module.Parent = game.ReplicatedStorage
local result = require(module)
assert(result == 42, "requiring the module should return the value it returns")
print(result) -- output: 42
```

### `getscriptbytecode(script)`
Aliases: `dumpstring`
Returns the raw compiled Luau bytecode of a script.
```lua
local target = getrunningscripts()[1]
local bytecode = getscriptbytecode(target)
assert(type(bytecode) == "string" and #bytecode > 0, "getscriptbytecode should return non empty bytecode")
print(#bytecode > 0) -- output: true
```

### `getscriptclosure(script)`
Aliases: `getscriptfunction`
Returns a callable closure equivalent to a script's top-level function.
```lua
local target = getrunningscripts()[1]
local closure = getscriptclosure(target)
assert(type(closure) == "function", "getscriptclosure should return a callable function")
print(type(closure)) -- output: function
```

### `decompile(script)`
Aliases: none
Decompiles a script's bytecode back into readable Lua source.
```lua
local target = getrunningscripts()[1]
local source = decompile(target)
assert(type(source) == "string" and #source > 0, "decompile should return non empty source")
print(#source > 0) -- output: true
```

## Native Signals

### `Signal.new()`
Constructs a Roblox-native `RBXScriptSignal`-like object supporting `Connect`, `Once`, `Wait`, `Fire`, with connections exposing `Disconnect`.
```lua
local signal = Signal.new()
local received = false
local connection = signal:Connect(function()
    received = true
end)
signal:Fire()
assert(received == true, "handler should run after Fire")
connection:Disconnect()
print(received) -- output: true
```

## Signal Manipulation

### `cansignalreplicate(signal)`
Returns whether firing a signal would replicate to the server.
```lua
local part = Instance.new("Part")
local result = cansignalreplicate(part.Touched)
assert(type(result) == "boolean", "cansignalreplicate should return a boolean")
print(result) -- output: false
```

### `firesignal(signal, ...)`
Fires a signal's connected handlers directly (locally), including inside Actors.
```lua
local part = Instance.new("Part")
local touched = false
part.Touched:Connect(function()
    touched = true
end)
firesignal(part.Touched, part)
assert(touched == true, "Touched handler should run after firesignal")
print(touched) -- output: true
```

### `getconnections(signal)`
Returns a list of `ConnectionProxy` objects (with `Enabled`, `Function`, `Thread`, `Fire`, `Defer`, `Disconnect`, `Disable`, `Enable`) for a signal.
```lua
local part = Instance.new("Part")
part.Touched:Connect(function() end)
local connections = getconnections(part.Touched)
assert(type(connections) == "table" and #connections == 1, "one connection should be reported")
print(#connections) -- output: 1
```

### `getsignalarguments(signal)`
Returns the argument names/types of the last firing of a signal.
```lua
local part1 = Instance.new("Part")
local part2 = Instance.new("Part")
part1.Touched:Connect(function() end)
firetouchinterest(part1, part2, 0)
local args = getsignalarguments(part1.Touched)
assert(type(args) == "table", "getsignalarguments should return a table")
print(#args > 0) -- output: true
```

### `getsignalargumentsinfo(signal)`
Returns detailed `{Name, Type}` info for a signal's arguments.
```lua
local part = Instance.new("Part")
local info = getsignalargumentsinfo(part.Touched)
assert(type(info) == "table", "getsignalargumentsinfo should return a table")
print(#info > 0) -- output: true
```

### `getsignalwhitelist()`
Returns the list of signals the executor allows client-side firing/filtering on.
```lua
local whitelist = getsignalwhitelist()
assert(type(whitelist) == "table", "getsignalwhitelist should return a table")
print(#whitelist > 0) -- output: true
```

### `replicatesignal(signal, player, ...)`
Replicates a signal firing to the server as if a given player triggered it.
```lua
local part = Instance.new("Part")
local ok = pcall(function()
    replicatesignal(part.Touched, game.Players.LocalPlayer, part)
end)
assert(ok == true, "replicatesignal should not error for a valid signal and player")
print(ok) -- output: true
```

## Actors / Parallel Luau

### `isparallel()`
Returns whether the current thread is running in a parallel (desynchronized) Actor context.
```lua
local result = isparallel()
assert(result == false, "executor console code should not be running in a parallel actor")
print(result) -- output: false
```

### `run_on_actor(actor, code, ...)`
Executes a code string (or compiled chunk) on a specific `Actor` instance, passing arguments across the VM boundary.
```lua
local actor = Instance.new("Actor")
actor.Parent = workspace
local ok = pcall(function()
    run_on_actor(actor, "print('running on actor')")
end)
assert(ok == true, "run_on_actor should not error for a valid actor and code string")
print(ok) -- output: true
```

### `create_comm_channel()`
Creates a cross-Actor communication channel; returns an ID and an event object with `Connect`/`Once`/`Wait`/`Fire`.
```lua
local id, event = create_comm_channel()
assert(id ~= nil and event ~= nil, "create_comm_channel should return an id and an event object")
print(id ~= nil, event ~= nil) -- output: true true
```

### `get_comm_channel(id)`
Retrieves an existing communication channel by ID.
```lua
local id, event = create_comm_channel()
local fetched = get_comm_channel(id)
assert(fetched ~= nil, "get_comm_channel should return the same channel for a valid id")
print(fetched ~= nil) -- output: true
```

### `getactors()`
Returns a list of all `Actor` instances currently in the game.
```lua
local actor = Instance.new("Actor")
actor.Parent = workspace
local actors = getactors()
local found = false
for i, entry in actors do
    if entry == actor then
        found = true
    end
end
assert(found == true, "the actor just created should appear in getactors")
print(found) -- output: true
```

## WebSocket

### `WebSocket.connect(url)`
Opens a WebSocket connection; returns an object with `Send`, `Close`, and `OnMessage`/`OnClose` signal-style event objects. Supports binary (`is_binary`) sends.
```lua
local socket = WebSocket.connect("wss://echo.websocket.org")
assert(socket ~= nil, "WebSocket.connect should return a socket object")
local received = nil
socket.OnMessage:Connect(function(message)
    received = message
end)
socket:Send("hello")
task.wait(1)
assert(received == "hello", "echo server should send back the same message")
print(received) -- output: hello
socket:Close()
```
