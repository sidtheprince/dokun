WINDOW_WIDTH  = 1280
WINDOW_HEIGHT = 720
WINDOW_X = 200
WINDOW_Y = 200
WINDOW_NAME = "Test"
WINDOW_COLOR = {32, 32, 32}
--[[
window = Window:new()
window:create("Test", 1280, 720)
window:show()
while window:is_open() do
    window:set_viewport(window:get_width(), window:get_height())
	window:clear(32, 32, 32)
    window:update()
end
window:destroy()
dokun:close   ()
]]--
