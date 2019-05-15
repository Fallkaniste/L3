WarpShop()  ;------------------------------------------------------------------------------
{

  Send, {t}
  Sleep, 500
  Send, {/}
  Sleep, 100
  Send, w
  Sleep, 100
  Send, a
  Sleep, 100
  Send, r
  Sleep, 100
  Send, p
    Sleep, 500
  Send, {Enter}
    Sleep, 500

  MouseClick, Left, 535, 625, 5
    Sleep, 15000

}

VenteNoAFK()  ;-------------------------------------------------------------------------------
{
  Send, {z down}
  Sleep, 400
  Send, {z up}

  Sleep, 500
  Send, {q down}
  Sleep, 3000
  Send, {q up}

  Sleep, 500

  Send, {z down}
  Loop, 50
  {
    Send, {a}
    Send, {Leftclick}
    Sleep, 100
  }
  Send {z up}

  Sleep, 500

    ; Vente

  MouseClick, Right, 286, 894, 5
  Sleep 300
  Send, 99999
  Sleep, 300
  Mouseclick, Left, 811, 932, 5
  Sleep, 300
  Send, {esc}

}

Mange() ;------------------------------------------------------------------------
{
  Send, {v}
  Send, {a down}
  Sleep, 5000
  Send, {a up}
  Sleep, 300
}

ViderInv() ;---------------------------------------------------------------------
{
xpos := 430
ypos := 740
xtrash := 145
ytrash := 694
Send, {tab}
Sleep, 500

  Loop, 9
  {
     MouseClickDrag, Left, xpos, ypos, xtrash, ytrash, 2
     Click (by itself)
     xpos :=  xpos + 56
  }
  xpos = 430
  ypos = 810
Loop, 9
  {
     MouseClickDrag, Left, xpos, ypos, xtrash, ytrash, 2
     Click (by itself)
     xpos :=  xpos + 56
  }
  xpos = 430
  ypos = 860
Loop, 9
  {
     MouseClickDrag, Left, xpos, ypos, xtrash, ytrash, 2
     Click (by itself)
     xpos :=  xpos + 56
  }
  xpos = 486
  ypos = 940
Loop, 7
  {
     MouseClickDrag, Left, xpos, ypos, xtrash, ytrash, 2
     Click (by itself)
     xpos :=  xpos + 56
  }


  Sleep, 500
Send, {tab}
Sleep, 500
Send, {s down}
Sleep, 2000
Send, {s up}
Sleep, 500
  Send, {g} ; touche 1ere case de l'inv
Sleep, 500
}


      !x::

Loop,
{

	; Replacement :

  Send, {t}
Sleep, 500
  Send, {/}
Sleep, 100
  Send, h
Sleep, 100
  Send, o
Sleep, 100
  Send, m
Sleep, 100
  Send, e
Sleep, 100
  Send, {space}
Sleep, 100
  Send, f
Sleep, 100
  Send, a
Sleep, 100
  Send, r
Sleep, 100
  Send, m	
Sleep, 100
  Send, 2
  Sleep, 500
  Send, {Enter}
  Sleep, 7000



	; Déplacement dans la farm :      ------------------------------------------------------

Send, {s down}
Send, {d down}
  Sleep, 1000
Send, {s up}
Send, {d up}

Send, {a down}
Loop, 10
{
  Send, {z down}
  Sleep, 8000
  Send, {z up}
  Sleep, 300
Send, {q down}
  Sleep, 440
Send, {q up}
Send, {s down}
  Sleep, 9000
  Send, {s up}
Send, {q down}
  Sleep, 440
Send, {q up}
}

Send, {s down}
Send, {q down}
  Sleep, 1000
Send, {s up}
Send, {q up}

Loop, 10
{
  Send, {z down}
  Sleep, 8000
  Send, {z up}
  Sleep, 300
Send, {d down}
  Sleep, 440
Send, {d up}
Send, {s down}
  Sleep, 9000
  Send, {s up}
Send, {d down}
  Sleep, 440
Send, {d up}
}

Send, {a up}


    ; TP / Anti-AFK / Vente

  WarpShop()

  VenteNoAFK()

  ; Mange()
}

      return

!w::
reload
return
