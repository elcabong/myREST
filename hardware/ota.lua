local chkstr = ""
local rst_cnt = 0
gpio.mode(1,gpio.OUTPUT)
gpio.write(1,gpio.HIGH)
uart.setup(0,57600,8,0,1,0)
srv=net.createServer(net.TCP) 
print("Server started")
srv:listen(2323,function(conn)
  conn:on("receive",function(conn,request)
    if request == "quit\r\n" then
      uart.on("data")
      conn:close()
      srv:close()
      node.restart()

    else
      if string.find(request, "0 ") then
        gpio.write(1,gpio.LOW)
      end
      uart.write(0, request)
      if request == "Q " then
        uart.on("data", 1,
          function(data)
            conn:send(data)
            if data == string.char(0x10) then
              uart.on("data")
              conn:close()
              srv:close()
              node.restart()
            end
          end, 0)
      end
    end
  end)
  uart.on("data", 1, 
    function(data)
      chkstr = chkstr .. data
      chkstr = string.sub(chkstr, -5)
      if chkstr == "quit\r" then
        uart.on("data")
        conn:close()
        srv:close()
        node.restart()
      else
        conn:send(data)
      end
    end, 0)
end)

