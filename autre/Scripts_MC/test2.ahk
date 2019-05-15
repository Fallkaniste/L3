
^x::

Send, {t}
    Sleep, 300
  Send, {/}warp
    Sleep, 300
  Send, {Enter}
    Sleep, 500

  MouseClick, Left, 535, 625, 5
    Sleep, 5000

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

return
