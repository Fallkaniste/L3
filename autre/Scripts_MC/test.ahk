!x::

xpos := 430
ypos := 740
xtrash := 145
ytrash := 694
Send, {tab}

  Loop, 9
  {
     MouseClickDrag, Left, xpos, ypos, xtrash, ytrash, 1 
     Click (by itself)
     xpos :=  xpos + 56
  }
  xpos = 430
  ypos = 810
Loop, 9
  {
     MouseClickDrag, Left, xpos, ypos, xtrash, ytrash, 1
     Click (by itself)
     xpos :=  xpos + 56
  }
  xpos = 430
  ypos = 860
Loop, 9
  {
     MouseClickDrag, Left, xpos, ypos, xtrash, ytrash, 1
     Click (by itself)
     xpos :=  xpos + 56
  }
  xpos = 486
  ypos = 940
Loop, 8
  {
     MouseClickDrag, Left, xpos, ypos, xtrash, ytrash, 1
     Click (by itself)
     xpos :=  xpos + 56
  }

return

!w::
reload
return