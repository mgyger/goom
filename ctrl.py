#!/usr/bin/env python

# Copyright 2014 Mark Owen
# http://www.quinapalus.com
# E-mail: goom@quinapalus.com
#
# This file is free software: you can redistribute it and/or modify
# it under the terms of version 2 of the GNU General Public License
# as published by the Free Software Foundation.
#
# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this file.  If not, see <http://www.gnu.org/licenses/> or
# write to the Free Software Foundation, Inc., 51 Franklin Street,
# Fifth Floor, Boston, MA  02110-1301, USA.

import pypm
import pygtk
pygtk.require('2.0')
import gtk
import json
import time

NCHAN=15 # number of MIDI channels we control (2...NCHAN+1)
NCTRL=24 # number of controllers

a=[[0]*NCTRL for i in range(NCHAN)] # Adjustment for each controller
v=[[0]*NCTRL for i in range(NCHAN)] # VScale for each adjustment

settings=[[ 0,0,0,0,0,0,0,0,64,64,64,64,0,127,127,0,127,0,0,64,127,64,64,64] for i in range(NCHAN)] # controller default settings
tlab=[  # top labels for sliders
"Saw", "Squ", "", "", "", "", "", "",
"", "", "", "",
"", "", "", "",
"Saw", "Squ", "",
"", "", "",
"", "R"]
blab=[  # bottom labels for sliders
"Tri", "Tri", "", "", "", "", "", "",
"", "", "", "",
"", "", "", "",
"Tri", "Tri", "",
"", "", "",
"", "L"]
lab=[   # labels below sliders
"Duty cycle", "Waveform", "Coarse", "Fine", "A", "D", "Level", "",
"A", "D", "S", "R",
"A", "D", "S", "R",
"Duty cycle", "Waveform", "",
"Rate", "Cutoff", "Res",
"Vol", "  Pan  "]

hptoct=[0,1,7,2,3,4,5,6,18,16,17,8,9,10,11,19,20,21,12,13,14,15,22,23]  # map horizontal position to controller number
section=["OSC 1","","","","","","","","","OSC 2","","VCF","","","","","","","VCA","","",""," ",""] # section labels

class goomctrl:
  def valchange(self,item,chan,ctrl,val=None): # called on slider update
    global settings
    if val==None: val=a[chan][ctrl].value  # get value unless given explicitly in call
    val=int(val)
    if(ctrl< 16):
      midi.WriteShort(0xb0+chan+1,ctrl+16    ,val)  # convert to Goom MIDI controller number and send control change message
    if(ctrl>=16):
      midi.WriteShort(0xb0+chan+1,ctrl+102-16,val)
    settings[chan][ctrl]=val # update settings

  def getfn(self,title,action):  # get a filename
    dialog = gtk.FileChooserDialog(title,None,action,(gtk.STOCK_CANCEL,gtk.RESPONSE_CANCEL,gtk.STOCK_OK, gtk.RESPONSE_OK))
    dialog.set_default_response(gtk.RESPONSE_OK)
    filter=gtk.FileFilter()
    filter.set_name("All files")
    filter.add_pattern("*")
    dialog.add_filter(filter)
    response = dialog.run()
    if response==gtk.RESPONSE_OK:
      fn=dialog.get_filename()
    else:
      fn=None
    dialog.destroy()
    return fn

  def updateone(self,item,chan): # update everything on one channel
    for j in range(NCTRL):
      if (j==7) | (j==18):  # these are radio buttons rather than sliders
        if   settings[chan][j]<32: v[chan][j][0].activate()
        elif settings[chan][j]<96: v[chan][j][1].activate()
        else              : v[chan][j][2].activate()
      else: v[chan][j].set_value(settings[chan][j])
      self.valchange(None,chan,j,settings[chan][j])

  def updateall(self,item):  # update everything on all channels
    for i in range(NCHAN):
      self.updateone(None,i)

  def loadone(self,item,chan):  # load preset for one channel only
    global settings
    fn=self.getfn("Load channel %d"%(chan+1),gtk.FILE_CHOOSER_ACTION_OPEN)
    if fn==None: return
    try:
      with open(fn,'r') as f:
        settings[chan]=json.load(f)
      f.close
    except IOError:
      print "IOError"
      pass
    self.updateone(item,chan)

  def saveone(self,item,chan):  # save preset for one channel only
    fn=self.getfn("Save channel %d"%(chan+1),gtk.FILE_CHOOSER_ACTION_SAVE)
    if fn==None: return
    with open(fn,'w') as f:
      json.dump(settings[chan],f)
    f.close

  def loadall(self,item):  # load all presets
    global settings
    fn=self.getfn("Load all channels",gtk.FILE_CHOOSER_ACTION_OPEN)
    if fn==None: return
    try:
      with open(fn,'r') as f:
        settings=json.load(f)
      f.close
    except IOError:
      print "IOError"
      pass
    self.updateall(None)

  def saveall(self,item):  # save all presets
    fn=self.getfn("Save all channels",gtk.FILE_CHOOSER_ACTION_SAVE)
    if fn==None: return
    with open(fn,'w') as f:
      json.dump(settings,f,indent=2)
    f.close

  def __init__(self):  # create widgets and window
    self.window = gtk.Window (gtk.WINDOW_TOPLEVEL)
    self.window.connect("destroy", lambda w: gtk.main_quit())
    self.window.set_title("Goom MIDI controller")
    self.window.set_default_size(1280,500)
    b2=gtk.VBox(False,10)
    self.window.add(b2)
    sw=gtk.ScrolledWindow()
    sw.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_ALWAYS)
    b2.pack_start(sw,True,True,0)
    b1=gtk.VBox(False,10)
    sw.add_with_viewport(b1)
    b0=gtk.HBox(False,10)  # box for buttons across top
    lbut=gtk.Button("Load all")
    lbut.connect("clicked",self.loadall)
    b0.pack_start(lbut,True,True,0)
    ubut=gtk.Button("Send all")
    ubut.connect("clicked",self.updateall)
    b0.pack_start(ubut,True,True,0)
    sbut=gtk.Button("Save all")
    sbut.connect("clicked",self.saveall)
    b0.pack_start(sbut,True,True,0)
    qbut=gtk.Button("Quit")
    qbut.connect("clicked",lambda w:gtk.main_quit())
    b0.pack_start(qbut,True,True,0)
    b1.pack_start(b0,True,True,0)
    for chan in range(NCHAN):
      b0=gtk.HBox(False,10)  # box to contain everything for one channel
