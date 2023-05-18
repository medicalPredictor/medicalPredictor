import tkinter as tk
import matplotlib.pyplot as plt
import numpy as np
import os 

# Init windows and titles
params = tk.Tk()
params.title("Parameters")
averagedSims = tk.Tk()
averagedSims.title("Averaged Simulations")

# Test graph data
testData = np.arange(0.0,2.0,0.01)
# Make/plot/save data
fig,ax = plt.subplots()
ax.plot(testData,testData) # More useful tags if needed
ax.set(xlabel = 'Beep', ylabel ='Boop', title = 'Testing') # fig is now our plot
fig.savefig("test.png")

# Get image for gui
pic = tk.PhotoImage(file="test.png")

message = tk.Label(params,text='Test',cursor='gobbler') # makes the message
message.grid() # puts the message in the textbox
myPlot = tk.Label(averagedSims,image=pic)
myPlot.pack()

# Turn it on
tk.mainloop()
