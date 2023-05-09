from tkinter import *
from PIL import ImageTk, Image
import matplotlib.pyplot as plt


params = Tk() # I dont actually know what to do with this window 

averagedSims = Toplevel()

averagedSims.title("Averaged Simulations")
averagedSims.geometry("1000x800")
avgImage = Frame(averagedSims,width = 700, height = 600)
avgImage.pack() # 
avgImage.place(anchor = 'center', relx=0.5, rely = 0.5)

testData = []
[testData for x in range(100)]
plt.plot(testData,testData,'ro')
plt.savefig("testData.png")

avgGraph = ImageTk.PhotoImage(Image.open("testData.png"))
avgLabel = Label(avgImage, image = avgGraph)
avgLabel.pack()




mainloop()