#      l=gtk.Label("Channel %d"%(chan+2))
#      l.set_angle(90)
#      b0.pack_start(l,True,True,0)
      b=gtk.VBox(False,10)   # box to contain individual channel load/send/save buttons
      l=gtk.Label("Chan %d"%(chan+2))
      b.pack_start(l,True,True,0)
      lbut=gtk.Button("Load")
      lbut.connect("clicked",self.loadone,chan)
      b.pack_start(lbut,True,True,0)
      ubut=gtk.Button("Send")
      ubut.connect("clicked",self.updateone,chan)
      b.pack_start(ubut,True,True,0)
      sbut=gtk.Button("Save")
      sbut.connect("clicked",self.saveone,chan)
      b.pack_start(sbut,True,True,0)
      b0.pack_start(b,True,True,0)
      for hpos in range(NCTRL):  # pack in the sliders
        if(section[hpos]!=""):
          b0.pack_start(gtk.VSeparator(),True,True,0)
          if(section[hpos]!=" "):
            l=gtk.Label(section[hpos])
            l.set_angle(90)
            b0.pack_start(l,True,True,0)
        ctrl=hptoct[hpos]
        b=gtk.VBox(False,10)

        if ctrl==7:  # special cases for the radio buttons
          r0=gtk.RadioButton(None,"Key")
          r1=gtk.RadioButton(r0,"High")
          r2=gtk.RadioButton(r0,"Low")
          r0.connect("clicked",self.valchange,chan,ctrl,0)
          r1.connect("clicked",self.valchange,chan,ctrl,64)
          r2.connect("clicked",self.valchange,chan,ctrl,127)
          v[chan][ctrl]=[r0,r1,r2]
          b.pack_start(r0,True,True,0)
          b.pack_start(r1,True,True,0)
          b.pack_start(r2,True,True,0)

        elif ctrl==18:
          r0=gtk.RadioButton(None,"Mix")
          r1=gtk.RadioButton(r0,"FM")
          r2=gtk.RadioButton(r0,"FM+\nFB")
          r0.connect("clicked",self.valchange,chan,ctrl,0)
          r1.connect("clicked",self.valchange,chan,ctrl,64)
          r2.connect("clicked",self.valchange,chan,ctrl,127)
          v[chan][ctrl]=[r0,r1,r2]
          b.pack_start(r0,True,True,0)
          b.pack_start(r1,True,True,0)
          b.pack_start(r2,True,True,0)

        else:
          a[chan][ctrl]=gtk.Adjustment(64.0,0.0,128.0,1.0,1.0,1.0)
          a[chan][ctrl].connect("value_changed",self.valchange,chan,ctrl)
          v[chan][ctrl]=gtk.VScale(a[chan][ctrl])
          v[chan][ctrl].set_value_pos(gtk.POS_BOTTOM)
          v[chan][ctrl].set_update_policy(gtk.UPDATE_CONTINUOUS)
          v[chan][ctrl].set_digits(0)
          v[chan][ctrl].set_inverted(True)
          v[chan][ctrl].set_draw_value(True)
          v[chan][ctrl].set_size_request(20,100)
          if tlab[ctrl]!="": v[chan][ctrl].add_mark(127.0,gtk.POS_LEFT,tlab[ctrl])
          if blab[ctrl]!="": v[chan][ctrl].add_mark(0.0,gtk.POS_LEFT,blab[ctrl])
          b.pack_start(v[chan][ctrl],True,True,0)
          b.pack_start(gtk.Label(lab[ctrl]))
        b0.pack_start(b,True,True,0)

      b1.pack_start(b0,True,True,0)
      b1.pack_start(gtk.HSeparator(),True,True,0)
    self.updateall(None)
    self.window.show_all()

def main():
  gtk.main()
  return 0            

if __name__=="__main__":
  pypm.Initialize()
  for loop in range(pypm.CountDevices()):  # list available MIDI devices
    interf,name,inp,outp,opened=pypm.GetDeviceInfo(loop)
    if outp==1:
      print loop,name," ",
      if (inp==1): print "(input) ",
      else: print "(output) ",
      if (opened==1): print "(opened)"
      else: print "(unopened)"
  print
  dev=int(raw_input("Enter output number: "))
  midi=pypm.Output(dev, 0)
  goomctrl()
  main()
  del midi
  pypm.Terminate()
