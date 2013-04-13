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
    self.xoffset=200
    self.yoffset=200

  def refresh(self):
    self.canvas.configure(scrollregion=self.canvas.bbox("all"))

  def DrawSquare(self, x, y, filler):
    orgx=x*self.size+self.xoffset
    orgy=y*self.size+self.yoffset
    self.hex = self.canvas.create_polygon(
      orgx, orgy,
      orgx+self.size, orgy,
      orgx+self.size, orgy+self.size,
      orgx, orgy+self.size,
      orgx, orgy,
      outline="black", fill=filler
    )

    self.canvas.create_text(orgx+self.size/2, orgy+self.size/2, text=str(x)+","+str(y))

  def DrawHex(self, x, y, filler):
    oneseg = self.size/4
    twoseg = oneseg*2  # same as size/2
    orgy=y*self.size+(x%2)*self.size/2 + self.yoffset
    orgx=x*self.size*3/4.              + self.xoffset

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

    self.canvas.create_text(orgx+twoseg, orgy+twoseg, text=str(x)+","+str(y))

def toroid(i,imax):
  if i>=0:
    return i%imax
  else:
    return imax-((-i)%imax)

def main():
  h=GridDrawer()

  if len(sys.argv)!=3:
    print "%s <HexEven/HexOdd/D4/D8> <coordinates file>" % (sys.argv[0])
    sys.exit(-1)

  drawer=None
  if sys.argv[1]=='HexEven' or sys.argv[1]=='HexOdd':
    drawer=h.DrawHex
  elif sys.argv[1]=='D4' or sys.argv[1]=='D8':
    drawer=h.DrawSquare
  else:
    print "Unrecognised connectivity!"
    sys.exit(-1)

  with open(sys.argv[2],'r') as fin:
    data=fin.readlines()

  for l in data:
    if len(l.strip())==0:
      continue
    if l.startswith("int " + sys.argv[1] + "::begins"):
      begins=[int(i) for i in l[l.index('{')+1:l.index('}')].split(',')]
    if l.startswith("int " + sys.argv[1] + "::dx"):
      dx=[int(i) for i in l[l.index('{')+1:l.index('}')].split(',')]
    if l.startswith("int " + sys.argv[1] + "::dy"):
      dy=[int(i) for i in l[l.index('{')+1:l.index('}')].split(',')]

  for x in range(0,6):
    for y in range(0,4):
      drawer(x,y,"white")

  x0=2
  y0=0

  drawer(x0,y0,"blue")

  for i in range(begins[3],begins[4]):
    drawer(x0+dx[i],y0+dy[i],"red")

  h.refresh()

  h.mainloop()

main()
