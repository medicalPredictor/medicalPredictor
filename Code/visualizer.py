import tkinter as tk
from PIL import ImageTk, Image
import matplotlib.pyplot as plt
import time

# Note, pic must be a UNIQUE variables
def displayGraph(winTitle,winSize,pic):
# Makes a new window with an image in the middle
    newWin = tk.Toplevel() 
    newWin.title(winTitle)
    newWin.geometry(winSize) # above 3 makes new window
    imageHolder = tk.Frame(newWin,width = 700, height = 600)
    imageHolder.pack()
    imageHolder.place(anchor = 'center', relx=0.5, rely = 0.5) # above 3 makes new image
    myPlot = tk.Label(newWin,image=pic) 
    myPlot.pack()
    return(newWin,imageHolder,myPlot)

def displayParams(winTitle,windSize,pic):
    # newWin = tk.Toplevel()
    # newWin.title(winTitle)
    return()



# want COMPLETE file name (DATA.DAT)
# Data file should be in format in github. Pull up example file if needed
def pullSim(filename):
    # Pull from file
    data = open(filename,'r')
    contents = data.read()
    print(contents)
    # Now pull the time series data and keep it as a list    
    return()

def pullSim(filename, delimiters = ["___","Start date: "]):
    f = open(filename,'r')
    unrefined = f.readlines()
    test = "Start Date: 34"
    print(test[len(delimiters[1]):])


    # for line in unrefined:
    #     if line == 
    return()



# Init of GUI
# params = tk.Tk() # The first window
# params.title("Medical Predictor Data Visualizer")
# title = 'Averaged Simulations'; size = '1000x800';pic = tk.PhotoImage(file="test.png")
# displayGraph(title,size,pic)
# title2 = 'Tomato'; size2 = '1000x800';pic2 = tk.PhotoImage(file="tomato.png")
# displayGraph(title2,size2,pic2)
data = pullSim("DATA.DAT")
print(data)



# Turn it on
# tk.mainloop()