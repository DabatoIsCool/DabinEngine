dabin_init()
dabin_createwindow(800, 600, "test")

while true do
	dabin_getevents()
	if dabin_iscloserequested() then
		dabin_destroywindow()
		break
	end
	dabin_clearwindow()
	dabin_updatewindow()
end