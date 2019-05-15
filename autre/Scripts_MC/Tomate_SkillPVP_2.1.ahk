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

  MouseClick, Right, 294, 688, 5
  Sleep 300
  Send, 99999
  Sleep, 300
  Mouseclick, Left, 811, 932, 5
  Sleep, 300
  Send, {esc}
}


      !x::
Loop,
{
  Send, {g}

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
  Sleep, 500
  Send, {Enter}
  Sleep, 6000

  Send, {s down}
  Send, {d down}
  Sleep, 500
  Send, {d up}
  Send, {s up}
  Sleep, 300


  Sleep, 500

	; 1er Déplacement dans la farm :      ------------------------------------------------------

Side := 0

Loop, 3
{
  if Side = 0
  {
    Gauche()
    Side := 1
  }
  else
  {
    Droite()
    Side := 0
  }
}

    ; TP / Anti-AFK / Vente

  WarpShop()

  VenteNoAFK()

    ; 2nd Déplacement dans la farm :      ------------------------------------------------------

Side := 1

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
  Send, {}
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
  Sleep, 6000

  Send, {s down}
  Send, {q down}
  Sleep, 500
  Send, {q up}
  Send, {s up}
  Sleep, 300

  Loop, 3
  {
    if Side = 0
    {
      Gauche()
      Side := 1
    }
    else
    {
      Droite()
      Side := 0
    }
  }

    ; TP / Anti-AFK / Vente

  WarpShop()
  VenteNoAFK()
  
}
      return
