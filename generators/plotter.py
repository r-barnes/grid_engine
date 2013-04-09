#!/usr/bin/python
from Tkinter import *

def frame(root, side):
  w = Frame(root)
  w.pack(side=side, expand=YES, fill=BOTH)
  return w

class HexDemo(Frame):
  def __init__(self):
    Frame.__init__(self)
    self.pack(expand=YES, fill=BOTH)
    self.master.title('Hex Drawing Demo')
    self.master.iconname('hexdraw')

    canvas = Canvas()
    canvas.pack(expand=YES, fill=BOTH)
    self.canvas = canvas
    self.size=20

    for x in range(-10,10):
      for y in range(-10,10):
        print x,y
        self.DrawHex(x,y,"white")

  def DrawSquare(self, orgx, orgy, filler):
    orgx=orgx*self.size
    orgy=orgy*self.size
    self.hex = self.canvas.create_polygon(
      orgx, orgy,
      orgx+self.size, orgy,
      orgx+self.size, orgy+self.size,
      orgx, orgy+self.size,
      orgx, orgy,
      outline="black", fill=filler
    )


  def DrawHex(self, orgx, orgy, filler):
    oneseg = self.size/4
    twoseg = oneseg*2  # same as size/2
    orgy=orgy*self.size+(orgx%2)*self.size/2
    orgx=orgx*self.size*3/4.

    # Create a new one
    self.hex = self.canvas.create_polygon( 
    orgx, orgy + twoseg,
    orgx + oneseg, orgy,
    orgx + oneseg + twoseg, orgy,
    orgx + self.size , orgy + twoseg,
    orgx + oneseg + twoseg, orgy + self.size,
    orgx + oneseg, orgy + self.size,
    orgx, orgy + twoseg,
    outline="black",fill=filler)


if __name__ == '__main__':

  HexDemo().mainloop()

  
