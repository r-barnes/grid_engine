#!/usr/bin/python
from Tkinter import *
import sys

class GridDrawer(Frame):
  def __init__(self):
    Frame.__init__(self)
    self.pack(expand=YES, fill=BOTH)
    self.master.title('Grid Plotter')

    canvas = Canvas()
    canvas.pack(expand=YES, fill=BOTH)
    self.canvas = canvas
    self.size=20

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

def main():
  if len(sys.argv)!=3:
    print "%s <hex/d4/d8> <coordinates file>" % (sys.argv[0])
    sys.exit(-1)

  h=GridDrawer()
  for x in range(-30,10):
    for y in range(-30,10):
      h.DrawHex(x,y,"white")

  h.mainloop()

main()
