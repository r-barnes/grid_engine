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
  h=GridDrawer()

  if len(sys.argv)!=3:
    print "%s <hex/d4/d8> <coordinates file>" % (sys.argv[0])
    sys.exit(-1)

  drawer=None
  if sys.argv[1]=='hex':
    drawer=h.DrawHex
  elif sys.argv[1]=='d4' or sys.argv[1]=='d8':
    drawer=h.DrawSquare

  with open(sys.argv[2],'r') as fin:
    data=fin.readlines()

  for l in data:
    if len(l.strip())==0:
      continue
    lfp=l.index('{')
    llp=l.index('}')
    if l.startswith(sys.argv[1] + "_begins"):
      begins=[int(i) for i in l[lfp+1:llp].split(',')]
    if l.startswith(sys.argv[1] + "_dx"):
      dx=[int(i) for i in l[lfp+1:llp].split(',')]
    if l.startswith(sys.argv[1] + "_dy"):
      dy=[int(i) for i in l[lfp+1:llp].split(',')]

  for x in range(-30,10):
    for y in range(-30,10):
      drawer(x,y,"white")

  drawer(0,0,"blue")

  for i in range(begins[3],begins[4]):
    drawer(dx[i],dy[i],"red")

  h.mainloop()

main()
